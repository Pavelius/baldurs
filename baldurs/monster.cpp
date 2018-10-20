#include "main.h"

using namespace res;

monster_info monster_data[] = {{"Персонаж", CID1},
{"Гоблин", MID1, {MGO1}, NeutralEvil, {Warrior, 1}, {8, 13, 11, 10, 11, 8}},
{"Гиганский скелет", MID1, {MSKA, MSKAA}, ChaoticEvil, {Undead, 5}},
};
assert_enum(monster, LastMonster);
getstr_enum(monster);