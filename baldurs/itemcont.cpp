#include "main.h"

adat<itemcont, 2048>	itemcont_data;

itemcont::itemcont(const item& value) : item(value) {}

void* itemcont::operator new(unsigned size) {
	for(auto& e : itemcont_data) {
		if(!e)
			return &e;
	}
	return (void*)itemcont_data.add();
}