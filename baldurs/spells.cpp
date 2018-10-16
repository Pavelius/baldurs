#include "main.h"

spell_info spell_data[] = {{"No spell", "", "SPWI104C"},
//
{"Armor of Faith", "", "SPPR115C", Abjuration, {{Cleric, 1}}},
{"Bane", "", "SPPR111C", Enchantment, {{Cleric, 1}}},
{"Bless", "", "SPPR101C", Conjuration, {{Cleric, 1}}},
{"Command", "", "SPPR102C", Enchantment, {{Cleric, 1}}},
{"Cure Light Wounds", "", "SPPR103C", Conjuration, {{Cleric, 1}}},
{"Doom", "", "SPPR114C", Enchantment, {{Cleric, 1}}},
{"Entangle", "", "SPPR105C", Transmutation, {{Cleric, 1}}},
{"Faerie Fire", "", "SPPR116C", Evocation, {{Cleric, 1}}},
{"Flame Strike", "", "SPPR503C", Evocation, {{Cleric, 1}}},
{"Frost Fingers", "", "SPPR117C", Transmutation, {{Cleric, 1}}},
{"Glyph of Warding", "", "SPPR304C", Abjuration, {{Cleric, 1}}},
{"Hold Person", "", "SPPR208C", Enchantment, {{Cleric, 1}}},
{"Inflict Light Wounds", "", "SPPR112C", Necromancy, {{Cleric, 1}}},
{"Magic Stone", "", "SPPR106C", Transmutation, {{Cleric, 1}}},
{"Protection From Evil", "", "SPPR107C", Abjuration, {{Cleric, 1}}},
{"Remove Fear", "", "SPPR108C", Abjuration, {{Cleric, 1}}},
{"Remove Paralysis", "", "SPPR308C", Abjuration, {{Cleric, 1}}},
{"Sanctuary", "", "SPPR109C", Abjuration, {{Cleric, 1}}},
{"Shillelagh", "", "SPPR110C", Transmutation, {{Cleric, 1}}},
{"Sunscorch", "", "SPPR113C", Evocation, {{Cleric, 1}}},
{"Antichickenator", "", "SPPR302C", NoSchool, {{Sorcerer, 1}, {Wizard, 1}}},
{"Burning Hands", "", "SPWI103C", Transmutation, {{Sorcerer, 1}, {Wizard, 1}}},
{"Charm Person", "", "SPWI104C", Enchantment, {{Sorcerer, 1}, {Wizard, 1}}},
{"Chill Touch", "", "SPWI117C", Necromancy, {{Sorcerer, 1}, {Wizard, 1}}},
{"Chromatic Orb", "", "SPWI118C", Evocation, {{Sorcerer, 1}, {Wizard, 1}}},
{"Color Spray", "", "SPWI105C", Transmutation, {{Sorcerer, 1}, {Wizard, 1}}},
{"Dispel Magic", "", "SPwi302C", Abjuration, {{Sorcerer, 1}, {Wizard, 1}}},
{"Eagle's Splendor", "", "SPWI107C", Enchantment, {{Sorcerer, 1}, {Wizard, 1}}},
{"Generic Abjuration", "", "SPWI112C", Evocation, {{Sorcerer, 1}, {Wizard, 1}}},
{"Grease", "", "SPWI101C", Conjuration, {{Sorcerer, 1}, {Wizard, 1}}},
{"Ice Dagger", "", "SPWI122C", Evocation, {{Sorcerer, 1}, {Wizard, 1}}},
{"Identify", "", "SPWI110C", Divination, {{Sorcerer, 1}, {Wizard, 1}}},
{"Larloch's Minor Drain", "", "SPWI119C", Necromancy, {{Sorcerer, 1}, {Wizard, 1}}},
{"Mage Armor", "", "SPWI102C", Conjuration, {{Sorcerer, 1}, {Wizard, 1}}},
{"Magic Missile", "", "SPWI112C", Evocation, {{Sorcerer, 1}, {Wizard, 1}}},
{"Minor Mirror Image", "", "SPWI120C", Illusion, {{Sorcerer, 1}, {Wizard, 1}}},
{"Protection from Petrification", "", "SPWI108C", Abjuration, {{Sorcerer, 1}, {Wizard, 1}}},
{"Shield", "", "SPWI114C", Abjuration, {{Sorcerer, 1}, {Wizard, 1}}},
{"Shocking Grasp", "", "SPWI115C", Transmutation, {{Sorcerer, 1}, {Wizard, 1}}},
{"Sleep", "", "SPWI116C", Enchantment, {{Sorcerer, 1}, {Wizard, 1}}},
{"Spook", "", "SPWI121C", Illusion, {{Sorcerer, 1}, {Wizard, 1}}},
{"Summon Monster I", "", "SPWI124C", Conjuration, {{Sorcerer, 1}, {Wizard, 1}}}, };
assert_enum(spell, LastSpell);
getstr_enum(spell);

//void get_spell_description(char* result, int rec) {
//	szprint(result, "###%1\n", "",bsgets(rec, Name));
//	const char* pb = bsgets(rec, Description);
//	// School
//	int shc = bsget(rec, SpellSchool);
//	zcat(result, bsgets(shc, Name));
//	for(int i = FirstSchoolSubtype; i <= LastSchoolSubtype; i++) {
//		int val = bsget(rec, i);
//		if(!val)
//			continue;
//		szprint(zend(result), " (%1)", "",bsgets(i, Name));
//		break;
//	}
//	zcat(result, "\n");
//	// Levels, like 'Wizard 1, Sorcerer 1'
//	int k = 0;
//	for(int i = FirstClass; i <= LastClass; i++) {
//		int val = bsget(rec, i);
//		if(!val)
//			continue;
//		if(k)
//			zcat(result, ", "",");
//		szprint(zend(result), "%1 %2i", "",bsgets(i, Name), val);
//		k++;
//	}
//	// Description
//	if(pb && pb[0]) {
//		zcat(result, pb);
//		zcat(result, "\n");
//	}
//}

bool creature::is(spell_s id, class_s cls, int level) {
	return spell_data[id].levels[cls] >= level;
}