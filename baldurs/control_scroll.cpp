#include "main.h"

using namespace draw;

scrolltext::scrolltext() : origin(0), maximum(0), increment(16), bar(res::GBTNSCRL),
cache_text(0), cache_height(0), cache_origin(-1), scroll_frame(4) {}

void scrolltext::reset() {
	cache_origin = 0;
}

static int* scroll_value;

static void scroll_change() {
	if(scroll_value)
		*scroll_value = *scroll_value + hot.param;
}

static void scroll_button(rect rc, sprite* pb, int i, int& value, int inc) {
	bool pressed = draw::area(rc) == AreaHilitedPressed;
	draw::image(rc.x1, rc.y1, pb, i + (pressed ? 1 : 0), 0);
	if(pressed) {
		if(hot.key == MouseLeft && hot.pressed) {
			scroll_value = &value;
			draw::execute(scroll_change, inc);
		}
	}
}

void draw::view(rect rc, rect rcs, int pixels_per_line, scrolllist& e) {
	int lines_per_screen = rc.height() / pixels_per_line;
	if(draw::areb(rc) || draw::areb(rcs)) {
		switch(hot.key) {
		case MouseWheelDown:
			e.origin++;
			break;
		case MouseWheelUp:
			e.origin--;
			break;
		}
	}
	sprite* pb = gres(e.bar);
	if(pb) {
		scroll_button({rcs.x1, rcs.y1, rcs.x2, rcs.y1 + rcs.width()}, pb, 0, e.origin, -1);
		scroll_button({rcs.x1, rcs.y2 - rcs.width(), rcs.x2, rcs.y2}, pb, 2, e.origin, 1);
	}
	//rectangle(rc, colors::white);
	auto valid_maximum = e.maximum - lines_per_screen - 1;
	if(e.origin > valid_maximum)
		e.origin = valid_maximum;
	if(e.origin < 0)
		e.origin = 0;
	if(pb) {
		//rectangle(rcs, colors::red);
		int h = rcs.height() - pb->get(0).sy - pb->get(2).sy - pb->get(e.scroll_frame).sy - 2;
		int h1 = valid_maximum;
		if(h1 > 0) {
			int my = e.origin*h / h1;
			draw::image(rcs.x1, rcs.y1 + pb->get(0).sy + my, pb, e.scroll_frame, 0);
		}
	}
	auto y1 = rc.y1;
	auto rw = rc.width();
	auto i = e.origin;
	while(true) {
		if(y1 >= rc.y2)
			break;
		if(i >= e.maximum)
			break;
		struct rect rcm = {rc.x1, y1, rc.x1 + rw, y1 + pixels_per_line};
		e.row(rcm, i);
		y1 += pixels_per_line;
		i++;
	}
}

void draw::view(rect rc, rect rcs, const char* text, scrolltext& e) {
	if(e.cache_origin != e.origin) {
		draw::state push;
		draw::setclip({0, 0, 0, 0});
		e.maximum = draw::textf(rc.x1, rc.y1, rc.width(), text);
		e.cache_origin = e.origin;
	}
	int lines_per_screen = rc.height();
	if(draw::areb(rc) || draw::areb(rcs)) {
		switch(hot.key) {
		case MouseWheelDown:
			e.origin += e.increment;
			break;
		case MouseWheelUp:
			e.origin -= e.increment;
			break;
		}
	}
	sprite* pb = gres(e.bar);
	if(pb) {
		scroll_button({rcs.x1, rcs.y1, rcs.x2, rcs.y1 + rcs.width()}, pb, 0, e.origin, -e.increment);
		scroll_button({rcs.x1, rcs.y2 - rcs.width(), rcs.x2, rcs.y2}, pb, 2, e.origin, e.increment);
	}
	//rectangle(rc, colors::white);
	auto valid_maximum = e.maximum - lines_per_screen - 1;
	if(e.origin > valid_maximum)
		e.origin = valid_maximum;
	if(e.origin < 0)
		e.origin = 0;
	if(pb) {
		//rectangle(rcs, colors::red);
		int h = rcs.height() - pb->get(0).sy - pb->get(2).sy - pb->get(e.scroll_frame).sy - 2;
		int h1 = valid_maximum;
		if(h1 > 0) {
			int my = e.origin*h / h1;
			draw::image(rcs.x1, rcs.y1 + pb->get(0).sy + my, pb, e.scroll_frame, 0);
		}
	}
	draw::state push;
	draw::setclip(rc);
	draw::textf(rc.x1, rc.y1 - e.origin + e.cache_height, rc.width(), text);
}