#include "main.h"

adat<container, 128>	container_data;

template<> void archive::set<container>(container& e) {
	set(e.type);
	set(e.name);
	set(e.launch);
	set(e.box);
	set(e.points);
}