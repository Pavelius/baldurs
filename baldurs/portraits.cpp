#include "main.h"

portrait_info portrait_data[] = {{"NOPORT"},
{"DFF", 12, 2, 41, 65, Female, {Dwarf, Halfling, Gnome}, {Fighter}},
{"DMC", 84, 2, 39, 27, Male, {Dwarf}, {Cleric, Paladin, Fighter}},
{"DMF", 84, 3, 38, 27, Male, {Dwarf}, {Fighter}},
{"DMT", 84, 1, 41, 49, Male, {Dwarf}, {Rogue}},
{"EFC", 13, 2, 47, 60, Female, {Elf}, {Cleric, Wizard, Sorcerer}},
{"EFW", 10, 2, 54, 66, Female, {Elf}, {Wizard, Sorcerer}},
{"EMF", 84, 2, 41, 26, Male, {Elf}, {Fighter, Ranger}},
{"EMF2", 12, 0, 39, 41, Male, {Elf}, {Fighter, Ranger}},
{"EMT", 12, 2, 66, 37, Male, {Elf}, {Rogue}},
{"EMW", 13, 6, 51, 49, Male, {Elf}, {Wizard, Sorcerer, Cleric}},
{"GFW", 85, 0, 40, 58, Female, {Gnome, Dwarf}, {Cleric, Sorcerer, Wizard}},
{"GMT", 12, 0, 66, 14, Male, {Gnome, Halfling}, {Rogue}},
{"GMW", 84, 2, 41, 46, Male, {Gnome, Dwarf}, {Wizard}},
{"HFF", 84, 2, 100, 98, Female, {Human}, {Fighter, Paladin}},
{"HFT", 84, 0, 39, 66, Female, {Human}, {Rogue, Monk}},
{"HFW", 12, 0, 49, 66, Female, {Human}, {Sorcerer, Cleric, Wizard}},
{"HMB", 87, 0, 41, 49, Male, {Human}, {Barbarian}},
{"HMC", 12, 0, 49, 53, Male, {Human}, {Cleric, Wizard, Sorcerer}},
{"HMF2", 87, 0, 66, 98, Male, {Human}, {Fighter, Paladin}},
{"HMF", 84, 0, 66, 27, Male, {Human}, {Fighter, Paladin}},
{"HMW", 12, 5, 58, 48, Male, {Human}, {Wizard}},
{"HaFF", 84, 2, 39, 49, Female, {Halfling, Gnome}, {Fighter}},
{"HaFT", 12, 0, 39, 14, Female, {Halfling, Gnome}, {Rogue, Bard}},
{"HaMF", 84, 2, 39, 24, Male, {Halfling, Gnome}, {Fighter, Paladin}},
{"HaMT", 84, 2, 41, 56, Male, {Halfling}, {Rogue}},
{"HeFB", 12, 2, 39, 56, Female, {HalfElf, Human}, {Bard, Rogue, Sorcerer}},
{"HeFC", 12, 3, 39, 58, Female, {HalfElf, Human}, {Cleric}},
{"HeFF", 84, 2, 41, 66, Female, {HalfElf}, {Cleric, Fighter, Paladin}},
{"HeFW", 84, 0, 41, 40, Female, {HalfElf}, {Wizard, Sorcerer}},
{"HeMF", 84, 1, 41, 98, Male, {HalfElf, Human, HalfOrc}, {Fighter, Paladin}},
{"HeMT", 85, 2, 41, 54, Male, {HalfElf, Human}, {Ranger, Rogue, Fighter}},
{"HeMW", 12, 0, 49, 41, Male, {HalfElf, Human}, {Wizard, Cleric, Sorcerer, Monk}},
{"HFW2", 13, 0, 66, 66, Female, {Human}, {Wizard, Sorcerer, Bard, Rogue}},
{"HMF3", 8, 0, 66, 103, Male, {Human, HalfElf}, {Barbarian}},
{"EFW2", 83, 0, 60, 60, Female, {Elf}, {Wizard, Sorcerer}},
{"HFW3", 8, 0, 66, 60, Female, {Human, HalfElf, Elf}, {Wizard, Sorcerer, Monk}},
{"EFF", 12, 0, 66, 25, Female, {Elf, HalfElf}, {Fighter, Paladin, Ranger}},
{"HMF4", 107, 0, 66, 102, Male, {Human, Elf, HalfOrc}, {Fighter}},
{"EMC", 83, 110, 49, 60, Male, {Elf}, {Cleric, Wizard, Sorcerer}},
{"HMW2", 12, 0, 22, 41, Male, {Human}, {Wizard, Sorcerer}},
{"HMW3", 84, 0, 46, 46, Male, {Human, HalfElf}, {Wizard, Sorcerer, Monk}},
{"HMC2", 87, 0, 63, 60, Male, {Human}, {Cleric}},
{"HMC3", 12, 79, 94, 91, Male, {Human}, {Cleric, Fighter}},
{"HMF5", 12, 3, 41, 66, Male, {Human}, {Fighter, Barbarian}},
{"HMB2", 84, 2, 39, 47, Male, {Human}, {Barbarian}},
{"DMF2", 12, 3, 66, 58, Male, {Human}, {Fighter}},
{"HFT2", 12, 2, 39, 63, Female, {Human, HalfElf}, {Rogue}},
{"HFD", 12, 2, 41, 49, Female, {Human}, {Druid, Cleric, Sorcerer}},
{"HFF2", 12, 2, 41, 37, Female, {Human}, {Fighter}},
{"HMC4", 12, 1, 39, 56, Male, {Human}, {Cleric, Wizard, Sorcerer}},
{"HMF6", 84, 0, 66, 47, Male, {Human, HalfOrc}, {Fighter, Paladin}},
{"HMF7", 8, 0, 41, 46, Male, {Human}, {Fighter}},
{"HMW4", 13, 0, 107, 66, Male, {Human}, {Wizard}},
{"2FBAR1", 84, 3, 41, 40, Female, {Human, HalfElf, Elf}, {Barbarian}},
{"2FDEF1", 0, 79, 66, 41, Female, {Elf}, {Rogue, Fighter}},
{"2FELF1", 85, 0, 39, 56, Female, {Elf}, {Fighter, Ranger, Wizard, Sorcerer}},
{"2FELF2", 13, 0, 66, 63, Female, {Elf, HalfElf}, {Wizard, Sorcerer}},
{"2FELF3", 8, 4, 51, 53, Female, {Elf, HalfElf}, {Wizard, Sorcerer, Monk, Bard}},
{"2FGNM1", 107, 79, 50, 51, Female, {Gnome, Halfling}, {Wizard, Sorcerer, Monk, Bard, Fighter}},
{"2FHUM1", 13, 0, 49, 66, Female, {Human}, {Fighter, Paladin}},
{"2FHUM2", 85, 0, 41, 40, Female, {Human}, {Fighter, Paladin}},
{"2FHUM3", 84, 2, 46, 45, Female, {Human}, {Fighter, Paladin}},
{"2FORC1", 85, 4, 39, 38, Female, {HalfOrc}, {Barbarian}},
{"2FTIF1", 14, 0, 66, 41, Female, {Human, Elf, HalfElf}, {Fighter, Rogue, Sorcerer}},
{"2MAAS1", 12, 3, 50, 39, Male, {Human, Elf, HalfElf}, {Fighter, Cleric, Sorcerer}},
{"2MDEF1", 0, 79, 66, 54, Male, {Elf, HalfElf}, {Fighter, Rogue}},
{"2MDEF2", 0, 79, 66, 56, Male, {Elf, HalfElf}, {Fighter, Barbarian}},
{"2MDWF1", 85, 0, 41, 46, Male, {Dwarf}, {Fighter, Paladin}},
{"2MELF1", 13, 6, 62, 66, Male, {Elf, HalfElf}, {Fighter, Paladin}},
{"2MGNM1", 107, 79, 37, 39, Male, {Gnome}, {Wizard, Sorcerer}},
{"2MHUM1", 10, 0, 39, 63, Male, {Human}, {Rogue, Wizard, Sorcerer, Ranger, Bard}},
{"2MHUM2", 10, 6, 49, 66, Male, {Human, HalfOrc}, {Monk, Fighter, Sorcerer}},
{"2MHUM3", 85, 0, 66, 49, Male, {Human}, {Rogue, Fighter}},
{"2MHUM4", 85, 0, 41, 66, Male, {Human}, {Fighter}},
{"2MHUM5", 10, 2, 65, 39, Male, {Human}, {Cleric, Wizard, Sorcerer, Monk}},
{"2MORC1", 112, 0, 66, 41, Male, {HalfOrc}, {Fighter, Paladin}},
{"2MPAL1", 84, 2, 41, 66, Male, {Human}, {Paladin, Cleric}},
{"2FHUM4", 85, 0, 50, 39, Female, {Human}, {Monk}},
{"2MBAR1", 85, 0, 41, 39, Male, {Human}, {Barbarian}},
{"2MDWF2", 85, 2, 39, 56, Male, {Dwarf}, {Fighter}},
{"2MHAF1", 85, 2, 66, 38, Male, {HalfOrc}, {Fighter, Ranger}},
{"2MHUM6", 85, 2, 41, 39, Male, {Human}, {Monk, Rogue}},
{"2MHUM7", 8, 0, 39, 66, Male, {HalfOrc, Human}, {Fighter}}
};

bool portrait_info::is(race_s id) const {
	for(auto e : races) {
		if(id == e)
			return true;
	}
	return false;
}

bool portrait_info::is(gender_s id) const {
	return gender==id;
}

aref<portrait_info> portrait_info::getelements() {
	return aref<portrait_info>(portrait_data);
}

//static int get_random_portrait_by_perk(int gender, int race, int cls) {
//	const auto maximum = sizeof(portrait_data) / sizeof(portrait_data[0]);
//	int data[maximum + 2];
//	int* p = data;
//	for(int rec = 0; rec <= maximum; rec++) {
//		portrait& e = portrait_data[rec];
//		if(e.gender != gender)
//			continue;
//		if(race && !has(e, race))
//			continue;
//		if(cls && !has(e, cls))
//			continue;
//		*p++ = rec;
//	}
//	*p = 0;
//	if(p == data)
//		return 0;
//	return data[rand() % (p - data)];
//}
//
//int game::generate::portrait(int gender, int race, int cls) {
//	int result = 0;
//	result = get_random_portrait_by_perk(gender, race, cls);
//	if(result)
//		return result;
//	result = get_random_portrait_by_perk(gender, race, 0);
//	if(result)
//		return result;
//	return get_random_portrait_by_perk(gender, 0, 0);
//}

//unsigned char* valid_colors(int type) {
//	static unsigned char hair[] = {6, 0, 2, 1, 4, 111, 80, 3, 81, 7, 82, 79, 0xFF};
//	static unsigned char hair_wood[] = {1, 2, 4, 7, 112, 114, 0xFF};
//	static unsigned char hair_dark[] = {79, 110, 5, 3, 0, 0xFF};
//	static unsigned char skin[] = {107, 8, 114, 9, 10, 85, 84, 12, 16, 15, 17, 108, 106, 113, 14, 13, 105, 83, 0xFF};
//	static unsigned char major[] = {41, 39, 38, 47, 54, 66, 51, 40, 100, 49, 58, 60, 22, 46, 63, 94, 107, 50, 62, 37, 65, 0xFF};
//	static unsigned char minor[] = {65, 27, 49, 60, 66, 26, 41, 37, 58, 14, 46, 98, 53, 48, 24, 56, 40, 54, 103, 25, 102, 91, 47, 63, 51, 45, 38, 39, 0xFF};
//	switch(type) {
//	case ColorSkin: return skin;
//	case ColorHair: return hair;
//	case ColorMajor: return major;
//	default: return minor;
//	}
//}