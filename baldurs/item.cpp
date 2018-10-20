#include "main.h"

static_assert(sizeof(item) == 4, "Struct 'item_t' can be only 'int' sized");
struct item_info {
	struct attack_info {
		dice		damage;
		char		critical;
		char		multiplier;
		char		ac;
	};
	const char*		name;
	const char*		image_file;
	slot_s			slots[2];
	feat_s			feat[2];
	attack_info		ai;
};

static item_info item_data[] = {{"No item", "IHANDGF"},
{"Club", "ICLUBB1", {QuickWeapon, QuickOffhand}, {ProficiencyClub, FocusMaces}, {{1, 6}}},
{"Hammer", "IHAMMB1", {QuickWeapon, QuickOffhand}, {ProficiencyMace, FocusMaces}, {{1, 6}}},
{"Mace", "IMACEB1", {QuickWeapon, QuickOffhand}, {ProficiencyMace, FocusMaces}, {{1, 6, 1}}},
{"Spear", "ISPERB1", {QuickWeapon}, {ProficiencySpear, FocusPolearm}, {{1, 8}}},
{"Staff", "ISTAFB1", {QuickWeapon}, {ProficiencySimple, FocusPolearm}, {{1, 6}}},
{"Crossbow", "IXBOWL01", {QuickWeapon}, {ProficiencyCrossbow, FocusCrossbows}, {{1, 8}}},
{"Heavy crossbow", "IXBOWH01", {QuickWeapon}, {ProficiencyHeavyCrossbow, FocusCrossbows}, {{1, 10}}},
{"Sling", "ISLNGB1", {QuickWeapon}, {ProficiencySimple}, {{1, 4}}},
//
{"Battle axe", "IAX1HB2", {QuickWeapon}, {ProficiencyAxe, FocusAxes}, {{1, 8}}},
{"Dagger", "IDAGGB1", {QuickWeapon, QuickOffhand}, {ProficiencyDagger, FocusDaggers}, {{1, 4}}},
{"Greataxe", "IAX2HB1", {QuickWeapon}, {ProficiencyGreatweapon, FocusAxes}, {{1, 12}}},
{"Halberd", "IHALBB1", {QuickWeapon}, {ProficiencyGreatweapon, FocusPolearm}, {{1, 10}}},
{"Scimitar", "ISWDCB2", {QuickWeapon, QuickOffhand}, {ProficiencyScimitar, FocusShortswords}, {{1, 6}}},
{"Long sword", "ISWDLB1", {QuickWeapon}, {ProficiencyLongsword, FocusLongswords}, {{1, 8}}},
{"Short sword", "ISWDSB1", {QuickWeapon, QuickOffhand}, {ProficiencyShortsword, FocusShortswords}, {{1, 6}}},
{"Two handed sword", "ISWDTB1", {QuickWeapon}, {ProficiencyGreatweapon, FocusGreatswords}, {{2, 6}}},
{"Rapier", "ISWDSB1", {QuickWeapon}, {ProficiencyShortsword, FocusShortswords}, {{1, 6}}}, // TODO: найти лучше вариант
{"Short bow", "IBOWSB1", {QuickWeapon}, {ProficiencyShortbow, FocusBows}, {{1, 6}}},
{"Long bow", "IBOWLB1", {QuickWeapon}, {ProficiencyLongbow, FocusBows}, {{1, 8}}},
//
{"Waraxe", "IAX1HBB", {QuickWeapon}, {ProficiencyWaraxe, FocusAxes}, {{1, 10}}},
{"Bastard sword", "ISWDBB1", {QuickWeapon}, {ProficiencyBastardsword, FocusLongswords}, {{1, 10}}},
{"Katana", "ISWDBB1", {QuickWeapon}, {ProficiencyCatana, FocusGreatswords}, {{1, 10}}}, // TODO: найти лучше вариант
//
{"Leather armor", "IARMLB1", {Body}, {ArmorProfeciencyLight}, {{}, 0, 0, 2}},
{"Studded leather armor", "IARMSB1", {Body}, {ArmorProfeciencyLight}, {{}, 0, 0, 3}},
{"Chain shirt", "IARMCM1", {Body}, {ArmorProfeciencyMedium}, {{}, 0, 0, 4}},
{"Scale mail", "IARMSM1", {Body}, {ArmorProfeciencyMedium}, {{}, 0, 0, 4}},
{"Chainmail", "IARMCB1", {Body}, {ArmorProfeciencyMedium}, {{}, 0, 0, 5}},
{"Breastplate", "IARMHM3", {Body}, {ArmorProfeciencyMedium}, {{}, 0, 0, 5}},
{"Splint mail", "IARMPB1", {Body}, {ArmorProfeciencyHeavy}, {{}, 0, 0, 6}},
{"Banded mail", "IARMPM1", {Body}, {ArmorProfeciencyHeavy}, {{}, 0, 0, 7}},
{"Plate mail", "IARMFM1", {Body}, {ArmorProfeciencyHeavy}, {{}, 0, 0, 8}},
//
{"Shiled small", "ISHDSB1", {QuickOffhand}, {ShieldProfeciency}, {{}, 0, 0, 2}},
{"Large shield", "ISHDLB1", {QuickOffhand}, {ShieldProfeciency}, {{}, 0, 0, 2}},
{"Tower shield", "ISHDTB1", {QuickOffhand}, {ShieldProfeciency}, {{}, 0, 0, 2}},
//
{"Helm", "IHELMB1", {Head}, {ArmorProfeciencyMedium}},
};
assert_enum(item, Helm);
getstr_enum(item);

void item::clear() {
	memset(this, 0, sizeof(*this));
}

int	item::getbonus() const {
	switch(magic) {
	case Cursed: return quality - 2; break;
	case Magical: return quality + 1; break;
	case Artifact: return quality + 2; break;
	default: return quality; break;
	}
}

bool item::is(slot_s value) const {
	for(auto s : item_data[type].slots) {
		if(s == value)
			return true;
	}
	return false;
}

bool item::istwohand() const {
	switch(type) {
	case Staff:
	case Greataxe:
		return true;
	default:
		return false;
	}
}

bool item::is(feat_s value) const {
	return item_data[type].feat[0] == value
		|| item_data[type].feat[1] == value;
}

int	item::getac() const {
	auto r = item_data[type].ai.ac;
	if(r)
		r += getbonus();
	return r;
}

int item::getframe() const {
	return type * 2;
}

feat_s item::getfeat() const {
	return item_data[type].feat[0];
}

const char* item::getfname(int type) {
	return item_data[type].image_file;
}

int item::getarmorindex() const {
	switch(item_data[type].feat[0]) {
	case ArmorProfeciencyLight: return 1;
	case ArmorProfeciencyMedium: return 2;
	case ArmorProfeciencyHeavy: return 3;
	default: return 0;
	}
}

bool creature::isallow(const item& it) const {
	const auto& ei = item_data[it.gettype()];
	if(!ei.feat[0])
		return true;
	for(auto e : ei.feat) {
		if(!e)
			break;
		if(is(e))
			return true;
	}
	return false;
}

bool item::isbow() const {
	return type == Longbow || type == Shortbow;
}

bool item::isxbow() const {
	return type == CrossbowLight || type == CrossbowLight;
}

bool item::isthown() const {
	return type == Sling;
}