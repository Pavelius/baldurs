#include "main.h"

BSDATAC(entrance, 128)

template<> void archive::set<entrance>(entrance& e) {
	set(e.name);
	set(e.orientation);
	set(e.position);
}

point map::getentrance(const char* name, unsigned char* orient) {
	if(orient)
		*orient = 0;
	for(auto& e : bsdata<entrance>()) {
		if(!e.name[0])
			break;
		if(strcmp(e.name, name) == 0) {
			if(orient)
				*orient = e.orientation;
			return e.position;
		}
	}
	if(name[0] == 0 && bsdata<entrance>::elements[0].name[0])
		return bsdata<entrance>::elements[0].position;
	return {0, 0};
}

entrance* map::find(const char* id) {
	if(!bsdata<entrance>::source.getcount())
		return 0;
	if(!id || id[0] == 0)
		return bsdata<entrance>::elements;
	for(auto& e : bsdata<entrance>()) {
		if(strcmp(e.name, id) == 0)
			return &e;
	}
	return 0;
}