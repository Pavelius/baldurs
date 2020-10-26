#include "main.h"

int dicei::roll() const {
	if(!d)
		return c;
	auto result = 0;
	for(auto i = 0; i < c; i++)
		result += 1 + (rand() % d);
	return result + b;
}