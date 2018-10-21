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

char* dice::print(char* result, const char* result_maximum) const {
	szprints(result, result_maximum, "%1id%2i", c, d);
	if(b != 0)
		szprints(zend(result), result_maximum, "%+1i", b);
	return result;
}

char* dice::range(char* result, const char* result_maximum) const {
	szprints(result, result_maximum, "%1i-%2i", c + b, c*d + b);
	return result;
}