#include "main.h"

adat<entrance> entrance_data;

template<> void archive::set<entrance>(entrance& e) {
	set(e.name);
	set(e.orientation);
	set(e.position);
}

point map::getentrance(const char* name, unsigned char* orient) {
	if(orient)
		*orient = 0;
	for(auto& e : entrance_data) {
		if(!e.name[0])
			break;
		if(strcmp(e.name, name) == 0) {
			if(orient)
				*orient = e.orientation;
			return e.position;
		}
	}
	if(name[0] == 0 && entrance_data[0].name[0])
		return entrance_data[0].position;
	return {0, 0};
}

entrance* map::find(const char* id) {
	if(!entrance_data)
		return 0;
	if(!id || id[0]==0)
		return entrance_data.data;
	for(auto& e : entrance_data) {
		if(strcmp(e.name, id) == 0)
			return &e;
	}
	return 0;
}