#include "main.h"

using namespace draw;

ctext::ctext(const rect& client, const rect& scroll, const char* text) : cscroll(client, scroll),
text(text), text_height(0), cache_text(0), cache_height(0), cache_origin(-1)
{}

void ctext::reset() {
	cache_origin = -1;
}

void ctext::view() const {
	cscroll::view();
	draw::state push;
	draw::setclip(client);
	draw::textf(client.x1, client.y1 - getorigin() + cache_height, client.width(), cache_text);
}

void ctext::update() {
	draw::state push;
	setclip({0, 0, 0, 0});
	text_height = draw::textf(0, 0, client.width(), text) + 4;
	correct();
	draw::textf(0, 0, client.width(), text, 0, origin, &cache_height, &cache_text, 0);
	cache_origin = origin;
}

void ctext::viewc() {
	if(cache_origin != origin)
		update();
	view();
}