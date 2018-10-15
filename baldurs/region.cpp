#include "main.h"

adat<region, 256> region_data;

int region::getcursor() const {
	switch(type) {
	case RegionTravel: return 42;
	case RegionInfo: return 22;
	default: return 34;
	}
	return 30;
}

template<> void archive::set<region>(region& e) {
	set(e.type);
	set(e.launch);
	set(e.box);
	set(e.move_to_area);
	set(e.move_to_entrance);
	set(e.name);
	set(e.points);
}