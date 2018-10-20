#include "main.h"

using namespace res;

monster_info monster_data[] = {{"Персонаж"},
{"Гоблин", {MGO1}},
{"Гиганский скелет", {MSKA, MSKAA}},
};
assert_enum(monster, LastMonster);
getstr_enum(monster);