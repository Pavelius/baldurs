#include "main.h"

struct feature_info {
	variant			type;
	char			level;
	const char*		name;
	void(*proc)(creature& player, feature_info& f, bool interactive);
	aref<variant>	elements;
};

static void set_spells(creature& player, class_s type, char level) {
	for(auto e = FirstSpell; e <= LastSpell; e = (spell_s)(e + 1)) {
		if(spell_data[e].levels[type] == level)
			player.set(e);
	}
}

static spell_s random_spell(creature& player, class_s type, char level) {
	adat<spell_s, 128> elements;
	for(auto e = FirstSpell; e <= LastSpell; e = (spell_s)(e + 1)) {
		if(player.isknown(e))
			continue;
		if(spell_data[e].levels[type] == level)
			elements.add(e);
	}
	if(!elements)
		return NoSpell;
	return elements.data[rand() % elements.count];
}

static void known_some_spells(creature& player, feature_info& f, bool interactive) {
	if(interactive) {

	} else {
		for(auto i = 0; i < 4; i++) {
			auto e = random_spell(player, f.type.clas, f.level);
			if(e)
				player.setknown(e);
		}
	}
}

static void known_all_spells(creature& player, feature_info& f, bool interactive) {
	set_spells(player, f.type.clas, 1 + f.level / 2);
}

static void equipment(creature& player, feature_info& f, bool interactive) {
	for(auto e : f.elements) {
		switch(e.type) {
		case Item: player.add(e.item); break;
		}
	}
}

static variant fighter_equipment[] = {Longsword, LeatherArmor};

feature_info feature_data[] = {{Cleric, 1, 0, known_all_spells},
{Cleric, 3, 0, known_all_spells},
{Cleric, 5, 0, known_all_spells},
{Cleric, 7, 0, known_all_spells},
{Cleric, 9, 0, known_all_spells},
{Cleric, 11, 0, known_all_spells},
{Cleric, 13, 0, known_all_spells},
{Cleric, 15, 0, known_all_spells},
{Cleric, 17, 0, known_all_spells},
{Wizard, 1, 0, known_some_spells},
{Fighter, 1, 0, equipment, fighter_equipment},
};

void creature::apply(variant type, char level, bool interactive) {
	for(auto& e : feature_data) {
		if(e.type == type && e.level == level)
			e.proc(*this, e, interactive);
	}
}