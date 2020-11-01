#include "main.h"

BSDATA(racei) = {{"Dwarf", {{Constitution, +2}, {Charisma, -2}, {Movement, 20}}, Fighter,
{{Appraise, 2}, {CraftWeapon, 2}, {CraftArmor, 2}}, {Infravision, HateGoblinoids, Stability, SaveBonusVsPoison, SaveBonusVsSpell, Stonecunning}},
{"Elf", {{Dexterity, +2}, {Constitution, -2}, {Movement, 30}}, Wizard,
{{Listen, 2}, {Spot, 2}, {Search, 2}}, {ImmunityToSleepSpell, SaveBonusVsEnchantment, Infravision, ProficiencyLongbow, ProficiencyLongsword, ProficiencyShortbow, ProficiencyShortsword, FindSecretDoors}},
{"Gnome", {{Strenght, -2}, {Intellegence, +2}, {Movement, 20}}, Bard,
{{Listen, 2}, {CraftAlchemy, 2}}, {Infravision}},
{"Halfelf", {{Movement, 30}}, Commoner,
{{Listen, 1}, {Spot, 1}, {Search, 1}, {Diplomacy, 2}, {GatherInformation, 2}}, {Infravision, SaveBonusVsEnchantment, ImmunityToSleepSpell}},
{"Halforc", {{Strenght, +2}, {Intellegence, -2}, {Charisma, -2}, {Movement, 30}}, Barbarian,
{}, {}},
{"Halfling", {{Strenght, -2}, {Dexterity, +2}, {Movement, 20}}, Rogue,
{{Climb, 2}, {Jump, 2}, {MoveSilently, 2}, {Listen, 2}}, {Lucky, SaveBonusVsFear, PreciseThrower}},
{"Human", {{Movement, 30}}, Commoner,
{}, {}, 1},
};
assert_enum(racei, Human)