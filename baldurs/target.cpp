#include "main.h"

target::target(struct drawable* value) {
	if(door_data.consist(value)) {
		type = Door;
		door = static_cast<struct door*>(value);
	} else if(region_data.consist(value)) {
		type = Region;
		region = static_cast<struct region*>(value);
	} else if(container_data.consist(value)) {
		type = Container;
		container = static_cast<struct container*>(value);
	} else if(itemground_data.consist(value)) {
		type = ItemGround;
		itemground = static_cast<struct itemground*>(value);
	} else if(creature_data.consist(value)
		|| value>=players && value<=(players+sizeof(players)/ sizeof(players[0]))) {
		type = Creature;
		creature = static_cast<struct creature*>(value);
	} else {
		type = NoTarget;
		creature = 0;
	}
}