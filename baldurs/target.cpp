#include "main.h"

target::target(struct drawable* value) {
	if(bsdata<struct door>::source.indexof(value) != -1) {
		type = Door;
		door = static_cast<struct door*>(value);
	} else if(bsdata<struct region>::source.indexof(value) != -1) {
		type = Region;
		region = static_cast<struct region*>(value);
	} else if(bsdata<struct container>::source.indexof(value) != -1) {
		type = Container;
		container = static_cast<struct container*>(value);
	} else if(bsdata<struct itemground>::source.indexof(value) != -1) {
		type = ItemGround;
		itemground = static_cast<struct itemground*>(value);
	} else if((bsdata<class creature>::source.indexof(value) != -1)
		|| value >= players && value <= (players + sizeof(players) / sizeof(players[0]))) {
		type = Creature;
		creature = static_cast<class creature*>(value);
	} else {
		type = NoTarget;
		creature = 0;
	}
}