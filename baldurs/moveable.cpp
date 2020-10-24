#include "main.h"

BSDATAC(moveable, 128)
static unsigned	arrow_flags[16] = {0, 0, 0, 0, 0,
ImageMirrorV, ImageMirrorV, ImageMirrorV, ImageMirrorV,
ImageMirrorV | ImageMirrorH, ImageMirrorV | ImageMirrorH, ImageMirrorV | ImageMirrorH, ImageMirrorV | ImageMirrorH,
ImageMirrorH, ImageMirrorH, ImageMirrorH};
static unsigned	arrow_frame[16] = {0, 1, 2, 3, 4,
3, 2, 1, 0,
1, 2, 3, 4,
3, 2, 1};

moveable::moveable(point start, point finish, res::tokens id, unsigned feets_per_second) :
	start(start), finish(finish), position(start), avatar(id), use_colors(false) {
	orientation = map::getorientation(start, finish);
	auto range = distance(start, finish);
	auto pixel_per_seconds = feets_per_second * 16 / 5;
	time_start = draw::getframe();
	time_finish = time_start + (range * 1000) / pixel_per_seconds;
}

void* moveable::operator new(unsigned size) {
	for(auto& e : bsdata<moveable>()) {
		if(!e)
			return &e;
	}
	return bsdata<moveable>::add();
}

void moveable::update() {
	if(!avatar)
		return;
	auto stamp = draw::getframe();
	if(stamp > time_finish) {
		avatar = res::NONE;
		return;
	}
	int d1 = stamp - time_start;
	int d2 = time_finish - time_start;
	int dx = finish.x - start.x;
	int dy = finish.y - start.y;
	position.x = start.x + dx * d1 / d2;
	position.y = start.y + dy * d1 / d2;
}

void moveable::painting(point screen) const {
	int x = position.x - screen.x;
	int y = position.y - screen.y;
	if(use_colors) {
		color pallette[256];
		colors.upload(pallette);
		draw::image(x, y, draw::gres(avatar), arrow_frame[orientation], arrow_flags[orientation], 0xFF, pallette);
	} else
		draw::image(x, y, draw::gres(avatar), arrow_frame[orientation], arrow_flags[orientation]);
}