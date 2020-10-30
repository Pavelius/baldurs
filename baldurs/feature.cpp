#include "main.h"

struct featurei {
	typedef void(*applyp)(creature& player, featurei& f, bool interactive);
	variant			type;
	char			level;
	const char*		name;
	applyp			proc;
	std::initializer_list<variant> elements;
};

static void set_spells(creature& player, class_s type, char level) {
	for(auto e = FirstSpell; e <= LastSpell; e = (spell_s)(e + 1)) {
		if(bsdata<spelli>::elements[e].levels[type] == level)
			player.set(e);
	}
}

static spell_s random_spell(creature& player, class_s type, char level) {
	adat<spell_s, 128> elements;
	for(auto e = FirstSpell; e <= LastSpell; e = (spell_s)(e + 1)) {
		if(player.isknown(e))
			continue;
		if(bsdata<spelli>::elements[e].levels[type] == level)
			elements.add(e);
	}
	if(!elements)
		return NoSpell;
	return elements.data[rand() % elements.count];
}

static void known_some_spells(creature& player, featurei& f, bool interactive) {
	if(interactive) {

	} else {
		for(auto i = 0; i < 4; i++) {
			auto e = random_spell(player, (class_s)f.type.value, f.level);
			if(e)
				player.setknown(e);
		}
	}
}

static void known_all_spells(creature& player, featurei& f, bool interactive) {
	set_spells(player, (class_s)f.type.value, 1 + f.level / 2);
}

static void apply_values(creature& player, featurei& f, bool interactive) {
	for(auto e : f.elements) {
		switch(e.type) {
		case Item: player.add((item_s)e.value); break;
		case Feat: player.set((feat_s)e.value); break;
			break;
		}
	}
}

featurei feature_data[] = {{Barbarian, 1, 0, apply_values, {FastMovement, Illiteracy}},
{Cleric, 1, 0, known_all_spells},
{Cleric, 1, 0, apply_values, {Mace}},
{Fighter, 1, 0, apply_values, {Longsword, LeatherArmor, ShieldLarge, Helm}},
{Paladin, 1, 0, apply_values, {Longsword, BandedMail, Helm}},
{Ranger, 1, 0, apply_values, {Shortbow, LeatherArmor}},
{Rogue, 1, 0, apply_values, {Shortsword}},
{Wizard, 1, 0, known_some_spells},
{Wizard, 1, 0, apply_values, {Staff}},
{Cleric, 3, 0, known_all_spells},
{Cleric, 5, 0, known_all_spells},
{Cleric, 7, 0, known_all_spells},
{Cleric, 9, 0, known_all_spells},
{Cleric, 11, 0, known_all_spells},
{Cleric, 13, 0, known_all_spells},
{Cleric, 15, 0, known_all_spells},
{Cleric, 17, 0, known_all_spells},
};

void creature::apply(variant type, char level, bool interactive) {
	for(auto& e : feature_data) {
		if(e.type == type && e.level == level)
			e.proc(*this, e, interactive);
	}
}