#include "main.h"

struct gender_info {
	const char*			id;
	const char*			name;
} gender_data[] = {{"Transgender", "Транссексуал"},
{"Male", "Мужчина"},
{"Female", "Женщина"},
};
assert_enum(gender, Female);
getstr_enum(gender);