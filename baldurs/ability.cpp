#include "main.h"

static struct ability {
	const char*		id;
	const char*		name;
} ability_data[] = {
	{"Strenght", "Сила"},
	{"Dexterity", "Ловкость"},
	{"Constitution", "Телосложение"},
	{"Intellegence", "Интеллект"},
	{"Wisdow", "Мудрость"},
	{"Charisma", "Харизма"},
};
assert_enum(ability, Charisma);
getstr_enum(ability);