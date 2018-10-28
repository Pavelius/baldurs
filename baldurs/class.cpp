#include "main.h"

static skill_s barbarian_skills[] = {Climb, CraftArmor, CraftBow, CraftWeapon, HandleAnimal, Intimidate, Jump, Listen, Ride, Survival, Swim};
static skill_s bard_skills[] = {Appraise, Balance, Bluff, Climb, Concentration, CraftBow, DecipherScript,
Diplomacy, Disguise, EscapeArtist, GatherInformation, Hide, Jump, KnowledgeHistory, KnowledgeArcana, KnowledgeNobility,
Listen, MoveSilently, Perform, SenseMotive, SleightOfHand, SpeakLanguage, Spellcraft, Swim, Tumble, UseMagicDevice};
static skill_s cleric_skills[] = {Concentration, CraftAlchemy, CraftArmor, Diplomacy, Heal, KnowledgeArcana, KnowledgeHistory, KnowledgeReligion, Spellcraft};
static skill_s druid_skills[] = {Concentration, CraftBow, Diplomacy, HandleAnimal, Heal, KnowledgeNature, Listen, Ride, Spellcraft, Spot, Swim, Survival};
static skill_s fighter_skills[] = {Climb, CraftArmor, CraftBow, CraftWeapon, HandleAnimal, Intimidate, Jump, Ride, Swim};
static skill_s monk_skills[] = {Balance, Climb, Concentration, CraftWeapon, Diplomacy, EscapeArtist, Hide, Jump, KnowledgeArcana, KnowledgeReligion, Listen, MoveSilently, Perform, SenseMotive, Spot, Swim, Tumble};
static skill_s paladin_skills[] = {Concentration, CraftArmor, CraftWeapon, Diplomacy, HandleAnimal, Heal, KnowledgeNobility, KnowledgeReligion, Ride, SenseMotive};
static skill_s ranger_skills[] = {Climb, Concentration, CraftBow, CraftWeapon, HandleAnimal, Heal, Hide, Jump, KnowledgeNature, Listen, MoveSilently, Ride, Search, Spot, Survival, Swim, UseRope};
static skill_s rogue_skills[] = {Appraise, Balance, Bluff, Climb, CraftDevice, DecipherScript, Diplomacy, DisableDevice, Disguise, EscapeArtist, Forgery, GatherInformation, Hide, Intimidate, Jump, KnowledgeHistory, Listen, MoveSilently, OpenLock, Perform, Search, SenseMotive, SleightOfHand, Spot, Swim, Tumble, UseMagicDevice, UseRope};
static skill_s sorcerer_skills[] = {Bluff, Concentration, CraftAlchemy, CraftDevice, KnowledgeArcana, Spellcraft};
static skill_s wizard_skills[] = {Concentration, CraftAlchemy, CraftDevice, CraftWeapon, DecipherScript, KnowledgeArcana, KnowledgeHistory, KnowledgeNature, KnowledgeNobility, KnowledgeReligion, Spellcraft};
static feat_s martial_weapons[] = {ProficiencyAxe, ProficiencyClub, ProficiencyCrossbow,
ProficiencyDagger, ProficiencyGreatweapon, ProficiencyHeavyCrossbow,
ProficiencyMace, ProficiencySimple, ProficiencySpear,
ProficiencyLongbow, ProficiencyLongsword,
ProficiencyScimitar, ProficiencyShortbow, ProficiencyShortsword};
static feat_s simple_weapons[] = {ProficiencyClub, ProficiencyCrossbow,
ProficiencyDagger, ProficiencyHeavyCrossbow,
ProficiencyMace, ProficiencySimple};
static feat_s druid_weapons[] = {ProficiencyClub, ProficiencyDagger, ProficiencySimple, ProficiencyScimitar, ProficiencySpear};
static feat_s monk_weapons[] = {ProficiencyCrossbow, ProficiencyClub, ProficiencySimple, ProficiencyDagger, ProficiencyHeavyCrossbow};
static feat_s simple_weapons_sword[] = {ProficiencyClub, ProficiencyCrossbow,
ProficiencyDagger, ProficiencyHeavyCrossbow,
ProficiencyMace, ProficiencySimple, ProficiencyShortsword};
static feat_s wizard_weapons[] = {ProficiencyClub, ProficiencyDagger, ProficiencyCrossbow, ProficiencyHeavyCrossbow, ProficiencySimple};
static feat_s all_armor[] = {ArmorProfeciencyLight, ArmorProfeciencyMedium, ArmorProfeciencyHeavy, ShieldProfeciency};
static feat_s medium_armor_and_shield[] = {ArmorProfeciencyLight, ArmorProfeciencyMedium, ShieldProfeciency};
static feat_s light_armor_and_shield[] = {ArmorProfeciencyLight, ShieldProfeciency};
static feat_s light_armor[] = {ArmorProfeciencyLight};
static feat_s rogue_weapons[] = {ProficiencyCrossbow, ProficiencySimple, ProficiencyDagger, ProficiencyClub, ProficiencyShortbow, ProficiencyShortsword, ProficiencyMace, ProficiencyHeavyCrossbow};

static char p1[] = {1, 2, 2, 3, 3, 3, 4};
static char p2[] = {1, 2, 2, 2, 3, 3, 4};
static char p3[] = {1, 2, 2, 3, 3, 4};
static char p4[] = {1, 2, 3, 4};
static char c1[] = {1, 2, 2, 3, 3, 3, 4, 4, 4, 5};
static char c2[] = {1, 2, 2, 3, 3, 3, 4, 4, 4, 5};
static char c3[] = {1, 2, 2, 3, 3, 4};
static class_info::slot_info wizard_spells[9] = {{1, p1}, {3, p1}, {5, p2},
{7, p2}, {9, p2}, {11, p1},
{13, p1}, {15, p3}, {17, p4}};
static class_info::slot_info cleric_spells[9] = {{1, c1}, {3, c1}, {5, c1},
{7, c1}, {9, c1}, {11, c1},
{13, c1}, {15, c2}, {17, c3}};

class_info class_data[] = {{"Commoner", "Крестьянин", 4, 2, 0, Strenght},
{"Adept", "Адэпт", 4, 2, wizard_spells, Intellegence},
{"Aristocrat", "Аристократ", 6, 2, 0, Strenght},
{"Beast", "Бестия", 8, 2, 0, Strenght},
{"Monster", "Монстер", 10, 2, 0, Strenght},
{"Undead", "Мертвец", 12, 2, 0, Strenght},
{"Warrior", "Воин", 6, 2, 0, Strenght, fighter_skills, martial_weapons, all_armor},
//
{"Barbarian", "Варвар", 12, 4, 0, Strenght, barbarian_skills, martial_weapons, medium_armor_and_shield},
{"Bard", "Бард", 6, 4, 0, Charisma, bard_skills, simple_weapons_sword, medium_armor_and_shield},
{"Cleric", "Клерик", 8, 2, cleric_spells, Wisdow, cleric_skills, simple_weapons, all_armor},
{"Druid", "Друид", 6, 4, cleric_spells, Wisdow, druid_skills, druid_weapons, medium_armor_and_shield},
{"Fighter", "Боец", 10, 2, 0, Strenght, fighter_skills, martial_weapons, all_armor},
{"Monk", "Монах", 6, 4, 0, Strenght, monk_skills, monk_weapons},
{"Paladin", "Паладин", 10, 2, 0, Wisdow, paladin_skills, martial_weapons, all_armor},
{"Ranger", "Следопыт", 10, 4, 0, Wisdow, ranger_skills, martial_weapons, light_armor_and_shield},
{"Rogue", "Плут", 6, 8, 0, Strenght, rogue_skills, rogue_weapons, light_armor},
{"Sorcerer", "Колдун", 4, 2, 0, Charisma, sorcerer_skills, simple_weapons},
{"Wizard", "Волшебник", 4, 2, wizard_spells, Intellegence, wizard_skills, wizard_weapons},
};
assert_enum(class, Wizard);
getstr_enum(class);