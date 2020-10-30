#include "main.h"

using namespace res;

BSDATA(monsteri) = {{"Персонаж", CID1},
{"Гоблин", MID1, {MGO1}, NeutralEvil, {Warrior, 1}, {8, 13, 11, 10, 11, 8}, {HandAxe}},
{"Гиганский скелет", MID1, {MSKA, MSKAA}, ChaoticEvil, {Undead, 2}, {14, 12, 10, 10, 10, 11}, {BattleAxe}, {}, {ImprovedInitiative}},
};
assert_enum(monsteri, LastMonster)

void creature::create(monster_s type, reaction_s reaction) {
	auto& ei = bsdata<monsteri>::elements[type];
	clear();
	this->kind = type;
	this->gender = Male;
	this->race = Human;
	this->reaction = reaction;
	for(const auto& e : ei.classes) {
		if(!e.type)
			continue;
		apply(e.type);
		apply(e.type, e.level, false);
	}
	for(auto a = Strenght; a <= Charisma; a = (ability_s)(a + 1))
		ability[a] = ei.ability[a];
	for(auto e : ei.equipment) {
		if(!e)
			continue;
		item it = e;
		auto ft = it.getfeat();
		set(ft);
		add(it);
	}
	this->name = ei.name;
	finish();
}