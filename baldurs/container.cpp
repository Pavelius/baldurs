#include "main.h"

adat<container, 128>	container_data;

void container::add(item value) const {
	auto p = new itemcont(value);
	p->object = (container*)this;
}

int container::getframe() const {
	switch(type) {
	case Altar: return 0;
	case Barrel: return 3;
	case Body: return 4;
	case Spellbook: return 5;
	case Shelf: return 6;
	case Chest: return 7;
	case Crate: return 8;
	case Table: return 13;
	case Nonvisible: return 11;
	default: return 1;
	}
}

template<> void archive::set<container>(container& e) {
	set(e.type);
	set(e.name);
	set(e.launch);
	set(e.box);
	set(e.points);
}