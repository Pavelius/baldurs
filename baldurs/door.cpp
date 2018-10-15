#include "main.h"

adat<door, 256>			door_data;
adat<door_tile, 1024>	door_tiles_data;

void door::clear() {
	opened = false;
	locked = false;
}

void door::recount() {
	for(auto& e : tiles)
		map::settile(e.index, isopen() ? e.open : e.closed);
}

void door::toggle() {
	opened = !opened;
	recount();
}

template<> void archive::set<door>(door& e) {
	set(e.box);
	set(e.launch);
	set(e.tiles);
	set(e.points);
	set(e.open_points);
	set(e.close_points);
	set(e.opened);
	set(e.locked);
	e.recount();
}