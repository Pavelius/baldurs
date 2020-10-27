#include "main.h"

static worldmap::area world_areas_0[] = {{"AR1000", "Доки Таргоса", {265, 59}, 0, 7, {}},
{"AR1100", "Таргос", {245, 91}, 1, 2, {}},
{"AR1200", "Частокол Таргоса", {206, 109}, 2, 2, {{North, 5, "FRWMap", 3, 5, 1}, {North, 4, "FRWMap", 2, 5, 1}}},
{"AR2102", "Шаенгарский мост", {92, 277}, 4, 2, {{North, 2, "FRWMap", 2, 5, 1}, {North, 1, "FR1200", 2, 5, 1}, {North, 0, "FR1100", 2, 5, 1}}},
{"AR2000", "Шаенгарский брод", {89, 235}, 3, 2, {{North, 2, "FRWMap", 2, 5, 1}, {North, 1, "FR1200", 2, 5, 1}, {North, 0, "FR1100", 2, 5, 1}, {North, 5, "FRWMap", 1, 5, 1}}},
{"AR3000", "Крепость Орды", {270, 260}, 5, 2, {{North, 2, "FRWMap", 3, 5, 1}, {North, 1, "FR1200", 3, 5, 1}, {North, 0, "FR1100", 3, 5, 1}, {North, 4, "FRWMap", 1, 5, 1}}},
{"", "Бремен", {159, 34}, 23, 3, {}},
{"", "Одинокий Лес", {289, 15}, 23, 3, {}},
{"", "Склеп Кельвина", {444, 17}, 23, 3, {}},
{"", "Проход Ледяного ветра", {552, 100}, 23, 3, {}},
{"", "Каэр-Кониг", {483, 153}, 23, 3, {}},
{"", "Каэр-Динивал", {459, 194}, 23, 3, {}},
{"", "Востоный край", {441, 244}, 23, 3, {}},
{"", "Славный Луг", {306, 335}, 23, 3, {}},
{"", "Дыра Дугана", {280, 355}, 23, 3, {}},
{"", "Западный проход", {75, 305}, 23, 3, {}},
{"", "Термалин", {346, 74}, 23, 3, {}},
};
static worldmap::area world_areas_1[] = {{"AR4000", "Western Pass", {135, 55}, 6, 2, {}},
{"AR4100", "Icewall", {147, 93}, 7, 2, {}},
{"AR5000", "Wandering Village", {255, 121}, 8, 2, {{North, 4, "FR5000", 1, 5, 1}, {North, 7, "FR5102", 6, 5, 1}, {North, 6, "FR5004", 4, 5, 1}, {North, 3, "FR5015", 3, 5, 1}}},
{"AR5004", "Cold Marshes", {409, 133}, 10, 2, {}},
{"AR5001", "The Fell Wood", {344, 98}, 9, 2, {{North, 2, "FR5001", 1, 5, 1}, {North, 7, "FR5102", 7, 5, 1}}},
{"AR5102", "River Caves Exit", {477, 49}, 12, 2, {}},
{"AR5005", "River Caves", {483, 100}, 11, 2, {{North, 3, "FR5005", 1, 5, 1}, {North, 2, "FR5001", 4, 5, 1}}},
{"AR5200", "Black Raven Monastery", {446, 5}, 13, 2, {{North, 2, "FR5001", 6, 5, 1}, {North, 5, "FR5200", 1, 5, 1}}},
{"AR5300", "The Underdark", {541, 34}, 14, 2, {}},
{"", "Лускан", {203, 299}, 23, 3, {}},
{"", "Лес Невервинтер", {475, 320}, 23, 3, {}},
};
static worldmap::area world_areas_2[] = {{"AR6000", "Kuldahar Valley", {263, 104}, 20, 2, {}},
{"AR6001", "Кулдахар", {322, 87}, 15, 2, {{North, 3, "FRWMap", 4, 5, 1}, {North, 5, "FRWMap", 6, 5, 1}, {North, 6, "FRWMap", 6, 5, 1}, {North, 0, "FR6001a", 0, 5, 1}}},
{"AR6104", "Dragon's Eye Exit", {496, 176}, 16, 2, {{North, 5, "FRWMap", 2, 5, 1}, {North, 1, "FRWMapE", 4, 5, 1}}},
{"AR6100", "Dragon's Eye", {439, 118}, 16, 2, {{North, 1, "FRWMapE", 4, 5, 1}}},
{"AR6201", "Fields of Slaughter", {481, 357}, 18, 2, {{North, 6, "FRWMap", 2, 5, 1}, {North, 2, "FRWMap", 2, 5, 1}, {North, 1, "FRWMapE", 6, 5, 1}}},
{"AR6200", "Fields of Slaughter", {522, 323}, 17, 2, {{North, 2, "", 2, 5, 1}, {North, 1, "FRWMapE", 6, 5, 1}}},
{"AR6300", "Severed Hand", {391, 345}, 19, 2, {{North, 2, "FRWMap", 6, 5, 1}, {North, 1, "FRWMapE", 6, 5, 1}, {South, 4, "FR6300", 2, 5, 2}}},
{"", "", {590, 302}, 21, 0, {}},
{"", "", {590, 357}, 21, 0, {}},
};
worldmap world[] = {{"WMAP1", "Долина ледяного ветра", "MAPICONS", world_areas_0},
{"WMAP2", "Западный проход", "MAPICONS", world_areas_1},
{"WMAP3", "Кулдахар", "MAPICONS", world_areas_2}};
static worldmap::area* current_area;

worldmap::area* worldmap::getarea() {
	return current_area;
}

void worldmap::set(area* pa) {
	if(!pa)
		return;
	auto pw = getworld(pa);
	if(!pw)
		return;
	pa->set(AreaVisible);
	for(auto& e : pa->links) {
		if(pw->areas[e.index].is(AreaVisibleFromAdjanced))
			pw->areas[e.index].set(AreaVisible);
	}
	if(current_area != pa) {
		auto pl = pw->find(current_area, pa);
		if(pl) {
			if(pl->time)
				draw::mslog("Путишествие заняло [+%1i] часов", pl->time * 4);
		}
		current_area = pa;
	}
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

worldmap::link* worldmap::find(const area* start, const area* dest) const {
	if(!start)
		return false;
	for(auto& e : start->links) {
		auto& a = areas[e.index];
		if(dest == &a)
			return const_cast<link*>(&e);
	}
	return 0;
}

aref<worldmap> getworldmaps() {
	return aref<worldmap>(world);
}