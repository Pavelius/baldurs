#include "main.h"

using namespace res;

BSDATA(monsteri) = {{"Goblin", Goblinoid, MID1, {MGO1}, NeutralEvil, {{Warrior, 1}}, {8, 13, 11, 10, 11, 8}, {HandAxe}},
{"SkeletonGiant", Human, MID1, {MSKA, MSKAA}, ChaoticEvil, {{Undead, 2}}, {14, 12, 10, 10, 10, 11}, {BattleAxe}, {}, {ImprovedInitiative}},
};
assert_enum(monsteri, LastMonster)

void creature::create(monster_s type, reaction_s reaction) {
	auto& ei = bsdata<monsteri>::elements[type];
	clear();
	setkind(type);
	setgender(Male);
	set(reaction);
	for(auto i = Commoner; i <= LastClass; i = (class_s)(i+1)) {
		if(!ei.classes[i])
			continue;
		apply(i);
		apply(i, ei.classes[i], false);
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
	name = ei.name;
	finish();
}