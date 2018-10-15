#include "draw.h"
#include "screenshoot.h"

draw::screenshoot::screenshoot(rect rc, bool fade) : surface(rc.width(), rc.height(), getbpp()) {
	x = rc.x1;
	y = rc.y1;
	if(draw::canvas) {
		blit(*this, 0, 0, width, height, 0, *draw::canvas, x, y);
		if(fade) {
			draw::state push;
			draw::canvas = this;
			draw::setclip();
			draw::rectf({0, 0, width, height}, colors::black, 128);
		}
	}
}

draw::screenshoot::screenshoot(bool fade) : screenshoot({0, 0, getwidth(), getheight()}, fade) {
}

draw::screenshoot::~screenshoot() {
}

void draw::screenshoot::restore() {
	setclip();
	if(draw::canvas)
		blit(*draw::canvas, x, y, width, height, 0, *this, 0, 0);
}