#include "main.h"

BSDATA(abilityi) = {
	{"Strenght", Strenght},
	{"Dexterity", Dexterity},
	{"Constitution", Constitution},
	{"Intellegence", Intellegence},
	{"Wisdow", Wisdow},
	{"Charisma", Charisma},
	{"ArmorClass", ArmorClass},
	{"Attack", Attack},
	{"Damage", Damage},
	{"MeleeAttack", MeleeAttack},
	{"MeleeDamage", MeleeDamage},
	{"RangeAttack", RangeAttack},
	{"RangeDamage", RangeDamage},
	{"DeflectCritical", DeflectCritical},
	{"Fortitude", Constitution, {Barbarian, Cleric, Druid, Fighter, Monk, Paladin, Ranger}},
	{"Reflexes", Dexterity, {Monk, Ranger, Rogue}},
	{"Will", Wisdow, {Bard, Cleric, Druid, Monk, Sorcerer, Wizard}},
	{"HitPoints", HitPoints},
	{"Movement", Movement},
};
assert_enum(abilityi, Movement)

bool creature::isgood(class_s id, ability_s value) {
	return bsdata<abilityi>::elements[value].classes.is(id);
}