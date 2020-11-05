#include "main.h"

using namespace draw;

typedef adat<drawable*, 256>	drawablet;
static variant					current_target;
static point					camera;
static point					camera_size = {800, 600};
static action_s					current_action = ActionGuard;
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

void choose_menu(void(*proc)()) {
	if(isnext(proc))
		adventure_step();
	else
		setpage(proc);
}

void creature::setactive() {
	game.selected.clear();
	game.selected.add(this);
}

static void nothing() {}
static void choose_action() { current_action = (action_s)hot.param; }
static void choose_formation() { game.formation = (formation_s)hot.param; }
static void choose_player() { ((creature*)hot.param)->setactive(); }

static void move_left() {
	point pt = camera;
	pt.x -= camera_step;
	actor::setcamera(pt);
}

static void move_right() {
	point pt = camera;
	pt.x += camera_step;
	actor::setcamera(pt);
}

static void move_up() {
	point pt = camera;
	pt.y -= camera_step;
	actor::setcamera(pt);
}

static void move_down() {
	point pt = camera;
	pt.y += camera_step;
	actor::setcamera(pt);
}

static void character_invertory() { choose_menu(&creature::invertory); }
static void character_sheet() { choose_menu(&creature::sheet); }
static void character_spellbook() { choose_menu(&creature::spellbook); }
static void game_option() { choose_menu(creature::options); }
static void game_minimap() { choose_menu(creature::minimap); }
static void game_journal() { choose_menu(creature::journal); }
static void game_zoom() { game.zoom = !game.zoom; }
static void layer_search() { game.show_search = !game.show_search; }
static void layer_path() { game.show_path = !game.show_path; }

static void change_mode() {
	game.panel = (setting::mode_s)(game.panel + 1);
	if(game.panel > setting::NoPanel)
		game.panel = setting::PanelFull;
}

void creature::select_all() {
	for(auto p : game.party) {
		if(!game.selected.is(p))
			game.selected.add(p);
	}
}

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

static bool act(int& x, int y, action_s id, action_s id_selected) {
	unsigned flags = 0;
	if(id == id_selected)
		flags |= Checked;
	auto i = id * 4;
	auto r = button(x, y, flags, res::GUIBTACT, i + 3, i, i + 1, i + 2, 0, 0, 0, 0);
	x += 41;
	return r;
}

static bool act(int& x, int y, item& it) {
	auto player = it.getowner();
	if(!player)
		return false;
	if(!it)
		return false;
	button_s state;
	auto r = button(x, y, 0, res::GUIBTBUT, 2, 0, 1, 1, 0, 0, &state);
	auto x1 = x + 2; auto y1 = y;
	if(state == ButtonPressed) {
		x1 += 1;
		y1 += 1;
	}
	player->icon(x1, y1, &it, QuickItem, 0, 0, false);
	x += 41;
	return r;
}

static bool act(int& x, int y, creature* player, slot_s s1, slot_s s2) {
	auto pi = player->get(s1);
	if(!pi || !(*pi))
		return false;
	button_s state;
	auto flags = 0;
	auto active_slot = slot_s(player->getquick() * 2 + QuickWeapon);
	if(active_slot == s1)
		flags |= Checked;
	auto r = button(x, y, flags, res::GUIBTBUT, 2, 0, 1, 0, 0, 0, &state);
	auto x1 = x + 2; auto y1 = y;
	if(state == ButtonPressed) {
		x1 += 1;
		y1 += 1;
	}
	player->icon(x1, y1, player->get(s2), s2, 0, 0, false);
	player->icon(x1, y1, player->get(s1), s1, 0, 0, false);
	x += 41;
	return r;
}

static bool act(int& x, int y, formation_s id) {
	unsigned flags = 0;
	if(id == game.formation)
		flags |= Checked;
	auto i = id * 4;
	button_s state;
	auto r = button(x, y, id, flags, res::GUIBTBUT, 0, 0, &state);
	auto x0 = x;
	if(state == ButtonPressed) {
		x0 += 2;
		y += 2;
	}
	draw::image(x0 + 3, y + 2, res::FORM, id);
	x += 41;
	return r;
}

static bool act(int& x, int y, creature& player, itemdrag* pd, bool change_player = true) {
	color s0 = colors::green;
	unsigned flags = 0;
	auto result = false;
	if(game.selected.is(&player))
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
			result = true;
	}
	if(player && (flags&Disabled))
		draw::rectf({x + 2, y + 2, x + 44, y + 44}, colors::red, 128);
	hitpoints(x, y + 48, 49 - 3, 4, hp, mhp);
	x += 49;
	return result;
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
			auto py = dy + (y1 - rc.y1) / 64;
			auto px = dx + (x1 - rc.x1) / 64;
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

static void render_path(const rect& rc, int mx, int my, unsigned short cost_cap) {
	auto x2 = rc.x1 + imin(mx + rc.width(), map::width * 16);
	auto y2 = rc.y1 + imin(my + rc.height(), map::height * 12);
	auto y1 = rc.y1 + my;
	while(y1 < y2) {
		auto x1 = rc.x1 + mx;
		while(x1 < x2) {
			short px = x1 - rc.x1;
			short py = y1 - rc.y1;
			auto index = map::getindex({px, py});
			auto cost = map::getcost(index);
			if(cost != Blocked && cost) {
				short sx = x1 - mx;
				short sy = y1 - my;
				if(cost <= cost_cap)
					rectf({sx + 2, sy + 2, sx + 14, sy + 10}, colors::black, 16);
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
	if(!game.selected)
		return 0;
	return game.selected[0];
}

bool creature::isselected() const {
	return game.selected.is((creature* const)this);
}

static void correct_camera() {
	if(camera.x < camera_size.x / 2)
		camera.x = camera_size.x / 2;
	if(camera.y < camera_size.y / 2)
		camera.y = camera_size.y / 2;
	auto mx = map::width * 16 - camera_size.x / 2;
	auto my = map::height * 12 - camera_size.y / 2;
	if(camera.x >= mx)
		camera.x = mx;
	if(camera.y >= my)
		camera.y = my;
}

static variant render_area_noscale(const rect& rc, point& hotspot, const point origin, point mouse, short unsigned cap_movement) {
	draw::state push;
	draw::setclip(rc);
	// Получим экран
	rect screen;
	screen.x1 = origin.x - getwidth() / 2;
	if(screen.x1 < 0)
		screen.x1 = 0;
	screen.y1 = origin.y - getheight() / 2;
	if(screen.y1 < 0)
		screen.y1 = 0;
	screen.x2 = screen.x1 + rc.width();
	screen.y2 = screen.y1 + rc.height();
	// Получим где мышка
	hotspot.x = mouse.x - rc.x1 + screen.x1;
	hotspot.y = mouse.y - rc.y1 + screen.y1;
	// Нарисуем тайлы
	render_tiles(rc, screen.x1, screen.y1);
	if(game.show_search)
		render_search(rc, screen.x1, screen.y1);
	if(game.show_movement)
		render_movement(rc, screen.x1, screen.y1);
	if(cap_movement)
		render_path(rc, screen.x1, screen.y1, cap_movement);
	// Нарисуем маркеры движения
	for(auto p : game.party) {
		if(p) {
			p->render_marker(rc, screen.x1, screen.y1);
			if(game.show_path)
				p->render_path(rc, screen.x1, screen.y1);
		}
	}
	// Получим объекты
	drawablet drawables;
	for(auto& e : bsdata<door>())
		add(screen, drawables, e);
	for(auto& e : bsdata<region>())
		add(screen, drawables, e);
	for(auto& e : bsdata<container>())
		add(screen, drawables, e);
	for(auto& e : bsdata<creature>())
		add(screen, drawables, e);
	for(auto& e : bsdata<animation>())
		add(screen, drawables, e);
	for(auto& e : bsdata<moveable>())
		add(screen, drawables, e);
	for(auto& e : bsdata<itemground>()) {
		if(e)
			add(screen, drawables, e);
	}
	for(auto& e : bsdata<floattext>()) {
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
	if(result)
		return result;
	if(hotspot.in(screen))
		return hotspot;
	return variant();
}

static variant render_area_scale(rect rc, point& hotspot, const point origin, point mouse, bool show_path) {
	rect rc1 = {0, 0, rc.width() / 2, rc.height() / 2};
	draw::surface copy(rc1.width(), rc1.height(), 32);
	point mouse1;
	mouse1.x = (mouse.x - rc.x1) / 2;
	mouse1.y = (mouse.y - rc.y1) / 2;
	auto push_canvas = draw::canvas;
	draw::canvas = &copy;
	auto result = render_area_noscale(rc1, hotspot, origin, mouse1, show_path);
	draw::canvas = push_canvas;
	scale2x(draw::canvas->ptr(rc.x1, rc.y1), draw::canvas->scanline,
		copy.ptr(0, 0), copy.scanline, copy.width, copy.height);
	return result;
}

static void debug_info(point hotspot, const variant& result) {
#ifdef _DEBUG
	char temp[1024]; stringbuilder sb(temp);
	auto map_index = map::getindex(hotspot);
	sb.add("Mouse (%3i, %4i), Map (%1i, %2i), Search (%5i, %6i), index=%7i",
		hotspot.x, hotspot.y,
		hot.mouse.x, hot.mouse.y,
		map::getx(map_index), map::gety(map_index), map_index);
	if(result) {
		if(result.getregion()) {
			auto p = result.getregion();
			if(p->type == RegionTravel)
				sb.addn("Region lead to %1, %2", p->move_to_area, p->move_to_entrance);
			else if(p->type == RegionTriger)
				sb.addn("Trigger %1", p->name);
			else
				sb.addn("Region info in %1i, %2i", p->launch.x, p->launch.y);
		} else if(result.type != Position)
			sb.addn("Object %1", getstr(result));
	}
	textf(0, 0, draw::getwidth(), temp);
#endif
}

static variant render_area(const rect& rc, point mouse, short unsigned cap_movement) {
	point hotspot; variant result;
	camera_size.x = rc.width(); camera_size.y = rc.height();
	correct_camera();
	if(game.zoom)
		result = render_area_scale(rc, hotspot, camera, mouse, cap_movement);
	else
		result = render_area_noscale(rc, hotspot, camera, mouse, cap_movement);
	debug_info(hotspot, result);
	return result;
}

static void render_shifter(animation& e, cursorset& cur) {
	if(e.rsname[0]) {
		cur.set(res::NONE);
		animation::info i; e.getinfo(i);
		draw::image(e.x, e.y, i.source, i.frame, i.flags, i.transparent);
	}
}

static void create_shifer(animation& d, point& camera) {
	rect screen = {0, 0, getwidth(), getheight()};
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

static targetreaction apply_cursor(cursorset& cur, const variant& result) {
	targetreaction react = result;
	auto combat_mode = creature::iscombatmode();
	auto player = creature::getactive();
	if(!player)
		return react;
	switch(react.target.type) {
	case Container:
	case ItemGround:
		cur.set(res::CURSORS, 2, true);
		react.method = &creature::getin;
		break;
	case Door:
		cur.set(res::CURSORS, react.target.getdoor()->getcursor());
		react.method = &creature::toggle;
		break;
	case Creature:
		if(player->isenemy(*react.target.getcreature())) {
			cur.set(res::CURSORS, 12);
			react.method = &creature::attack;
		} else if(react.target.getcreature()->isplayer()) {
			if(!combat_mode)
				react.method = &creature::setactive;
		} else {
			cur.set(res::CURSORS, react.target.getcreature()->getcursor());
			react.method = &creature::talk;
		}
		break;
	case Region:
		cur.set(res::CURSORS, react.target.getregion()->getcursor());
		break;
	case Position:
		if(map::isblock(map::getindex(react.target.getposition(), 1))) {
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
	auto player = creature::getactive();
	if(!player)
		return;
	if(show_actions) {
		auto x1 = x + 6, y1 = y + 12;
		if(game.selected.getcount() > 1) {
			for(auto e : actions) {
				if(act(x1, y1, e, current_action))
					execute(choose_action, e);
			}
			for(auto e : formations) {
				if(act(x1, y1, e))
					execute(choose_formation, e);
			}
		} else {
			if(act(x1, y1, ActionGuard, ActionAttack))
				execute(choose_action, ActionGuard);
			for(auto i = 0; i < 4; i++) {
				auto s1 = slot_s(QuickWeapon + i * 2 + 0);
				auto s2 = slot_s(QuickWeapon + i * 2 + 1);
				act(x1, y1, player, s1, s2);
			}
			for(auto i = QuickItem; i <= LastQuickItem; i = (slot_s)(i+1))
				act(x1, y1, *((item*)player->getwear(i)));
		}
	}
	auto x1 = 506, y1 = y + 4;
	if(show_players) {
		for(auto p : game.party) {
			if(act(x1, y1, *p, pd, change_players))
				execute(choose_player, (int)p);
		}
	}
	rcs.y2 = y;
}

static void render_footer(rect& rcs, bool show_buttons = true) {
	auto i = draw::getframe(12) % 32;
	draw::image(0, 493, res::GCOMM, 0);
	if(show_buttons) {
		if(button(576, 496, 0, res::GCOMMBTN, 0, 0, 1, 0, 0, 0, 0, 0))
			execute(creature::select_all);
		if(button(575, 565, 0, res::GCOMMBTN, 0, 16, 17, 0, 0, 0, 0, 0))
			execute(nothing);
		if(button(600, 515, 0, res::GCOMMBTN, 0, 4, 5, 0, 0, 0, 0, 0))
			execute(character_sheet);
		if(button(630, 510, 0, res::GCOMMBTN, 0, 6, 7, 0, 0, 0, 0, 0))
			execute(character_invertory);
		if(button(668, 514, 0, res::GCOMMBTN, 0, 8, 9, 0, 0, 0, 0, 0))
			execute(character_spellbook);
		if(button(600, 550, 0, res::GCOMMBTN, 0, 14, 15, 0, 0, 0, 0, 0))
			execute(game_minimap);
		if(button(628, 553, 0, res::GCOMMBTN, 0, 12, 13, 0, 0, 0, 0, 0))
			execute(game_journal);
		if(button(670, 550, 0, res::GCOMMBTN, 0, 10, 11, 0, 0, 0, 0, 0))
			execute(game_option);
		if(button(703, 495, 0, res::GCOMMBTN, 0, 2, 3, 0, 0, 0, 0, 0))
			execute(nothing);
		if(button(736, 536, 0, res::CGEAR, i, i, i, i, 0, 0, 0, true))
			execute(nothing);
	} else
		image(736, 536, gres(res::CGEAR), i, 0);
	draw::image(757, 494, res::GCOMMBTN, creature::iscombatmode() ? 19 : 18, 0);
	mspaint({12, 500, 542, 592}, {554, 497, 566, 592});
	rcs.y2 -= 107;
}

static variant render_screen(bool active_buttons, bool show_actions, bool show_players, bool show_background, bool change_players, short unsigned cap_movement) {
	rect rcs = {0, 0, getwidth(), getheight()};
	if(game.panel == setting::PanelFull)
		render_footer(rcs, active_buttons);
	if(game.panel == setting::PanelFull || game.panel == setting::PanelActions)
		render_panel(rcs, show_actions, 0, show_players, show_background, change_players);
	return render_area(rcs, hot.mouse, cap_movement);
}

#ifdef _DEBUG
static void character_test() {
	auto player = creature::getactive();
	if(!player)
		return;
	//varianta creatures; creatures.creatures(Hostile, true);
	//if(!creatures)
	//	return;
	//auto player = creatures[0].getcreature();
	//if(!player)
	//	return;
	//player->testground();
	player->choose_target();
}
#endif

static hotkey movement_keys[] = {{move_left, KeyLeft, "Двигать влево"},
{move_right, KeyRight, "Двигать вправо"},
{move_up, KeyUp, "Двигать вверх"},
{move_down, KeyDown, "Двигать вниз"},
{change_mode, Alpha + 'G', "Изменить режим панелей"},
#ifdef _DEBUG
{layer_search, Alpha + Ctrl + 'S', "Наложить фильтр карты поиска"},
{layer_path, Alpha + Ctrl + 'P', "Наложить фильтр карты пути"},
{character_test, Alpha + 'T', "Тестирование анимации"},
#endif
{}};

static hotkey menu_keys[] = {{character_invertory, Alpha + 'I', "Предметы инвентаря"},
{character_sheet, Alpha + 'C', "Листок персонажа"},
{character_spellbook, Alpha + 'S', "Заклинания персонажа"},
{creature::select_all, Alpha + '=', "Выбрать всех"},
{game_minimap, Alpha + 'M', "Карта местности"},
{game_option, Alpha + 'O', "Опции"},
{game_journal, Alpha + 'J', "Журнал заданий"},
{game_zoom, Alpha + 'Z', "Увеличенный режим"},
{adventure_step, KeyEscape, "Вернуться в режим приключений"},
{}};

void draw::menumodal(bool use_keys) {
	rect rcs = {0, 0, getwidth(), getheight()};
	render_footer(rcs);
	render_panel(rcs, false);
	domodal();
	if(use_keys)
		translate(menu_keys);
}

static void party_interact(point destination) {
	if(creature::iscombatmode()) {
		auto player = creature::getactive();
		if(player) {
			player->move(destination, 0, map::getrange(player->getmovement()));
			player->wait();
		}
	} else if(game.selected) {
		auto start = game.selected[0]->getposition();
		auto index = 0;
		for(auto p : game.selected)
			p->move(map::getfree(p->getposition(start, destination, game.formation, index++), p->getsize()));
	}
}

void actor::wait(char percent) {
	cursorset cur(res::NONE);
	auto maximum_frame = getciclecount(getcicle());
	auto stop_frame = (maximum_frame * percent) / 100;
	while(ismodal()) {
		setcamera(position);
		if(isready())
			break;
		if(stop_frame && frame >= stop_frame)
			break;
		render_screen(false, false, false, false, true, 0);
		domodal();
	}
}

void moveable::wait() {
	cursorset cur(res::NONE);
	while(ismodal()) {
		if(!(*this))
			break;
		render_screen(false, false, false, false, false, 0);
		domodal();
	}
}

variant creature::choose_position(int cursor, short unsigned max_cost) {
	variant tg;
	cursorset cur;
	animation shifter;
	while(ismodal()) {
		cur.set(res::CURSORS, cursor);
		create_shifer(shifter, camera);
		auto tg = render_screen(false, true, true, true, false, max_cost);
		short unsigned index, cost;
		switch(tg.type) {
		case Position:
			index = map::getindex(tg.getposition());
			cost = map::getcost(index);
			if(cost == Blocked)
				tg.clear();
			if(max_cost && !cost)
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

static void checkcombat(unsigned& counter) {
	if(draw::getframe() < counter)
		return;
	counter += 1000;
	for(auto p : game.party) {
		if(!p->isready())
			continue;
		for(auto& m : bsdata<creature>()) {
			if(p == &m)
				continue;
			if(!m.isready())
				continue;
			if(m.isenemy(*p)) {
				creature::makecombat();
				return;
			}
		}
	}
}

static void item_to_backpack() {
	auto pi = (item*)hot.param;
	auto player = creature::getactive();
	if(!player)
		return;
	player->add(*pi);
	pi->clear();
}

static void item_to_container() {
	auto pi = (item*)hot.param;
	switch(current_target.type) {
	case Container:
		current_target.getcontainer()->add(*pi);
		break;
	case ItemGround:
		map::drop(current_target.getitemground()->index, *pi);
		break;
	default:
		return;
	}
	pi->clear();
}

static bool translate_mouse(const targetreaction& tg) {
	auto combat_mode = creature::iscombatmode();
	auto player = creature::getactive();
	auto result = false;
	switch(hot.key) {
	case MouseLeft:
		switch(tg.target.type) {
		case Region:
			switch(tg.target.getregion()->type) {
			case RegionInfo:
				if(!hot.pressed) {
					mslog(tg.target.getregion()->name);
					textblend(tg.target.getregion()->launch, tg.target.getregion()->name, 8000);
				}
				break;
			case RegionTravel:
				if(!hot.pressed) {
					auto destination = tg.target.getregion()->getposition();
					if(creature::getpartymaxdistance(destination) < 250) {
						if(!combat_mode) {
							creature::moveto(tg.target.getregion()->move_to_area, tg.target.getregion()->move_to_entrance);
							result = true;
						}
					} else {
						party_interact(destination);
						result = true;
					}
				}
				break;
			}
			break;
		case Container:
		case ItemGround:
		case Door:
		case Creature:
			if(!hot.pressed) {
				if(creature::iscombatmode())
					player->interact(tg, map::getrange(player->getmovement()) + 1, true);
				else if(tg.target.type == Creature && tg.target.getcreature()->isplayer())
					tg.target.getcreature()->setactive();
				else
					player->interact(tg, 0, false);
				result = true;
			}
			break;
		case Position:
			if(!hot.pressed) {
				party_interact(tg.target.getposition());
				result = true;
			}
			break;
		}
		break;
	}
	return result;
}

static void getin_container() {
	cursorset cur;
	animation shifter;
	if(!current_target)
		setpage();
	char temp[260]; stringbuilder sb(temp);
	citem container(142, 476 + 24, {375, 476 + 24, 387, 576}, 6, 2);
	citem backpack(501, 476 + 24, {602, 476 + 24, 614, 575}, 2, 2);
	short unsigned current_index = Blocked;
	if(current_target.type == ItemGround)
		current_index = current_target.getitemground()->index;
	while(ismodal()) {
		cur.set(res::CURSORS);
		auto player = creature::getactive();
		create_shifer(shifter, camera);
		auto container_frame = 10;
		switch(current_target.type) {
		case Container:
			container.update(current_target.getcontainer());
			container_frame = current_target.getcontainer()->getframe();
			break;
		case ItemGround:
			container.update(current_index);
			break;
		}
		rect rcs = {0, 0, getwidth(), getheight()};
		int x = 0, y = 476;
		image(x, y, res::GUICONT, 1, 0);
		image(x + 59, y + 25, res::CONTAINER, container_frame, 0);
		if(player) {
			image(x + 430, y + 28, res::CONTAINER, 1, 0);
			container.view(player, item_to_backpack);
			backpack.update(player);
			backpack.view(player, item_to_container);
			button(x + 684, y + 28, 0, res::GBTNOPT1, 0, 1, 2, 3, 0, 0, 0);
			sb.clear(); sb.add("%1i", player->getmoney() / SP);
			labelr(x + 661, y + 78, 70, 20, temp); // NORMAL
		}
		rcs.y2 -= 107;
		auto tg = render_area(rcs, hot.mouse, 0);
		render_shifter(shifter, cur);
		domodal();
		translate(movement_keys);
		translate(menu_keys);
		if(translate_mouse(tg))
			setpage();
		creature::updategame();
	}
}

void creature::getin(const variant& e) {
	current_target = e;
	setpage(getin_container);
}

void creature::adventure() {
	cursorset cur;
	animation shifter;
	if(!getactive())
		game.party[0]->setactive();
	unsigned counter = draw::getframe();
	while(ismodal()) {
		cur.set(res::CURSORS);
		auto player = creature::getactive();
		create_shifer(shifter, camera);
		auto tg = apply_cursor(cur, render_screen(true, true, true, true, true, 0));
		render_shifter(shifter, cur);
		domodal();
		translate(movement_keys);
		translate(menu_keys);
		translate_mouse(tg);
		updategame();
		checkcombat(counter);
	}
}

void creature::adventure_combat() {
	cursorset cur;
	animation shifter;
	unsigned counter = draw::getframe();
	while(ismodal()) {
		cur.set(res::CURSORS);
		auto player = creature::getactive();
		create_shifer(shifter, camera);
		auto tg = apply_cursor(cur, render_screen(true, true, true, true, false, 0));
		render_shifter(shifter, cur);
		domodal();
		translate(movement_keys);
		translate(menu_keys);
		if(translate_mouse(tg))
			setpage(0);
		updategame();
	}
}

bool creature::iscombatmode() {
	return getlayout() == adventure_combat;
}

void creature::choose_action() {
	setactive();
	draw::setlayout(adventure_combat);
}

void actor::setcamera(point value) {
	camera = value;
	correct_camera();
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
	rc.x1 = camera.x - camera_size.x / 2;
	rc.y1 = camera.y - camera_size.y / 2;
	rc.x2 = rc.x1 + camera_size.x;
	rc.y2 = rc.y1 + camera_size.y;
	if(position.in(rc))
		return;
	point start, destination;
	start = camera;
	destination = position;
	auto range = 0;
	auto step = 4;
	auto range_maximum = distance(start, destination);
	auto dx = destination.x - start.x;
	auto dy = destination.y - start.y;
	while(ismodal() && range < range_maximum) {
		update_timestamp();
		point p1;
		p1.x = start.x + dx * range / range_maximum;
		p1.y = start.y + dy * range / range_maximum;
		setcamera(p1);
		render_screen(true, true, true, true, false, 0);
		sysredraw();
		range += step;
	}
}