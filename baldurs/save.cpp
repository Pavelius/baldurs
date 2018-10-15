#include "main.h"

static struct save_info {
	const char*			id;
	const char*			name;
	ability_s			ability;
	cflags<class_s>		classes;
} save_data[] = {{"Fortitude", "Стойкость", Constitution, {Barbarian, Cleric, Druid, Fighter, Monk, Paladin, Ranger}},
{"Reflexes", "Рефлексы", Dexterity, {Bard, Monk, Rogue}},
{"Will", "Воля", Wisdow, {Bard, Cleric, Druid, Monk, Sorcerer, Wizard}},
};
assert_enum(save, Will);
getstr_enum(save);

bool creature::isgood(class_s id, save_s value) {
	return save_data[value].classes.is(id);
}

ability_s creature::getability(save_s id) {
	return save_data[id].ability;
}