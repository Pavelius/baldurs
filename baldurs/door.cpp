#include "main.h"

BSDATAC(door, 256)
BSDATAC(door_tile, 1024)

void door::clear() {
	opened = false;
	locked = false;
}

void door::recount() {
	for(auto& e : tiles)
		map::settile(e.index, isopen() ? e.open : e.closed);
}

void door::setopened(bool value) {
	this->opened = value;
	recount();
}

template<> void archive::set<door>(door& e) {
	set(e.box);
	set(e.launch);
	set(e.tiles);
	set(e.points);
	set(e.open_points);
	set(e.close_points);
	set(e.search_open_points);
	set(e.search_close_points);
	set(e.opened);
	set(e.locked);
	e.recount();
}