#include "main.h"

using namespace draw;

typedef adat<drawable*, 256>	drawablet;
static point					camera;
static point					camera_size = {800, 600};
static action_s					current_action = ActionGuard;
static adat<creature*, 8>		current_selected;
static void						(creature::*creature_proc)();
const int						camera_step = 16;

void update_timestamp();

static void character_submenu() {
	cursorset cur;
	while(ismodal()) {
		auto player = creature::getactive();
		if(!player)
			break;
		(player->*creature_proc)();
		menumodal();
	}
}

static void adventure_step() {
	creature_proc = 0;
	setpage();
}

static void choose_menu(void (creature::*proc)()) {
	if(creature_proc == proc)
		adventure_step();
	else {
		creature_proc = proc;
		setpage(character_submenu);
	}
}

static void choose_menu(void(*proc)()) {
	if(isnext(proc))
		adventure_step();
	else
		setpage(proc);
}

void creature::setactive() {
	current_selected.clear();
	current_selected.add(this);
}

static void nothing() {}
static void choose_action() { current_action = (action_s)hot.param; }
static void choose_formation() { settings.formation = (formation_s)hot.param; }
static void choose_player() { ((creature*)hot.param)->setactive(); }
static void move_left() { camera.x -= camera_step; }
static void move_right() { camera.x += camera_step; }
static void move_up() { camera.y -= camera_step; }
static void move_down() { camera.y += camera_step; }
static void character_invertory() { choose_menu(&creature::invertory); }
static void character_sheet() { choose_menu(&creature::sheet); }
static void character_spellbook() { choose_menu(&creature::spellbook); }
static void character_test() { actor::slide({1000, 1000}); }
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
{character_test, Alpha + 'T', "Тестирование анимации"},
{game_minimap, Alpha + 'M', "Карта местности"},
{game_option, Alpha + 'O', "Опции"},
{game_journal, Alpha + 'J', "Журнал заданий"},
{adventure_step, KeyEscape, "Вернуться в режим приключений"},
{}};

static void hitpoints(int x, int y, int width, int height, int hp, int mhp) {
	if(!mhp || hp < 0)
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

static int act(int x, int y, const runable& cmd, item& it) {
	button_states state;
	button(x, y, cmd, 0, res::GUIBTBUT, 2, 1, 2, 1, 0, 0, &state);
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

static int act(int x, int y, const runable& cmd, creature& player, itemdrag* pd, bool change_player = true) {
	color s0 = colors::green;
	unsigned flags = 0;
	if(current_selected.is(&player))
		flags |= Checked;
	int hp = player.gethits();
	int mhp = player.gethitsmax();
	if(hp <= 0)
		flags |= Disabled;
	rect rc = {x, y, x + 44, y + 44};
	image(x, y, res::GUIRSPOR, (flags&Checked) ? 1 : 0, flags);
	image(x + 2, y + 2, res::PORTS, player.getportrait());
	if(change_player) {
		if(hot.key == MouseLeft && hot.pressed && area(rc) == AreaHilitedPressed)
			cmd.execute();
	}
	if(player && (flags&Disabled))
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

static void render_movement(const rect& rc, int mx, int my) {
	auto x2 = rc.x1 + imin(mx + rc.width(), map::width * 16);
	auto y2 = rc.y1 + imin(my + rc.height(), map::height * 12);
	auto y1 = rc.y1 + my;
	while(y1 < y2) {
		int x1 = rc.x1 + mx;
		while(x1 < x2) {
			short px = x1 - rc.x1;
			short py = y1 - rc.y1;
			auto index = map::getindex({px, py});
			auto value = map::getcost(index);
			if(value && value != Blocked) {
				auto x = x1 - mx + rc.x1;
				auto y = y1 - my + rc.y1;
				rectf({x + 1, y + 1, x + 15, y + 11}, colors::black, 0x40);
			}
			x1 += 16;
		}
		y1 += 12;
	}
}

static void correct_camera(const rect& rc, point& camera) {
	camera_size.x = rc.width();
	camera_size.y = rc.height();
	if(camera.x < 0)
		camera.x = 0;
	if(camera.y < 0)
		camera.y = 0;
	auto mx = map::width * 16 - rc.width();
	auto my = map::height * 12 - rc.height();
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
	point s1 = e1->getposition(); s1.y += e1->getzorder();
	point s2 = e2->getposition(); s2.y += e2->getzorder();
	if(s1.y != s2.y)
		return s1.y - s2.y;
	if(s1.x != s2.x)
		return s1.x - s2.x;
	return 0;
}

creature* creature::getactive() {
	if(!current_selected)
		return 0;
	return current_selected[0];
}

bool creature::isselected() const {
	return current_selected.is((creature* const)this);
}

static target render_area(rect rc, const point origin) {
	draw::state push;
	draw::setclip(rc);
	// Получим экран
	rect screen;
	screen.x1 = origin.x;
	screen.y1 = origin.y;
	screen.x2 = screen.x1 + rc.width();
	screen.y2 = screen.y1 + rc.height();
	// Получим где мышка
	point hotspot;
	hotspot.x = hot.mouse.x - rc.x1 + origin.x;
	hotspot.y = hot.mouse.y - rc.y1 + origin.y;
	// Нарисуем тайлы
	render_tiles(rc, screen.x1, screen.y1);
	if(settings.show_search)
		render_search(rc, screen.x1, screen.y1);
	//if(show_movement)
	//	render_movement(rc, screen.x1, screen.y1);
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
	for(auto& e : itemground_data) {
		if(e)
			add(screen, drawables, e);
	}
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
	if(result)
		return result;
	if(hotspot.in(screen))
		return hotspot;
	return target();
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

static targetreaction render_area(rect rc, const point origin, cursorset& cur) {
	auto combat_mode = creature::iscombatmode();
	auto player = creature::getactive();
	targetreaction react = render_area(rc, origin);
	switch(react.type) {
	case Container:
		cur.set(res::CURSORS, 2, true);
		break;
	case ItemGround:
		cur.set(res::CURSORS, 2);
		break;
	case Door:
		cur.set(res::CURSORS, react.door->getcursor());
		react.method = &creature::toggle;
		break;
	case Creature:
		if(player->isenemy(*react.creature)) {
			react.method = &creature::attack;
			cur.set(res::CURSORS, 12);
		} else if(react.creature->isplayer()) {
			if(!combat_mode)
				react.method = &creature::setactive;
		} else {
			react.method = &creature::talk;
			cur.set(res::CURSORS, react.creature->getcursor());
		}
		break;
	case Region:
		cur.set(res::CURSORS, react.region->getcursor());
		break;
	case Position:
		if(map::isblock(map::getindex(react.position, 1))) {
			cur.set(res::CURSORS, 6);
			react.clear();
		} else
			cur.set(res::CURSORS, 4);
		break;
	}
	return react;
}

static void render_panel(rect& rcs, bool show_actions = true, itemdrag* pd = 0, bool show_players = true, bool show_background = true, bool change_players = true) {
	static action_s actions[] = {ActionGuard, ActionAttack};
	static formation_s formations[] = {Formation3by2, FormationT, FormationGather, Formation4and2, FormationProtect};
	auto x = rcs.x1;
	auto y = rcs.y2 - 60;
	if(show_background)
		draw::image(x, y, res::GACTN, show_actions ? 0 : 1);
	if(show_actions) {
		auto x1 = x + 6, y1 = y + 12;
		if(current_selected.getcount() > 1) {
			for(auto e : actions)
				x1 += act(x1, y1, cmpr(choose_action, e), e, current_action);
			for(auto e : formations)
				x1 += act(x1, y1, cmpr(choose_formation, e), e);
		} else {
			x1 += act(x1, y1, cmpr(choose_action, ActionGuard), ActionGuard, ActionAttack);
		}
	}
	auto x1 = 506, y1 = y + 4;
	if(show_players) {
		for(auto& e : players)
			x1 += act(x1, y1, cmpr(choose_player, (int)&e), e, pd, change_players);
	}
	rcs.y2 = y;
}

static void render_footer(rect& rcs, bool show_buttons = true) {
	auto i = draw::getframe(12) % 32;
	draw::image(0, 493, res::GCOMM, 0);
	if(show_buttons) {
		button(576, 496, cmpr(select_all), 0, res::GCOMMBTN, 0, 0, 1, 0, 0, 0, 0, 0);
		button(575, 565, cmpr(nothing), 0, res::GCOMMBTN, 0, 16, 17, 0, 0, 0, 0, 0); // Отдых
		button(600, 515, cmpr(character_sheet), 0, res::GCOMMBTN, 0, 4, 5, 0, 0, 0, 0, 0);
		button(630, 510, cmpr(character_invertory), 0, res::GCOMMBTN, 0, 6, 7, 0, 0, 0, 0, 0);
		button(668, 514, cmpr(character_spellbook), 0, res::GCOMMBTN, 0, 8, 9, 0, 0, 0, 0, 0);
		button(600, 550, cmpr(game_minimap), 0, res::GCOMMBTN, 0, 14, 15, 0, 0, 0, 0, 0);
		button(628, 553, cmpr(game_journal), 0, res::GCOMMBTN, 0, 12, 13, 0, 0, 0, 0, 0);
		button(670, 550, cmpr(game_option), 0, res::GCOMMBTN, 0, 10, 11, 0, 0, 0, 0, 0);
		button(703, 495, cmpr(nothing), 0, res::GCOMMBTN, 0, 2, 3, 0, 0, 0, 0, 0);
		button(736, 536, cmpr(nothing), 0, res::CGEAR, i, i, i, i, 0, 0, 0, true);
	} else
		image(736, 536, gres(res::CGEAR), i, 0);
	draw::image(757, 494, res::GCOMMBTN, creature::iscombatmode() ? 19 : 18, 0);
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

static void combat_movement(point destination) {
	auto player = creature::getactive();
	if(!player)
		return;
	player->move(destination, map::getrange(player->getmovement()) + 1, player->getsize(), true);
}

static void party_interact(point destination) {
	if(creature::iscombatmode())
		combat_movement(destination);
	else {
		if(!current_selected)
			return;
		auto start = current_selected[0]->getposition();
		auto index = 0;
		for(auto p : current_selected)
			p->move(map::getfree(p->getposition(start, destination, settings.formation, index++), p->getsize()));
	}
}

void actor::animate() {
	cursorset cur(res::NONE);
	auto current_action = action;
	while(ismodal()) {
		setcamera(position);
		if(current_action != action)
			break;
		rect rcs = {0, 0, getwidth(), getheight()};
		if(settings.panel == setting::PanelFull)
			render_footer(rcs, false);
		if(settings.panel == setting::PanelFull || settings.panel == setting::PanelActions)
			render_panel(rcs, false, 0, false, false);
		correct_camera(rcs, camera);
		render_area(rcs, camera);
		domodal();
	}
}

void actor::animate(actor& opponent, animate_s id) {
	cursorset cur(res::NONE);
	auto current_action = action;
	auto maximum_frame = getciclecount(getcicle());
	while(ismodal()) {
		if(frame >= maximum_frame / 2)
			opponent.set(id);
		setcamera(position);
		if(current_action != action)
			break;
		rect rcs = {0, 0, getwidth(), getheight()};
		if(settings.panel == setting::PanelFull)
			render_footer(rcs, false);
		if(settings.panel == setting::PanelFull || settings.panel == setting::PanelActions)
			render_panel(rcs, false, 0, false, false);
		correct_camera(rcs, camera);
		render_area(rcs, camera);
		domodal();
	}
}

target creature::choose_target(int cursor, short unsigned start, short unsigned max_cost) {
	target tg;
	cursorset cur;
	animation shifter;
	map::blockimpassable(0);
	map::createwave(start, 1, max_cost);
	while(ismodal()) {
		cur.set(res::CURSORS, cursor);
		rect rcs = {0, 0, getwidth(), getheight()};
		create_shifer(rcs, shifter, camera);
		if(settings.panel == setting::PanelFull)
			render_footer(rcs, false);
		if(settings.panel == setting::PanelFull || settings.panel == setting::PanelActions)
			render_panel(rcs, true, 0, true, true, false);
		correct_camera(rcs, camera);
		tg = render_area(rcs, camera);
		switch(tg.type) {
		case Position:
			if(map::getcost(map::getindex(tg.position)) == Blocked)
				tg.clear();
			break;
		}
		if(!tg)
			cur.set(res::CURSORS, 6);
		render_shifter(shifter, cur);
		domodal();
		translate(movement_keys);
		switch(hot.key) {
		case MouseLeft:
			if(!hot.pressed && tg)
				breakmodal(1);
			break;
		case KeyEscape:
			breakmodal(0);
			break;
		}
		updategame();
	}
	if(!getresult())
		tg.clear();
	return tg;
}

static void combat_mode_proc() {
	creature::adventure();
}

bool creature::iscombatmode() {
	return getlayout() == combat_mode_proc;
}

void creature::choose_action() {
	setactive();
	draw::setlayout(combat_mode_proc);
}

static void checkcombat(unsigned& counter) {
	if(draw::getframe() < counter)
		return;
	counter += 1000;
	for(auto& e : players) {
		if(!e.isready())
			continue;
		for(auto& m : creature_data) {
			if(!m.isready())
				continue;
			if(m.isenemy(e)) {
				creature::makecombat();
				return;
			}
		}
	}
}

static void render_container(rect& rcs) {
	int x = 0, y = 476;
	image(x, y, res::GUICONT, 1, 0);
	rectb({x + 375, y + 24, x + 387, y + 100}, colors::white);
	button(x + 150, y + 22, cmpr(buttonparam, 1), 0, res::STONSLOT, 0, 0, 0, 0, 0, 0, 0);
	button(x + 195, y + 22, cmpr(buttonparam, 2), 0, res::STONSLOT, 0, 0, 0, 0, 0, 0, 0);
	button(x + 239, y + 22, cmpr(buttonparam, 3), 0, res::STONSLOT, 0, 0, 0, 0, 0, 0, 0);
	button(x + 283, y + 22, cmpr(buttonparam, 4), 0, res::STONSLOT, 0, 0, 0, 0, 0, 0, 0);
	button(x + 327, y + 22, cmpr(buttonparam, 5), 0, res::STONSLOT, 0, 0, 0, 0, 0, 0, 0);
	button(x + 150, y + 65, cmpr(buttonparam, 6), 0, res::STONSLOT, 0, 0, 0, 0, 0, 0, 0);
	rectb({x + 602, y + 24, x + 614, y + 100}, colors::white);
	button(x + 509, y + 22, cmpr(buttonparam, 11), 0, res::STONSLOT, 0, 0, 0, 0, 0, 0, 0);
	button(x + 553, y + 22, cmpr(buttonparam, 12), 0, res::STONSLOT, 0, 0, 0, 0, 0, 0, 0);
	button(x + 553, y + 65, cmpr(buttonparam, 14), 0, res::STONSLOT, 0, 0, 0, 0, 0, 0, 0);
	//button(x + 59, y + 25, cmpr(buttonparam, 51), 0, res::GUICTRL, 0, 0, 1, 0, 0, 0, 0);
	button(x + 509, y + 65, cmpr(buttonparam, 13), 0, res::STONSLOT, 0, 0, 0, 0, 0, 0, 0);
	button(x + 684, y + 28, cmpr(buttonparam, 52), Disabled, res::GBTNOPT1, 0, 1, 2, 3, 0, 0, 0);
	label(x + 661, y + 78, 70, 20, ""); // NORMAL
	//button(x + 430, y + 28, cmpr(buttonparam, 55), 0, res::CONTBACK, 0, 0, 1, 0, 0, 0, 0);
	button(x + 195, y + 65, cmpr(buttonparam, 7), 0, res::STONSLOT, 0, 0, 1, 0, 0, 0, 0);
	button(x + 239, y + 65, cmpr(buttonparam, 8), 0, res::STONSLOT, 0, 0, 1, 0, 0, 0, 0);
	button(x + 283, y + 65, cmpr(buttonparam, 9), 0, res::STONSLOT, 0, 0, 1, 0, 0, 0, 0);
	button(x + 327, y + 65, cmpr(buttonparam, 10), 0, res::STONSLOT, 0, 0, 1, 0, 0, 0, 0);
	rcs.y2 -= 107;
	//rcs.y2 -= 60;
}

static void player_interact(creature* player, const targetreaction& tg) {
	if(creature::iscombatmode())
		player->interact(tg, map::getrange(player->getmovement()) + 1, true);
	else
		player->interact(tg, 0, false);
}

static void translate_mouse(const targetreaction& tg) {
	auto combat_mode = creature::iscombatmode();
	auto player = creature::getactive();
	switch(hot.key) {
	case MouseLeft:
		switch(tg.type) {
		case Region:
			switch(tg.region->type) {
			case RegionInfo:
				if(!hot.pressed) {
					mslog(tg.region->name);
					textblend(tg.region->launch, tg.region->name, 8000);
				}
				break;
			case RegionTravel:
				if(!hot.pressed) {
					auto destination = tg.region->getposition();
					if(creature::getpartymaxdistance(destination) < 250) {
						if(!combat_mode)
							creature::moveto(tg.region->move_to_area, tg.region->move_to_entrance);
					} else
						party_interact(destination);
				}
				break;
			}
			break;
		case Container:
		case ItemGround:
		case Door:
		case Creature:
			if(!hot.pressed)
				player_interact(player, tg);
			break;
		case Position:
			if(!hot.pressed)
				party_interact(tg.position);
			break;
		}
		break;
	}
}

void creature::adventure() {
	cursorset cur;
	animation shifter;
	if(!getactive())
		players[0].setactive();
	unsigned counter = draw::getframe();
	bool combat_mode = creature::iscombatmode();
	while(ismodal()) {
		void(creature::*proc_creature)(const target& opponent) = 0;
		cur.set(res::CURSORS);
		auto player = creature::getactive();
		rect rcs = {0, 0, getwidth(), getheight()};
		create_shifer(rcs, shifter, camera);
		if(settings.panel == setting::PanelFull)
			render_footer(rcs, true);
		if(settings.panel == setting::PanelFull || settings.panel == setting::PanelActions)
			render_panel(rcs, true, 0, true, true, !combat_mode);
		correct_camera(rcs, camera);
		auto tg = render_area(rcs, camera, cur);
		render_shifter(shifter, cur);
		domodal();
		translate(movement_keys);
		translate(menu_keys);
		translate_mouse(tg);
		updategame();
		if(!combat_mode)
			checkcombat(counter);
	}
}

void actor::setcamera(point value) {
	camera.x = value.x - camera_size.x / 2;
	camera.y = value.y - camera_size.y / 2;
	correct_camera({0, 0, camera_size.x, camera_size.y}, camera);
}

point actor::getcamera() {
	return camera;
}

point actor::getcamerasize() {
	return camera_size;
}

void actor::slide(const point position) {
	cursorset cur(res::NONE);
	rect rc;
	rc.x1 = camera.x;
	rc.y1 = camera.y;
	rc.x2 = rc.x1 + camera_size.x;
	rc.y2 = rc.y1 + camera_size.y;
	if(position.in(rc))
		return;
	point start, destination;
	start = camera;
	destination.x = position.x - camera_size.x / 2;
	destination.y = position.y - camera_size.y / 2;
	correct_camera({0, 0, camera_size.x, camera_size.y}, destination);
	auto range = 0;
	auto step = 4;
	auto range_maximum = distance(start, destination);
	auto dx = destination.x - start.x;
	auto dy = destination.y - start.y;
	while(ismodal() && range < range_maximum) {
		update_timestamp();
		camera.x = start.x + dx * range / range_maximum;
		camera.y = start.y + dy * range / range_maximum;
		rect rcs = {0, 0, getwidth(), getheight()};
		if(settings.panel == setting::PanelFull)
			render_footer(rcs, true);
		if(settings.panel == setting::PanelFull || settings.panel == setting::PanelActions)
			render_panel(rcs, true, 0, true, true, false);
		correct_camera(rcs, camera);
		render_area(rcs, camera);
		sysredraw();
		range += step;
	}
}