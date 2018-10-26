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

//static int minimap_drag() {
//	sprite* mm = get_area_minimap();
//	const sprite::frame& sf = mm->get(0);
//	draw::screenshoot back;
//	while(true) {
//		update_logic();
//		back.restore();
//		set_camera(s2m(hot.mouse));
//		draw::image(mview.x1, mview.y1, mm, 0, 0);
//		draw::rectb({x2m(camera.x), y2m(camera.y),
//			x2m(camera.x + camera_sizes.x), y2m(camera.y + camera_sizes.y)},
//			colors::white);
//		int id = draw::input();
//		switch(id) {
//		case 0:
//			return 0;
//		case KeyEscape:
//			return Next;
//		case MouseLeft:
//			if(!hot.pressed)
//				return Next;
//			break;
//		}
//	}
//}

static void worldmap() {
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
		button(680, 288, cmpr(worldmap), 0, res::GUIMAPWC, 0, 0, 1, 0, 0, 0, 0, 0);
		draw::image(mview.x1, mview.y1, mm, 0, 0);
		auto camera = getcamera();
		auto camera_size = getcamerasize();
		draw::rectb({x2m(camera.x), y2m(camera.y),
			x2m(camera.x + camera_size.x), y2m(camera.y + camera_size.y)},
			colors::white);
		button(664, 54, cmpr(worldmap), 0, res::GBTNOPT1, 0, 1, 2, 3, 0, 0, 0);
		if(hot.mouse.in(mview))
			cur.set(res::CURSORS, 44);
		else
			cur.set();
		menumodal();
		switch(hot.key) {
		case MouseLeft:
			if(hot.pressed && hot.mouse.in(mview))
				setcamera(s2m(hot.mouse));
			break;
		}
	}
}