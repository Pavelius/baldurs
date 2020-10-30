#include "main.h"

BSDATA(spelli) = {{"NoSpell", "SPWI104C"},
//
{"ArmorOfFaith", "SPPR115C", Abjuration, {{Cleric, 1}}},
{"Bane", "SPPR111C", Enchantment, {{Cleric, 1}}},
{"Bless", "SPPR101C", Conjuration, {{Cleric, 1}}, Range50, Minute1p},
{"Command", "SPPR102C", Enchantment, {{Cleric, 1}}},
{"CureLightWounds", "SPPR103C", Conjuration, {{Cleric, 1}}},
{"Doom", "SPPR114C", Enchantment, {{Cleric, 1}}},
{"Entangle", "SPPR105C", Transmutation, {{Cleric, 1}}},
{"FaerieFire", "SPPR116C", Evocation, {{Cleric, 1}}},
{"FlameStrike", "SPPR503C", Evocation, {{Cleric, 5}}},
{"FrostFingers", "SPPR117C", Transmutation, {{Cleric, 1}}},
{"GlyphOfWarding", "SPPR304C", Abjuration, {{Cleric, 3}}},
{"HoldPerson", "SPPR208C", Enchantment, {{Cleric, 2}}},
{"InflictLightWounds", "SPPR112C", Necromancy, {{Cleric, 1}}},
{"MagicStone", "SPPR106C", Transmutation, {{Cleric, 1}}},
{"ProtectionFromEvil", "SPPR107C", Abjuration, {{Cleric, 1}}},
{"RemoveFear", "SPPR108C", Abjuration, {{Cleric, 1}}},
{"RemoveParalysis", "SPPR308C", Abjuration, {{Cleric, 3}}},
{"Sanctuary", "SPPR109C", Abjuration, {{Cleric, 1}}},
{"Shillelagh", "SPPR110C", Transmutation, {{Druid, 1}}},
{"Sunscorch", "SPPR113C", Evocation, {{Cleric, 1}}},
{"Antichickenator", "SPPR302C", Transmutation, {{Sorcerer, 1}, {Wizard, 1}}},
{"BurningHands", "SPWI103C", Transmutation, {{Sorcerer, 1}, {Wizard, 1}}},
{"CharmPerson", "SPWI104C", Enchantment, {{Sorcerer, 1}, {Wizard, 1}}},
{"ChillTouch", "SPWI117C", Necromancy, {{Sorcerer, 1}, {Wizard, 1}}},
{"ChromaticOrb", "SPWI118C", Evocation, {{Sorcerer, 1}, {Wizard, 1}}},
{"ColorSpray", "SPWI105C", Transmutation, {{Sorcerer, 1}, {Wizard, 1}}},
{"DispelMagic", "SPwi302C", Abjuration, {{Sorcerer, 3}, {Wizard, 3}}},
{"EaglesSplendor", "SPWI107C", Enchantment, {{Sorcerer, 1}, {Wizard, 1}}},
{"GenericAbjuration", "SPWI112C", Evocation, {{Sorcerer, 1}, {Wizard, 1}}},
{"Grease", "SPWI101C", Conjuration, {{Sorcerer, 1}, {Wizard, 1}}},
{"IceDagger", "SPWI122C", Evocation, {{Sorcerer, 1}, {Wizard, 1}}},
{"Identify", "SPWI110C", Divination, {{Sorcerer, 1}, {Wizard, 1}}},
{"LarlochsMinorDrain", "SPWI119C", Necromancy, {{Sorcerer, 1}, {Wizard, 1}}},
{"MageArmor", "SPWI102C", Conjuration, {{Sorcerer, 1}, {Wizard, 1}}, Touch, Hour1p},
{"MagicMissile", "SPWI112C", Evocation, {{Sorcerer, 1}, {Wizard, 1}}},
{"MinorMirrorImage", "SPWI120C", Illusion, {{Sorcerer, 1}, {Wizard, 1}}},
{"ProtectionFromPetrification", "SPWI108C", Abjuration, {{Sorcerer, 1}, {Wizard, 1}}},
{"Shield", "SPWI114C", Abjuration, {{Sorcerer, 1}, {Wizard, 1}}},
{"ShockingGrasp", "SPWI115C", Transmutation, {{Sorcerer, 1}, {Wizard, 1}}},
{"Sleep", "SPWI116C", Enchantment, {{Sorcerer, 1}, {Wizard, 1}}},
{"Spook", "SPWI121C", Illusion, {{Sorcerer, 1}, {Wizard, 1}}},
{"SummonMonsterI", "SPWI124C", Conjuration, {{Sorcerer, 1}, {Wizard, 1}}},
};
assert_enum(spelli, LastSpell)

bool creature::is(spell_s id, class_s cls, int level) {
	return bsdata<spelli>::elements[id].levels[cls] >= level;
}

bool creature::affect(spell_s id, int level, bool run) {
	switch(id) {
	case Bless:
		if(run) {
		}
		break;
	}
	return true;
}