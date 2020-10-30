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

void varianta::shuffle() {
	zshuffle(data, count);
}

void varianta::creatures(variant v1, bool ispresent) {
	auto pb = data;
	for(auto& e : bsdata<creature>()) {
		if(!e)
			continue;
		if(e.have(v1) != ispresent)
			continue;
		add(variant(&e));
	}
}

void varianta::match(const creature& player, bool ispresent) {
	auto pb = data;
	for(auto e : *this) {
		if(player.have(e) != ispresent)
			continue;
		*pb++ = e;
	}
	count = pb - data;
}

void varianta::select(const variant v1, const variant v2, const creature& player, bool required_feats) {
	auto pb = data;
	auto pe = pb + getmaximum();
	if(v1.type == Feat) {
		for(auto e = v1; e.value <= v2.value; e.value++) {
			if(!player.isallow((feat_s)e.value, required_feats))
				continue;
			if(pb < pe)
				*pb++ = e;
		}
	} else {
		for(auto e = v1; e.value <= v2.value; e.value++) {
			if(pb < pe)
				*pb++ = e;
		}
	}
	count = pb - data;
}

void varianta::select(const variant v1, const variant v2) {
	auto pb = data;
	auto pe = pb + getmaximum();
	for(auto e = v1; e.value <= v2.value; e.value++) {
		if(pb < pe)
			*pb++ = e;
	}
	count = pb - data;
}