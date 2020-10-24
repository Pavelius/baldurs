#include "main.h"

BSDATAC(itemcont, 2048)

itemcont::itemcont(const item& value) : item(value) {}

void* itemcont::operator new(unsigned size) {
	for(auto& e : bsdata<itemcont>()) {
		if(!e)
			return &e;
	}
	return bsdata<itemcont>::add();
}