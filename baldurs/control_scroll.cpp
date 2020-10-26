#include "main.h"

using namespace draw;

scrolltext::scrolltext() : origin(0), maximum(0), increment(16), bar(res::GBTNSCRL),
cache_text(0), cache_height(0), cache_origin(-1), scroll_frame(4) {}

void scrolltext::reset() {
	cache_origin = -1;
}

void scrolltext::prerender() {
}

item* scrollitem::get(int index) const {
	if(index < maximum_items)
		return data[index];
	return 0;
}

void scrollitem::update(creature* player, int item_in_line) {
	auto bi = origin * item_in_line;
	maximum_items = 0;
	auto pb = data;
	auto pe = pb + sizeof(data) / sizeof(data[0]);
	for(auto i = Backpack; i <= LastBackpack; i = (slot_s)(i + 1)) {
		auto pi = player->get(i);
		if(!pi || !(*pi))
			continue;
		if(maximum_items++ < bi)
			continue;
		if(pb < pe)
			*pb++ = pi;
	}
	maximum = (maximum_items + (item_in_line - 1)) / item_in_line;
}

void scrollitem::update(container* object, int item_in_line) {
	auto bi = origin * 2;
	maximum_items = 0;
	auto pb = data;
	auto pe = pb + sizeof(data) / sizeof(data[0]);
	for(auto& e : bsdata<itemcont>()) {
		if(e.object != object)
			continue;
		if(!e)
			continue;
		if(maximum_items++ < bi)
			continue;
		if(pb < pe)
			*pb++ = &e;
	}
	maximum = (maximum_items + 1) / 2;
}

void scrollitem::update(short unsigned index, int item_in_line) {
	auto i = origin * 2;
	maximum_items = 0;
	auto pb = data;
	auto pe = pb + sizeof(data) / sizeof(data[0]);
	for(auto& e : bsdata<itemground>()) {
		if(!e)
			continue;
		if(e.index != index)
			continue;
		if(maximum_items++ < i)
			continue;
		if(pb < pe)
			*pb++ = &e;
	}
	maximum = (maximum_items + 1) / 2;
}

void scrollitem::view(creature* player, int x, int y, int dx, int dy, const rect& rcs, fnevent item_proc) {
	auto count = getcount();
	for(auto i = 0; i < count; i++) {
		auto n = i % mx;
		auto k = i / mx;
		auto pi = get(i);
		player->icon(x + n * dx, y + k * dy, pi, LastBackpack, 0, item_proc);
	}
	draw::view({x, y, x + dx*mx, y + dy*my}, rcs, dy, *this);
}

static void scroll_button(rect rc, sprite* pb, int i, int& value, int inc) {
	bool pressed = draw::area(rc) == AreaHilitedPressed;
	draw::image(rc.x1, rc.y1, pb, i + (pressed ? 1 : 0), 0);
	if(pressed) {
		if(hot.key == MouseLeft && hot.pressed)
			value += inc;
	}
}

void draw::view(rect rc, rect rcs, int pixels_per_line, scrolllist& e) {
	int lines_per_screen = rc.height() / pixels_per_line;
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
	//rectb(rc, colors::white);
	e.prerender();
	auto valid_maximum = e.maximum - lines_per_screen - 1;
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
		if(i >= e.maximum)
			break;
		struct rect rcm = {rc.x1, y1, rc.x1 + rw, y1 + pixels_per_line};
		e.row(rcm, i);
		y1 += pixels_per_line;
		i++;
	}
}

void scrolltext::cashing(const char* text, int width) {
	draw::state push;
	draw::setclip({0, 0, 0, 0});
	maximum = draw::textf(0, 0, width, text, 0,
		origin, &cache_height, &cache_text, 0);
	cache_origin = origin;
}

void draw::view(rect rc, rect rcs, const char* text, scrolltext& e) {
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