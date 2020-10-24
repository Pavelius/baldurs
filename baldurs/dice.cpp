#include "crt.h"
#include "dice.h"

int dice::roll(int c, int d) {
	if(!d)
		return c;
	auto result = 0;
	for(auto i = 0; i < c; i++)
		result += 1 + (rand() % d);
	return result;
}

int dice::roll() const {
	return roll(c, d) + b;
}