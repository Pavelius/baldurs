#include "main.h"

using namespace draw;

static rect mview;

static int x2m(int x) {
	return mview.x1 + x * mview.width() / (map::width * 16);
}

static int y2m(int y) {
	return mview.y1 + y * mview.height() / (map::height * 12);
}

static point s2m(point m) {
	return{
		(short)((m.x - mview.x1)*(map::width * 16) / mview.width()),
		(short)((m.y - mview.y1)*(map::height * 12) / mview.height())};
}

static void choose_map_info() {
	screenshoot screen(true);
	cursorset cur;
	while(ismodal()) {
		int x = 201, y = 88;
		screen.restore();
		image(x, y, res::GUIMAP, 1, 0);
		button(x + 61, y + 157, cmpr(buttonparam, 1), 0, res::GBTNMED, 0, 1, 2, 3, "Удалить", 0, 0);
		button(x + 61, y + 187, cmpr(buttonok), 0, res::GBTNMED, 0, 1, 2, 2, "Применить", KeyEnter, 0);
		button(x + 61, y + 217, cmpr(buttoncancel), 0, res::GBTNMED, 0, 1, 2, 3, "Отмена", KeyEscape, 0);
		rectb({x + 20, y + 20, x + 258, y + 123}, colors::white);
		button(x + 21, y + 127, cmpr(buttonparam, 5), 0, res::FLAG1, 0, 0, 2, 1, 0, 0, 0);
		button(x + 51, y + 127, cmpr(buttonparam, 6), 0, res::FLAG1, 0, 0, 2, 1, 0, 0, 0);
		button(x + 81, y + 127, cmpr(buttonparam, 7), 0, res::FLAG1, 0, 0, 2, 1, 0, 0, 0);
		button(x + 111, y + 127, cmpr(buttonparam, 8), 0, res::FLAG1, 0, 0, 2, 1, 0, 0, 0);
		button(x + 141, y + 127, cmpr(buttonparam, 9), 0, res::FLAG1, 0, 0, 2, 1, 0, 0, 0);
		button(x + 171, y + 127, cmpr(buttonparam, 10), 0, res::FLAG1, 0, 0, 2, 1, 0, 0, 0);
		button(x + 201, y + 127, cmpr(buttonparam, 11), 0, res::FLAG1, 0, 0, 2, 1, 0, 0, 0);
		button(x + 231, y + 127, cmpr(buttonparam, 12), 0, res::FLAG1, 0, 0, 2, 1, 0, 0, 0);
		domodal();
	}
}

void choose_menu(void(*proc)());

static void open_minimap() {
	choose_menu(creature::minimap);
}

static void render_worldmap() {
	cursorset cur;
	auto pw = worldmap::getworld();
	while(ismodal()) {
		cur.set(res::CURSORS, 267);
		worldmap::area* current_area_hilite = 0;
		background(res::GUIMAP, 2);
		int x = 0, y = 0;
		auto x1 = x + 23, y1 = y + 20;
		if(pw) {
			auto back = gres(pw->background, "data/worldmap");
			image(x1, y1, back, 0, 0);
			auto icons = gres(pw->icons, "data/worldmap");
			if(icons) {
				rect rc = {x1, y1, x1 + back->get(0).sx, y1 + back->get(0).sy};
				if(hot.mouse.in(rc))
					cur.setblock();
				for(auto& e : pw->areas) {
					auto x = x1 + e.position.x;
					auto y = y1 + e.position.y;
					image(x, y, icons, e.avatar, 0);
					auto w = textw(e.name);
					text(x - w / 2 + 8, y + 16, e.name, -1, TextStroke);
					if(hot.mouse.in({x, y, x + 16, y + 16}))
						current_area_hilite = &e;
				}
			}
		}
		button(x + 680, y + 288, cmpr(open_minimap), 0, res::GUIMAPWC, 0, 0, 1, 0, 0, 0, 0);
		label(x + 666, y + 18, 113, 22, "Карта мира");
		if(current_area_hilite)
			cur.set(res::CURSORS, 267);
		menumodal();
		switch(hot.key) {
		case MouseLeft:
			if(hot.pressed) {
				if(current_area_hilite)
					creature::moveto(current_area_hilite->id);
			}
			break;
		}
	}
}

static void open_worldmap() {
	choose_menu(render_worldmap);
}

void creature::minimap() {
	cursorset cur;
	char description[4096]; description[0] = 0;
	auto mm = map::getminimap();
	auto& sf = mm->get(0);
	mview.x1 = 98 + (578 - 98 - sf.sx) / 2;
	mview.x2 = mview.x1 + sf.sx;
	mview.y1 = 36 + (396 - 36 - sf.sy) / 2;
	mview.y2 = mview.y1 + sf.sy;
	while(ismodal()) {
		background(res::GUIMAP);
		label(666, 18, 113, 22, "Карта местности");
		label(696, 56, 82, 20, "Обозначения");
		label(668, 92, 109, 165, description);
		button(680, 288, cmpr(open_worldmap), 0, res::GUIMAPWC, 0, 0, 1, 0, 0, 0, 0, 0);
		draw::image(mview.x1, mview.y1, mm, 0, 0);
		auto camera = getcamera();
		auto camera_size = getcamerasize();
		draw::rectb({x2m(camera.x), y2m(camera.y),
			x2m(camera.x + camera_size.x), y2m(camera.y + camera_size.y)},
			colors::white);
		for(auto& e : players) {
			if(!e)
				continue;
			auto position = e.getposition();
			auto x1 = x2m(position.x);
			auto y1 = y2m(position.y);
			circle(x1, y1, 2, colors::green);
		}
		button(664, 54, cmpr(buttonparam), 0, res::GBTNOPT1, 0, 1, 2, 3, 0, 0, 0);
		if(hot.mouse.in(mview))
			cur.set(res::CURSORS, 44);
		else
			cur.set();
		switch(hot.key) {
		case MouseRight:
			if(hot.pressed && hot.mouse.in(mview))
				execute(choose_map_info);
			break;
		}
		menumodal();
		switch(hot.key) {
		case MouseLeft:
			if(hot.pressed && hot.mouse.in(mview))
				setcamera(s2m(hot.mouse));
			break;
		}
	}
}