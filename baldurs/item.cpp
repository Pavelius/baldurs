#include "main.h"

static_assert(sizeof(item) == 4, "Struct 'item_t' can be only 'int' sized");
struct item_info {
	const char*		id;
	const char*		name;
	const char*		image_file;
	const char*		ground_file;
	slot_s			slots[2];
	feat_s			feat[2];
	attack_info		ai;
	unsigned char	count;
	int				weight;
	int				cost;
};

static item_info item_data[] = {{"No item", "Нет предмета", "IHANDGF", "GGEM01"},
{"Club", "Дубина", "ICLUBB1", "GBLUN01", {QuickWeapon, QuickOffhand}, {ProficiencyClub, FocusMaces}, {{1, 6}}},
{"Hammer", "Молот", "IHAMMB1", "GHAMM01", {QuickWeapon, QuickOffhand}, {ProficiencyMace, FocusMaces}, {{1, 6}}},
{"Mace", "Булава", "IMACEB1", "GBLUN06", {QuickWeapon, QuickOffhand}, {ProficiencyMace, FocusMaces}, {{1, 6, 1}}},
{"Spear", "Копье", "ISPERB1", "GSPER01", {QuickWeapon}, {ProficiencySpear, FocusPolearm}, {{1, 8}}},
{"Staff", "Посох", "ISTAFB1", "GSTAF01", {QuickWeapon}, {ProficiencySimple, FocusPolearm}, {{1, 6}}},
{"Crossbow", "Арбалет", "IXBOWL01", "GXBOW01", {QuickWeapon}, {ProficiencyCrossbow, FocusCrossbows}, {{1, 8}}},
{"Heavy crossbow", "Тяжелый арбалет", "IXBOWH01", "GXBOW01", {QuickWeapon}, {ProficiencyHeavyCrossbow, FocusCrossbows}, {{1, 10}}},
{"Sling", "Праща", "ISLNGB1", "GSLNG01", {QuickWeapon}, {ProficiencySimple}, {{1, 4}}},
//
{"Battle axe", "Боевой топор", "IAX1HB2", "GAX1H01", {QuickWeapon}, {ProficiencyAxe, FocusAxes}, {{1, 8}}},
{"Dagger", "Кинжал", "IDAGGB1", "GDAGG01", {QuickWeapon, QuickOffhand}, {ProficiencyDagger, FocusDaggers}, {{1, 4}}},
{"Greataxe", "Секира", "IAX2HB1", "GHALB01", {QuickWeapon}, {ProficiencyGreatweapon, FocusAxes}, {{1, 12}}},
{"Halberd", "Алебарда", "IHALBB1", "GHALB01", {QuickWeapon}, {ProficiencyGreatweapon, FocusPolearm}, {{1, 10}}},
{"Handaxe", "Топор", "IAX1HB1", "GAX1H01", {QuickWeapon, QuickOffhand}, {ProficiencyAxe, FocusAxes}, {{1, 6}}},
{"Scimitar", "Сабля", "ISWDCB2", "GSW1H07", {QuickWeapon, QuickOffhand}, {ProficiencyScimitar, FocusShortswords}, {{1, 6}}},
{"Longsword", "Длинный меч", "ISWDLB1", "GSW1H01", {QuickWeapon}, {ProficiencyLongsword, FocusLongswords}, {{1, 8}}},
{"Shortsword", "Короткий меч", "ISWDSB1", "GSW1H07", {QuickWeapon, QuickOffhand}, {ProficiencyShortsword, FocusShortswords}, {{1, 6}}},
{"Two handed sword", "Двуручный меч", "ISWDTB1", "GSW2H01", {QuickWeapon}, {ProficiencyGreatweapon, FocusGreatswords}, {{2, 6}}},
{"Rapier", "Рапира", "ISWDSB1", "GSW1H07", {QuickWeapon}, {ProficiencyShortsword, FocusShortswords}, {{1, 6}}}, // TODO: найти лучше вариант
{"Short bow", "Короткий лук", "IBOWSB1", "GBOW01", {QuickWeapon}, {ProficiencyShortbow, FocusBows}, {{1, 6}}},
{"Long bow", "Длинный лук", "IBOWLB1", "GBOW01", {QuickWeapon}, {ProficiencyLongbow, FocusBows}, {{1, 8}}},
//
{"Waraxe", "Военный топор", "IAX1HBB", "GHAMM01", {QuickWeapon}, {ProficiencyWaraxe, FocusAxes}, {{1, 10}}},
{"Bastard sword", "Полутораручный меч", "ISWDBB1", "GSW1H01", {QuickWeapon}, {ProficiencyBastardsword, FocusLongswords}, {{1, 10}}},
{"Katana", "Катана", "ISWDBB1", "GSW2H01", {QuickWeapon}, {ProficiencyCatana, FocusGreatswords}, {{1, 10}}}, // TODO: найти лучше вариант
//
{"Leather armor", "Кожанная броня", "IARMLB1", "GLEAT01", {Body}, {ArmorProfeciencyLight}, {2}},
{"Studded leather armor", "Клепанная кожанная броня", "IARMSB1", "GLEAT01", {Body}, {ArmorProfeciencyLight}, {3}},
{"Chain shirt", "Кольчужная рубаха", "IARMCM1", "GCHAN01", {Body}, {ArmorProfeciencyMedium}, {4}},
{"Scale mail", "Чешуйчатый доспех", "IARMSM1", "GCHAN01", {Body}, {ArmorProfeciencyMedium}, {4}},
{"Chainmail", "Кольчуга", "IARMCB1", "GCHAN01", {Body}, {ArmorProfeciencyMedium}, {5}},
{"Breastplate", "Нагрудник", "IARMHM3", "GCHAN01", {Body}, {ArmorProfeciencyMedium}, {5}},
{"Splint mail", "", "IARMPB1", "GPLAT01", {Body}, {ArmorProfeciencyHeavy}, {6}},
{"Banded mail", "Кольцевой доспех", "IARMPM1", "GPLAT01", {Body}, {ArmorProfeciencyHeavy}, {7}},
{"Plate mail", "Латы", "IARMFM1", "GPLAT01", {Body}, {ArmorProfeciencyHeavy}, {8}},
//
{"Shield small", "Малый щит", "ISHDSB1", "GSHLD01", {QuickOffhand}, {ShieldProfeciency}, {2}},
{"Large shield", "Средний щит", "ISHDLB1", "GSHLD03", {QuickOffhand}, {ShieldProfeciency}, {2}},
{"Tower shield", "Огромный щит", "ISHDTB1", "GSHLD05", {QuickOffhand}, {ShieldProfeciency}, {2}},
//
{"Helm", "Шлем", "IHELMB1", "GHELM01", {Head}, {ArmorProfeciencyMedium}},
//
{"Silver Coins", "Серебро", "IMISC08", "GMISC87", {}, {}, {}, 100, 0, 1},
{"Gold Coins", "Золото", "IMISC07", "GMISC87", {}, {}, {}, 100, 0, 10},
};
assert_enum(item, LastItem);
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

const char* item::getfgname(int type) {
	return item_data[type].ground_file;
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

bool item::isthrown() const {
	return type == Sling;
}

const dice& item::getattack() const {
	return item_data[type].ai;
}

int	item::getcount() const {
	if(item_data[type].count)
		return count + 1;
	else
		return 1;
}

void item::setcount(int value) {
	if(value <= 0)
		clear();
	else if(item_data[type].count) {
		if(value > item_data[type].count)
			value = item_data[type].count;
		count = value - 1;
	}
}