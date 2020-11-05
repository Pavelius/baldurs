#include "main.h"

using namespace draw;

ctext::ctext() : origin(0), maximum(0), increment(16), bar(res::GBTNSCRL),
cache_text(0), cache_height(0), cache_origin(-1), scroll_frame(4) {}

void ctext::reset() {
	cache_origin = -1;
}

void ctext::prerender() {
}

static void scroll_button(rect rc, sprite* pb, int i, int& value, int inc) {
	bool pressed = draw::area(rc) == AreaHilitedPressed;
	draw::image(rc.x1, rc.y1, pb, i + (pressed ? 1 : 0), 0);
	if(pressed) {
		if(hot.key == MouseLeft && hot.pressed)
			value += inc;
	}
}

void draw::view(rect rc, rect rcs, int pixels_per_line, clist& e) {
	auto lines_per_screen = rc.height() / pixels_per_line;
	if(draw::areb(rc) || draw::areb(rcs)) {
		switch(hot.key) {
		case MouseWheelDown: e.origin++; break;
		case MouseWheelUp: e.origin--; break;
		}
	}
	sprite* pb = gres(e.bar);
	if(rcs) {
		if(pb) {
			scroll_button({rcs.x1, rcs.y1, rcs.x2, rcs.y1 + rcs.width()}, pb, 0, e.origin, -1);
			scroll_button({rcs.x1, rcs.y2 - rcs.width(), rcs.x2, rcs.y2}, pb, 2, e.origin, 1);
		}
	}
	e.prerender();
	auto valid_maximum = e.getmaximum() - lines_per_screen - 1;
	if(e.origin > valid_maximum)
		e.origin = valid_maximum;
	if(e.origin < 0)
		e.origin = 0;
	if(pb) {
		//rectb(rcs, colors::red);
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
		if(i >= e.getmaximum())
			break;
		rect rcm = {rc.x1, y1, rc.x1 + rw, y1 + pixels_per_line};
		e.row(rcm, i);
		y1 += pixels_per_line;
		i++;
	}
}

void ctext::cashing(const char* text, int width) {
	draw::state push;
	draw::setclip({0, 0, 0, 0});
	maximum = draw::textf(0, 0, width, text, 0,
		origin, &cache_height, &cache_text, 0);
	cache_origin = origin;
}

void draw::view(rect rc, rect rcs, const char* text, ctext& e) {
	int lines_per_screen = rc.height();
	if(draw::areb(rc) || draw::areb(rcs)) {
		switch(hot.key) {
		case MouseWheelDown: e.origin += e.increment; break;
		case MouseWheelUp: e.origin -= e.increment; break;
		}
	}
	sprite* pb = gres(e.bar);
	//rectb(rc, colors::white);
	if(pb) {
		//rectb(rcs, colors::red);
		scroll_button({rcs.x1, rcs.y1, rcs.x2, rcs.y1 + rcs.width()}, pb, 0, e.origin, -e.increment);
		scroll_button({rcs.x1, rcs.y2 - rcs.width(), rcs.x2, rcs.y2}, pb, 2, e.origin, e.increment);
	}
	e.prerender();
	if(e.cache_origin != e.origin)
		e.cashing(text, rc.width());
	auto valid_maximum = e.maximum - lines_per_screen - 1;
	if(valid_maximum < 0)
		valid_maximum = 0;
	if(e.origin > valid_maximum) {
		e.origin = valid_maximum;
		e.reset();
	}
	if(e.origin < 0) {
		e.origin = 0;
		e.reset();
	}
	if(e.cache_origin != e.origin)
		e.cashing(text, rc.width());
	if(pb) {
		int h = rcs.height() - pb->get(0).sy - pb->get(2).sy - pb->get(e.scroll_frame).sy - 2;
		int h1 = valid_maximum;
		if(h1 > 0) {
			int my = e.origin*h / h1;
			draw::image(rcs.x1, rcs.y1 + pb->get(0).sy + my, pb, e.scroll_frame, 0);
		}
	}
	draw::state push;
	draw::setclip(rc);
	draw::textf(rc.x1, rc.y1 - e.origin + e.cache_height, rc.width(), e.cache_text);
}