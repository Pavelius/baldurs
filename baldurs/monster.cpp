#include "main.h"

using namespace res;

monster_info monster_data[] = {{"Персонаж", CID1},
{"Гоблин", MID1, {MGO1}, NeutralEvil, {Warrior, 1}, {8, 13, 11, 10, 11, 8}, {HandAxe}},
{"Гиганский скелет", MID1, {MSKA, MSKAA}, ChaoticEvil, {Undead, 2}, {14, 12, 10, 10, 10, 11}, {BattleAxe}, {}, {ImprovedInitiative}},
};
assert_enum(monster, LastMonster);
getstr_enum(monster);

void creature::create(monster_s type, reaction_s reaction) {
	clear();
	this->kind = type;
	this->gender = Male;
	this->race = Human;
	this->reaction = reaction;
	for(const auto& e : monster_data[type].classes) {
		if(!e.type)
			continue;
		apply(e.type);
		apply(e.type, e.level, false);
	}
	for(auto a = Strenght; a <= Charisma; a = (ability_s)(a + 1))
		ability[a] = monster_data[type].ability[a];
	for(auto e : monster_data[type].equipment) {
		if(!e)
			continue;
		item it = e;
		auto ft = it.getfeat();
		set(ft);
		add(it);
	}
	this->name = monster_data[type].name;
	update_levels();
}