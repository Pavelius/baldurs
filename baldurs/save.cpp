#include "main.h"

BSDATA(savei) = {{"Fortitude", "Стойкость", Constitution, {Barbarian, Cleric, Druid, Fighter, Monk, Paladin, Ranger}},
{"Reflexes", "Рефлексы", Dexterity, {Bard, Monk, Rogue}},
{"Will", "Воля", Wisdow, {Bard, Cleric, Druid, Monk, Sorcerer, Wizard}},
};
assert_enum(savei, Will)

bool creature::isgood(class_s id, save_s value) {
	return bsdata<savei>::elements[value].classes.is(id);
}