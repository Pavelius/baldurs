#include "main.h"

static worldmap::area world_areas_0[] = {{"AR1000", "Targos Docks", {265, 59}, 0, {}},
{"AR1100", "Targos", {245, 91}, 1, {}},
{"AR1200", "Targos Palisade", {206, 109}, 2, {{North, 5, "FRWMap", 3, 5, 1}, {North, 4, "FRWMap", 2, 5, 1}}},
{"AR2102", "Shaengarne Bridge", {92, 277}, 4, {{North, 2, "FRWMap", 2, 5, 1}, {North, 1, "FR1200", 2, 5, 1}, {North, 0, "FR1100", 2, 5, 1}}},
{"AR2000", "Shaengarne Ford", {89, 235}, 3, {{North, 2, "FRWMap", 2, 5, 1}, {North, 1, "FR1200", 2, 5, 1}, {North, 0, "FR1100", 2, 5, 1}, {North, 5, "FRWMap", 1, 5, 1}}},
{"AR3000", "Horde Fortress", {270, 260}, 5, {{North, 2, "FRWMap", 3, 5, 1}, {North, 1, "FR1200", 3, 5, 1}, {North, 0, "FR1100", 3, 5, 1}, {North, 4, "FRWMap", 1, 5, 1}}},
{"", "Bremen", {159, 34}, 23, {}},
{"", "Lonelywood", {289, 15}, 23, {}},
{"", "Kelvin's Cairn", {444, 17}, 23, {}},
{"", "Icewind Pass", {552, 100}, 23, {}},
{"", "Caer-Konig", {483, 153}, 23, {}},
{"", "Caer-Dinival", {459, 194}, 23, {}},
{"", "Easthaven", {441, 244}, 23, {}},
{"", "Good Mead", {306, 335}, 23, {}},
{"", "Dougan's Hole", {280, 355}, 23, {}},
{"", "Western Pass", {75, 305}, 23, {}},
{"", "Termalaine", {346, 74}, 23, {}},
};
static worldmap::area world_areas_1[] = {{"AR4000", "Western Pass", {135, 55}, 6, {}},
{"AR4100", "Icewall", {147, 93}, 7, {}},
{"AR5000", "Wandering Village", {255, 121}, 8, {{North, 4, "FR5000", 1, 5, 1}, {North, 7, "FR5102", 6, 5, 1}, {North, 6, "FR5004", 4, 5, 1}, {North, 3, "FR5015", 3, 5, 1}}},
{"AR5004", "Cold Marshes", {409, 133}, 10, {}},
{"AR5001", "The Fell Wood", {344, 98}, 9, {{North, 2, "FR5001", 1, 5, 1}, {North, 7, "FR5102", 7, 5, 1}}},
{"AR5102", "River Caves Exit", {477, 49}, 12, {}},
{"AR5005", "River Caves", {483, 100}, 11, {{North, 3, "FR5005", 1, 5, 1}, {North, 2, "FR5001", 4, 5, 1}}},
{"AR5200", "Black Raven Monastery", {446, 5}, 13, {{North, 2, "FR5001", 6, 5, 1}, {North, 5, "FR5200", 1, 5, 1}}},
{"AR5300", "The Underdark", {541, 34}, 14, {}},
{"", "Luskan", {203, 299}, 23, {}},
{"", "Neverwinter Wood", {475, 320}, 23, {}},
};
static worldmap::area world_areas_2[] = {{"AR6000", "Kuldahar Valley", {263, 104}, 20, {}},
{"AR6001", "Kuldahar", {322, 87}, 15, {{North, 3, "FRWMap", 4, 5, 1}, {North, 5, "FRWMap", 6, 5, 1}, {North, 6, "FRWMap", 6, 5, 1}, {North, 0, "FR6001a", 0, 5, 1}}},
{"AR6104", "Dragon's Eye Exit", {496, 176}, 16, {{North, 5, "FRWMap", 2, 5, 1}, {North, 1, "FRWMapE", 4, 5, 1}}},
{"AR6100", "Dragon's Eye", {439, 118}, 16, {{North, 1, "FRWMapE", 4, 5, 1}}},
{"AR6201", "Fields of Slaughter", {481, 357}, 18, {{North, 6, "FRWMap", 2, 5, 1}, {North, 2, "FRWMap", 2, 5, 1}, {North, 1, "FRWMapE", 6, 5, 1}}},
{"AR6200", "Fields of Slaughter", {522, 323}, 17, {{North, 2, "", 2, 5, 1}, {North, 1, "FRWMapE", 6, 5, 1}}},
{"AR6300", "Severed Hand", {391, 345}, 19, {{North, 2, "FRWMap", 6, 5, 1}, {North, 1, "FRWMapE", 6, 5, 1}, {South, 4, "FR6300", 2, 5, 2}}},
{"", "", {590, 302}, 21, {}},
{"", "", {590, 357}, 21, {}},
};
static worldmap world[] = {{"WMAP1", "Icewind Dale II", "MAPICONS", world_areas_0},
{"WMAP2", "Western Pass", "MAPICONS", world_areas_1},
{"WMAP3", "Kuldahar", "MAPICONS", world_areas_2}};

static worldmap::area* current_area;

worldmap::area* worldmap::getarea() {
	return current_area;
}

void worldmap::set(area* value) {
	if(!value)
		return;
	current_area = value;
}

worldmap::area* worldmap::getarea(const char* id) {
	for(auto& e : world) {
		for(auto& a : e.areas) {
			if(strcmp(a.id, id) == 0)
				return &a;
		}
	}
	return 0;
}

worldmap* worldmap::getworld(worldmap::area* p) {
	for(auto& e : world) {
		if(e.areas.indexof(p) != -1)
			return &e;
	}
	return 0;
}

aref<worldmap> getworldmaps() {
	return aref<worldmap>(world);
}