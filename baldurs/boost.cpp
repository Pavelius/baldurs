#include "main.h"

BSDATAC(boosti, 1024)

boosti*	creature::find(variant id) const {
	auto owner_id = getid();
	for(auto& e : bsdata<boosti>()) {
		if(e.owner_id == owner_id && e.id == id)
			return &e;
	}
	return 0;
}

boosti*	creature::finds(variant source) const {
	auto owner_id = getid();
	for(auto& e : bsdata<boosti>()) {
		if(e.owner_id == owner_id && e.source == source)
			return &e;
	}
	return 0;
}

int	creature::getboost(variant id) const {
	auto owner_id = getid();
	auto result = 0;
	for(auto& e : bsdata<boosti>()) {
		if(e.owner_id == owner_id && e.id == id)
			result += e.bonus;
	}
	return result;
}

void creature::recall(variant id, variant source, int bonus, unsigned rounds) {
	auto p = bsdata<boosti>::add();
	p->id = id;
	p->source = source;
	p->bonus = bonus;
	p->time = rounds;
	p->owner_id = getid();
}

void creature::dispell() {
	auto id = getid();
	auto ps = bsdata<boosti>::elements;
	for(auto& e : bsdata<boosti>()) {
		if(e.owner_id == id) {
			addvar(e.id, e.bonus);
			continue;
		}
		*ps++ = e;
	}
	bsdata<boosti>::source.setcount(ps - bsdata<boosti>::elements);
}