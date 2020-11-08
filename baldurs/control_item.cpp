#include "main.h"

using namespace draw;

citem::citem(int x, int y, const rect& scroll, int mx, int my) : cscroll({x, y, x + mx*size, y + my*size}, scroll),
mx(mx), my(my), maximum_items(0) {}

item* citem::get(int index) const {
	if(index < getcount())
		return data[index];
	return 0;
}

int citem::getmaximum() const {
	return (maximum_items + mx - 1) / mx;
}

void citem::update(creature* player) {
	auto bi = origin * mx;
	auto pb = data;
	auto pe = pb + sizeof(data) / sizeof(data[0]);
	maximum_items = 0;
	for(auto i = Backpack; i <= LastBackpack; i = (slot_s)(i + 1)) {
		auto pi = player->get(i);
		if(!pi || !(*pi))
			continue;
		maximum_items++;
		if(bi-- > 0)
			continue;
		if(pb < pe)
			*pb++ = pi;
	}
	count = pb - data;
}

void citem::update(container* object) {
	auto bi = origin * mx;
	auto pb = data;
	auto pe = pb + sizeof(data) / sizeof(data[0]);
	maximum_items = 0;
	for(auto& e : bsdata<itemcont>()) {
		if(!e)
			continue;
		if(e.object != object)
			continue;
		maximum_items++;
		if(bi-- > 0)
			continue;
		if(pb < pe)
			*pb++ = &e;
	}
	count = pb - data;
}

void citem::update(short unsigned index) {
	auto bi = origin * 2;
	auto pb = data;
	auto pe = pb + sizeof(data) / sizeof(data[0]);
	maximum_items = 0;
	for(auto& e : bsdata<itemground>()) {
		if(!e)
			continue;
		if(e.index != index)
			continue;
		maximum_items++;
		if(bi-- > 0)
			continue;
		if(pb < pe)
			*pb++ = &e;
	}
	count = pb - data;
}

void citem::view(creature* player, fnevent item_proc, itemdrag* di) {
	cscroll::view();
	auto x = client.x1;
	auto y = client.y1;
	const auto c = mx*my;
	for(auto i = 0; i < c; i++) {
		auto pi = get(i);
		player->icon(x + (i % mx) * size, y + (i / mx) * size,
			pi, LastBackpack, di, item_proc);
	}
}