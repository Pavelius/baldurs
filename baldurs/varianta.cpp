#include "main.h"

void varianta::addu(variant v1, variant v2) {
	for(auto i = v1.value; i <= v2.value; i++) {
		variant v(v1.type, i);
		if(is(v))
			continue;
		add(v);
	}
}

void varianta::addu(variant v1, variant v2, creature& e) {
	for(auto i = v1.value; i <= v2.value; i++) {
		variant v(v1.type, i);
		if(is(v))
			continue;
		if(!e.isallow(v))
			continue;
		add(v);
	}
}

static int compare(const void* v1, const void* v2) {
	auto p1 = (variant*)v1;
	auto p2 = (variant*)v2;
	auto n1 = getstr(*p1);
	auto n2 = getstr(*p2);
	return strcmp(n1, n2);
}

void varianta::sort() {
	qsort(data, count, sizeof(data[0]), compare);
}