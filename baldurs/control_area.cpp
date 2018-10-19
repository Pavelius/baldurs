#include "main.h"

using namespace draw;

typedef adat<drawable*, 256>	drawablet;
static point					camera;
static point					camera_size;
static action_s					current_action = ActionGuard;
static adat<creature*, 8>		current_selected;
static void						(creature::*creature_proc)();
const int						camera_step = 16;

static void character_submenu() {
	cursorset cur;
	while(ismodal()) {
		auto player = creature::getplayer();
		if(!player)
			break;
		(player->*creature_proc)();
		menumodal();
	}
}

static void adventure_step() {
	creature_proc = 0;
	setnext(creature::adventure);
}

static void choose_menu(void (creature::*proc)()) {
	if(creature_proc == proc)
		adventure_step();
	else {
		creature_proc = proc;
		setnext(character_submenu);
	}
}

static void choose_menu(void(*proc)()) {
	if(isnext(proc))
		adventure_step();
	else
		setnext(proc);
}

static void setplayer(creature* player) {
	current_selected.clear();
	current_selected.add(player);
}

static void nothing() {}
static void choose_action() { current_action = (action_s)hot.param; }
static void choose_formation() { settings.formation = (formation_s)hot.param; }
static void choose_player() { setplayer((creature*)hot.param); }
static void move_left() { camera.x -= camera_step; }
static void move_right() { camera.x += camera_step; }
static void move_up() { camera.y -= camera_step; }
static void move_down() { camera.y += camera_step; }
static void character_invertory() { choose_menu(&creature::invertory); }
static void character_sheet() { choose_menu(&creature::sheet); }
static void character_spellbook() { choose_menu(&creature::spellbook); }
static void game_option() { choose_menu(creature::options); }
static void game_minimap() { choose_menu(creature::minimap); }
static void game_journal() { choose_menu(creature::journal); }
static void layer_search() { settings.show_search = !settings.show_search; }
static void layer_path() { settings.show_path = !settings.show_path; }

static void change_mode() {
	settings.panel = (setting::mode_s)(settings.panel + 1);
	if(settings.panel > setting::NoPanel)
		settings.panel = setting::PanelFull;
}

static void select_all() {
	for(auto& e : players) {
		if(!current_selected.is(&e))
			current_selected.add(&e);
	}
}

static hotkey movement_keys[] = {{move_left, KeyLeft, "Двигать влево"},
{move_right, KeyRight, "Двигать вправо"},
{move_up, KeyUp, "Двигать вверх"},
{move_down, KeyDown, "Двигать вниз"},
{change_mode, Alpha + 'G', "Изменить режим панелей"},
{}};

static hotkey menu_keys[] = {{character_invertory, Alpha + 'I', "Предметы инвентаря"},
{character_sheet, Alpha + 'C', "Листок персонажа"},
{character_spellbook, Alpha + 'S', "Заклинания персонажа"},
{layer_search, Alpha + Ctrl + 'S', "Наложить фильтр карты поиска"},
{layer_path, Alpha + Ctrl + 'P', "Наложить фильтр карты пути"},
{select_all, Alpha + '=', "Выбрать всех"},
{game_minimap, Alpha + 'M', "Карта местности"},
{game_option, Alpha + 'O', "Опции"},
{game_journal, Alpha + 'J', "Журнал заданий"},
{adventure_step, KeyEscape, "Вернуться в режим приключений"},
{}};

static void hitpoints(int x, int y, int width, int height, int hp, int mhp) {
	if(!mhp || !hp)
		return;
	color c1;
	if(hp == mhp)
		c1 = colors::white;
	else
		c1 = colors::green.mix(colors::red, hp * 255 / mhp);
	width = hp * width / mhp;
	draw::rectf({x, y, x + width, y + height}, c1);
}

static int act(int x, int y, const runable& cmd, action_s id, action_s id_selected) {
	unsigned flags = 0;
	if(id == id_selected)
		flags |= Checked;
	auto i = id * 4;
	button(x, y, cmd, flags, res::GUIBTACT, i + 3, i, i + 1, i + 2, 0, 0, 0, 0);
	return 41;
}

static int act(int x, int y, const runable& cmd, formation_s id) {
	unsigned flags = 0;
	if(id == settings.formation)
		flags |= Checked;
	auto i = id * 4;
	button_states state;
	button(x, y, cmd, flags, res::GUIBTBUT, 0, 0, &state);
	if(state == ButtonPressed) {
		x += 2;
		y += 2;
	}
	draw::image(x + 3, y + 2, res::FORM, id);
	return 41;
}

static int act(int x, int y, const runable& cmd, creature& player, itemdrag* pd) {
	color s0 = colors::green;
	unsigned flags = 0;
	if(current_selected.is(&player))
		flags |= Checked;
	int hp = player.gethits();
	int mhp = player.gethitsmax();
	if(!hp)
		flags |= Disabled;
	button(x, y, cmd, flags, res::GUIRSPOR, 1, 0, 1, 0, 0, 0, 0, true);
	draw::image(x + 2, y + 2, res::PORTS, player.getportrait());
	if(flags&Disabled)
		draw::rectf({x + 2, y + 2, x + 44, y + 44}, colors::red, 128);
	hitpoints(x, y + 48, 49 - 3, 4, hp, mhp);
	return 49;
}

static void render_tiles(rect rc, int mx, int my) {
	auto sp = map::getareasprite();
	if(!sp)
		return;
	int dx = mx / 64;
	int dy = my / 64;
	int x2 = imin(rc.x1 + (map::width / 4 - dx) * 64, rc.x2);
	int y2 = imin(rc.y1 + (map::height / 4 - dy) * 64, rc.y2);
	rc.x1 -= (mx - dx * 64);
	rc.y1 -= (my - dy * 64);
	int y1 = rc.y1;
	while(y1 < y2) {
		int x1 = rc.x1;
		while(x1 < x2) {
			int py = dy + (y1 - rc.y1) / 64;
			int px = dx + (x1 - rc.x1) / 64;
			draw::image(x1, y1, sp, map::gettile(py * 64 + px), 0);
			x1 += 64;
		}
		y1 += 64;
	}
}

static void render_search(const rect& rc, int mx, int my) {
	auto x2 = rc.x1 + imin(mx + rc.width(), map::width * 16);
	auto y2 = rc.y1 + imin(my + rc.height(), map::height * 12);
	auto y1 = rc.y1 + my;
	while(y1 < y2) {
		int x1 = rc.x1 + mx;
		while(x1 < x2) {
			short px = x1 - rc.x1;
			short py = y1 - rc.y1;
			auto index = map::getindex({px, py});
			if(map::isblock(index)) {
				auto x = x1 - mx + rc.x1;
				auto y = y1 - my + rc.y1;
				rectf({x + 1, y + 1, x + 15, y + 11}, colors::black, 0x80);
			}
			x1 += 16;
		}
		y1 += 12;
	}
}

static void correct_camera(const rect& rc, point& camera) {
	camera_size.x = rc.width();
	camera_size.y = rc.height();
	auto mx = rc.width() / 2;
	auto my = rc.height() / 2;
	if(camera.x < mx)
		camera.x = mx;
	if(camera.y < my)
		camera.y = my;
	mx = map::width * 16 - rc.width() / 2;
	my = map::height * 12 - rc.height() / 2;
	if(camera.x > mx)
		camera.x = mx;
	if(camera.y > my)
		camera.y = my;
}

static void add(const rect& srceen, drawablet& source, drawable& e) {
	if(!e.isvisible())
		return;
	if(!e.getrect().intersect(srceen))
		return;
	source.add(&e);
}

static int compare_zorder(const void* p1, const void* p2) {
	drawable* e1 = *((drawable**)p1);
	drawable* e2 = *((drawable**)p2);
	point s1 = e1->getposition();
	point s2 = e2->getposition();
	if(s1.y != s2.y)
		return s1.y - s2.y;
	if(s1.x != s2.x)
		return s1.x - s2.x;
	return e1->getzorder() - e2->getzorder();
}

creature* creature::getplayer() {
	if(!current_selected)
		return 0;
	return current_selected[0];
}

bool creature::isselected() const {
	return current_selected.is((creature* const)this);
}

static drawable* render_area(rect rc, const point origin, const point hotspot) {
	draw::state push;
	draw::setclip(rc);
	// Получим экран
	rect screen;
	screen.x1 = origin.x;
	screen.y1 = origin.y;
	screen.x2 = screen.x1 + rc.width();
	screen.y2 = screen.y1 + rc.height();
	// Нарисуем тайлы
	render_tiles(rc, screen.x1, screen.y1);
	if(settings.show_search)
		render_search(rc, screen.x1, screen.y1);
	// Нарисуем маркеры движения
	for(auto& e : players) {
		if(e) {
			e.render_marker(rc, screen.x1, screen.y1);
			if(settings.show_path)
				e.render_path(rc, screen.x1, screen.y1);
		}
	}
	// Получим объекты
	drawablet drawables;
	for(auto& e : door_data)
		add(screen, drawables, e);
	for(auto& e : region_data)
		add(screen, drawables, e);
	for(auto& e : container_data)
		add(screen, drawables, e);
	for(auto& e : creature_data)
		add(screen, drawables, e);
	for(auto& e : players)
		add(screen, drawables, e);
	for(auto& e : animation_data)
		add(screen, drawables, e);
	for(auto& e : floattext_data) {
		if(e)
			add(screen, drawables, e);
	}
	drawable* result = 0;
	if(drawables) {
		qsort(drawables.data, drawables.count, sizeof(drawables.data[0]), compare_zorder);
		if(hotspot.in(screen)) {
			for(auto pp = drawables.data + drawables.count - 1; pp >= drawables.data; pp--) {
				auto p = *pp;
				if(p->hittest(hotspot)) {
					result = p;
					break;
				}
			}
		}
		// Отсечем только те, которые видимы при выборе
		auto pp = drawables.data;
		for(auto p : drawables) {
			if(p->isvisibleactive()) {
				if(p != result)
					continue;
			}
			*pp++ = p;
		}
		drawables.count = pp - drawables.data;
		// Выведем на экран все что надо
		point origin;
		origin.x = screen.x1 - rc.x1;
		origin.y = screen.y1 - rc.y1;
		for(auto p : drawables)
			p->painting(origin);
	}
#ifdef _DEBUG
	char temp[1024]; temp[0] = 0;
	stringcreator sc;
	stringbuilder sb(sc, temp);
	auto map_index = map::getindex(hotspot);
	sb.add("Mouse (%3i, %4i), Map (%1i, %2i), Search (%5i, %6i), index=%7i, ",
		hotspot.x, hotspot.y,
		hot.mouse.x, hot.mouse.y,
		map::getx(map_index), map::gety(map_index), map_index);
	sb.add("Drawables=%1i", drawables.count);
	if(result) {
		if(region_data.indexof(static_cast<region*>(result)) != -1) {
			auto p = static_cast<region*>(result);
			if(p->type == RegionTravel)
				sb.addn("Region lead to %1, %2", p->move_to_area, p->move_to_entrance);
			else if(p->type == RegionTriger)
				sb.addn("Trigger");
			else
				sb.addn("Region info in %1i, %2i", p->launch.x, p->launch.y);
		} else
			sb.addn("Object (%1i, %2i)", result->getposition().x, result->getposition().y);
	}
	textf(rc.x1, rc.y1, rc.width(), temp);
#endif
	return result;
}

static void render_shifter(animation& e, cursorset& cur) {
	if(e.rsname[0]) {
		cur.set(res::NONE);
		animation::info i; e.getinfo(i);
		draw::image(e.x, e.y, i.source, i.frame, i.flags, i.transparent);
	}
}

static void create_shifer(const rect& screen, animation& d, point& camera) {
	int index;
	const int cmx = screen.x2 - 1;
	const int cmy = screen.y2 - 1;
	d.x = hot.mouse.x;
	d.y = hot.mouse.y;
	if(d.x <= screen.x1)
		d.x = screen.x1;
	else if(d.x >= cmx)
		d.x = cmx;
	if(d.y <= screen.y1)
		d.y = screen.y1;
	else if(d.y >= cmy)
		d.y = cmy;
	if(d.x <= screen.x1)
		index = (d.y <= screen.y1) ? 3 : (d.y < cmy) ? 4 : 5;
	else if(d.x >= cmx)
		index = (d.y <= screen.y1) ? 7 : (d.y < cmy) ? 0 : 1;
	else
		index = (d.y <= screen.x1) ? 2 : (d.y < cmy) ? -1 : 6;
	switch(index) {
	case 0: camera.x += camera_step; break;
	case 2: camera.y -= camera_step; break;
	case 4: camera.x -= camera_step; break;
	case 6: camera.y += camera_step; break;
	}
	if(index != -1) {
		zcpy(d.rsname, "CURSARW");
		d.circle = index;
	} else
		d.rsname[0] = 0;
}

static unsigned getblendtextduration() {
	return 8000;
}

static void render_panel(rect& rcs, bool show_actions = true, itemdrag* pd = 0) {
	static action_s actions[] = {ActionGuard, ActionAttack};
	static formation_s formations[] = {Formation3by2, FormationT, FormationGather, Formation4and2, FormationProtect};
	auto x = rcs.x1;
	auto y = rcs.y2 - 60;
	draw::image(x, y, res::GACTN, show_actions ? 0 : 1);
	if(show_actions) {
		auto x1 = x + 6, y1 = y + 12;
		for(auto e : actions)
			x1 += act(x1, y1, cmpr(choose_action, e), e, current_action);
		for(auto e : formations)
			x1 += act(x1, y1, cmpr(choose_formation, e), e);
	}
	auto x1 = 506, y1 = y + 4;
	for(auto& e : players)
		x1 += act(x1, y1, cmpr(choose_player, (int)&e), e, pd);
	rcs.y2 = y;
}

static void render_footer(rect& rcs) {
	auto i = draw::getframe(12) % 32;
	draw::image(0, 493, res::GCOMM, 0);
	button(576, 496, cmpr(select_all), 0, res::GCOMMBTN, 0, 0, 1, 0, 0, 0, 0, 0);
	button(575, 565, cmpr(nothing), 0, res::GCOMMBTN, 0, 16, 17, 0, 0, 0, 0, 0);
	button(600, 515, cmpr(character_sheet), 0, res::GCOMMBTN, 0, 4, 5, 0, 0, 0, 0, 0);
	button(630, 510, cmpr(character_invertory), 0, res::GCOMMBTN, 0, 6, 7, 0, 0, 0, 0, 0);
	button(668, 514, cmpr(character_spellbook), 0, res::GCOMMBTN, 0, 8, 9, 0, 0, 0, 0, 0);
	button(600, 550, cmpr(game_minimap), 0, res::GCOMMBTN, 0, 14, 15, 0, 0, 0, 0, 0);
	button(628, 553, cmpr(game_journal), 0, res::GCOMMBTN, 0, 12, 13, 0, 0, 0, 0, 0);
	button(670, 550, cmpr(game_option), 0, res::GCOMMBTN, 0, 10, 11, 0, 0, 0, 0, 0);
	button(703, 495, cmpr(nothing), 0, res::GCOMMBTN, 0, 2, 3, 0, 0, 0, 0, 0);
	button(757, 494, cmpr(nothing), 0, res::GCOMMBTN, 19, 18, 18, 0, 0, 0, 0, 0);
	button(736, 536, cmpr(nothing), 0, res::CGEAR, i, i, i, i, 0, 0, 0, true);
	mspaint({12, 500, 542, 592}, {554, 497, 566, 592});
	rcs.y2 -= 107;
}

void draw::menumodal(bool use_keys, itemdrag* pd) {
	rect rcs = {0, 0, getwidth(), getheight()};
	render_footer(rcs);
	render_panel(rcs, false);
	correct_camera(rcs, camera);
	domodal();
	if(use_keys)
		translate(menu_keys);
}

static void party_move_to(point destination) {
	if(!current_selected)
		return;
	auto start = current_selected[0]->getposition();
	auto index = 0;
	for(auto p : current_selected)
		p->move(map::getfree(p->getposition(start, destination, settings.formation, index++), p->getsize()));
}

void creature::adventure() {
	cursorset cur;
	animation shifter;
	if(!getplayer())
		setplayer(players);
	while(ismodal()) {
		rect rcs = {0, 0, getwidth(), getheight()};
		cur.set(res::CURSORS);
		create_shifer(rcs, shifter, camera);
		if(settings.panel == setting::PanelFull)
			render_footer(rcs);
		if(settings.panel == setting::PanelFull || settings.panel == setting::PanelActions)
			render_panel(rcs);
		correct_camera(rcs, camera);
		void(*proc_point)(point pt) = 0;
		point origin = camera; origin.x -= rcs.width() / 2; origin.y -= rcs.height() / 2;
		point hotspot = origin; hotspot.x += hot.mouse.x - rcs.x1; hotspot.y += hot.mouse.y - rcs.y1;
		auto current = render_area(rcs, origin, hotspot);
		if(current) {
			if(container_data.consist(current))
				cur.set(res::CURSORS, 2, true);
			else
				cur.set(res::CURSORS, current->getcursor());
		} else if(hot.mouse.in(rcs)) {
			auto index = map::getindex(hotspot);
			if(map::isblock(index))
				cur.set(res::CURSORS, 6);
			else {
				proc_point = party_move_to;
				cur.set(res::CURSORS, 4);
			}
		}
		render_shifter(shifter, cur);
		domodal();
		translate(movement_keys);
		translate(menu_keys);
		switch(hot.key) {
		case MouseLeft:
			if(current) {
				if(region_data.consist(current)) {
					auto p = static_cast<region*>(current);
					switch(p->type) {
					case RegionInfo:
						if(!hot.pressed)
							textblend(p->launch, p->name, getblendtextduration());
						break;
					case RegionTravel:
						if(!hot.pressed) {
							auto destination = p->getposition();
							if(getpartymaxdistance(destination) < 250)
								moveto(p->move_to_area, p->move_to_entrance);
							else
								party_move_to(destination);
						}
						break;
					}
				} else if(container_data.consist(current)) {
					auto p = static_cast<container*>(current);
				} else if(door_data.consist(current)) {
					auto p = static_cast<door*>(current);
					if(!hot.pressed)
						p->toggle();
				}
			} else if(proc_point)
				proc_point(hotspot);
			break;
		}
		updategame();
	}
}

void actor::setcamera(point value) {
	camera = value;
	correct_camera({0, 0, camera_size.x, camera_size.y}, camera);
}

point actor::getcamera() {
	return camera;
}

point actor::getcamerasize() {
	return camera_size;
}