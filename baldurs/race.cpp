#include "main.h"

BSDATA(racei) = {{"Dwarf", {0, 0, 2, 0, 0, -2}, Fighter, {{Appraise, 2}, {CraftWeapon, 2}, {CraftArmor, 2}}, {Infravision, HateGoblinoids, Stability, SaveBonusVsPoison, SaveBonusVsSpell, Stonecunning}},
{"Elf", {0, 2, -2, 0, 0, 0}, Wizard, {{Listen, 2}, {Spot, 2}, {Search, 2}}, {ImmunityToSleepSpell, SaveBonusVsEnchantment, Infravision, ProficiencyLongbow, ProficiencyLongsword, ProficiencyShortbow, ProficiencyShortsword, FindSecretDoors}},
{"Gnome", {-2, 0, 2, 0, 0, 0}, Bard, {{Listen, 2}, {CraftAlchemy, 2}}, {Infravision}},
{"Halfelf", {}, Commoner, {{Listen, 1}, {Spot, 1}, {Search, 1}, {Diplomacy, 2}, {GatherInformation, 2}}, {Infravision, SaveBonusVsEnchantment, ImmunityToSleepSpell}},
{"Halforc", {2, 0, 0, -2, 0, -2}, Barbarian, {}, {}},
{"Halfling", {-2, 2, 0, 0, 0, 0}, Rogue, {{Climb, 2}, {Jump, 2}, {MoveSilently, 2}, {Listen, 2}}, {Lucky, SaveBonusVsFear, PreciseThrower}},
{"Human", {}, Commoner, {}, {}, 1},
};
assert_enum(racei, Human)