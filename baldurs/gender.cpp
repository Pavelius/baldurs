#include "main.h"

struct gender_info {
	const char*			id;
	const char*			name;
} gender_data[] = {{"Transgender", "������������"},
{"Male", "�������"},
{"Female", "�������"},
};
assert_enum(gender, Female);
getstr_enum(gender);