#include "main.h"

using namespace res;

monster_info monster_data[] = {{"��������"},
{"������", {MGO1}},
{"��������� ������", {MSKA, MSKAA}},
};
assert_enum(monster, LastMonster);
getstr_enum(monster);