#include "main.h"

static struct ability {
	const char*		id;
	const char*		name;
} ability_data[] = {
	{"Strenght", "����"},
	{"Dexterity", "��������"},
	{"Constitution", "������������"},
	{"Intellegence", "���������"},
	{"Wisdow", "��������"},
	{"Charisma", "�������"},
};
assert_enum(ability, Charisma);
getstr_enum(ability);