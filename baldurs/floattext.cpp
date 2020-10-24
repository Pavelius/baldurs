#include "main.h"

BSDATAC(floattext, 32)

void floattext::clear() {
	text = 0;
	stamp = 0;
}

void floattext::painting(point offset) const {
	color r = colors::white;
	if(stamp + 1000 >= draw::getframe())
		r.mix(colors::black, 200 * (stamp + 1000 - draw::getframe()) / 1000);
	rect rc = getrect();
	rc.move(-offset.x, -offset.y);
	draw::rectf(rc, colors::black, 192);
	draw::label(box.x1 - offset.x, box.y1 - offset.y, box.width(), box.height(), text, 0, 0);
}

void floattext::update() {
	if(!isvisible())
		return;
	if(draw::getframe() > stamp)
		clear();
}

static floattext* find(point pt, const char* text) {
	for(auto& e : bsdata<floattext>()) {
		if(!e.text)
			continue;
		if(strcmp(e.text, text)==0)
			return &e;
	}
	for(auto& e : bsdata<floattext>()) {
		if(!e)
			return &e;
	}
	return bsdata<floattext>::add();
}

void draw::textblend(point pos, const char* text, unsigned duration) {
	pos.x -= 150;
	auto p = find(pos, text);
	if(!p)
		return;
	p->text = text;
	p->stamp = draw::getframe() + duration;
	p->box.x1 = p->box.y1 = 0; p->box.x2 = 300;
	p->box.y2 = draw::textf(p->box, text);
	p->box.move(pos.x, pos.y);
}