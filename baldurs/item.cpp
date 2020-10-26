#include "main.h"

static_assert(sizeof(item) == 4, "Struct 'item_t' can be only 'int' sized");
static item ammunition_arrow(Arrow);

BSDATA(itemi) = {{"No item", "Нет предмета", {"IHANDGF", "GGEM01"}},
{"Club", "Дубина", {"ICLUBB1", "GBLUN01", res::WQSMC}, QuickOffhand, {ProficiencyClub, FocusMaces}, {{1, 6}}, 0, 0, 0, {}},
{"Hammer", "Молот", {"IHAMMB1", "GHAMM01", res::WQSWH}, QuickOffhand, {ProficiencyMace, FocusMaces}, {{1, 6}}},
{"Mace", "Булава", {"IMACEB1", "GBLUN06", res::WQSMC}, QuickOffhand, {ProficiencyMace, FocusMaces}, {{1, 6, 1}}},
{"Spear", "Копье", {"ISPERB1", "GSPER01", res::WQSSP}, QuickWeapon, {ProficiencySpear, FocusPolearm}, {{1, 8}, 0, 0, 20}},
{"Staff", "Посох", {"ISTAFB1", "GSTAF01", res::WQSQS}, QuickWeapon, {ProficiencySimple, FocusPolearm}, {{1, 6}}},
{"Crossbow", "Арбалет", {"IXBOWL01", "GXBOW01", res::WQSBW}, QuickWeapon, {ProficiencyCrossbow, FocusShooting}, {{1, 8}, 0, 0, 80}},
{"Heavy crossbow", "Тяжелый арбалет", {"IXBOWH01", "GXBOW01", res::WQSBW}, QuickWeapon, {ProficiencyHeavyCrossbow, FocusShooting}, {{1, 10}, 0, 0, 120}},
{"Sling", "Праща", {"ISLNGB1", "GSLNG01", res::WQSSL}, QuickWeapon, {ProficiencySimple}, {{1, 4}, 0, 0, 50}},

{"Battle axe", "Боевой топор", {"IAX1HB2", "GAX1H01", res::WQSAX}, QuickWeapon, {ProficiencyAxe, FocusAxes}, {{1, 8}}, 0, 0, 0, {}, "Боевой топор, наиболее распространённое оружие ближнего боя среди карликов."},
{"Dagger", "Кинжал", {"IDAGGB1", "GDAGG01", res::WQSDD}, QuickOffhand, {ProficiencyDagger, FocusDaggers}, {{1, 4}}},
{"Greataxe", "Секира", {"IAX2HB1", "GHALB01", res::WQSHB}, QuickWeapon, {ProficiencyGreatweapon, FocusAxes}, {{1, 12}}, 0, 0, 0, {}, "Этот большой, тяжёлый топор, любимое оружие у варваров, или любого персонажа, желающего обладать способностью наносить непревзойдённые повреждения."},
{"Halberd", "Алебарда", {"IHALBB1", "GHALB01", res::WQSHB}, QuickWeapon, {ProficiencyGreatweapon, FocusPolearm}, {{1, 10}}},
{"Handaxe", "Топор", {"IAX1HB1", "GAX1H01", res::WQSAX}, QuickOffhand, {ProficiencyAxe, FocusAxes}, {{1, 6}}},
{"Scimitar", "Сабля", {"ISWDCB2", "GSW1H07", res::WQSSS}, QuickOffhand, {ProficiencyScimitar, FocusSwords}, {{1, 6}}, 0, 0, 0, {}, "Изогнутость лезвия этого оружия, делает кромку этого оружия очень острой."},
{"Longsword", "Длинный меч", {"ISWDLB1", "GSW1H01", res::WQSS1}, QuickWeapon, {ProficiencyLongsword, FocusSwords}, {{1, 8}}, 0, 0, 0, {}, "Это классическое, прямое лезвие. Оружие, воплощающее в себе рыцарство и доблесть. Оно излюбленное оружие паладинов."},
{"Shortsword", "Короткий меч", {"ISWDSB1", "GSW1H07", res::WQSSS}, QuickOffhand, {ProficiencyShortsword, FocusSwords}, {{1, 6}}},
{"Two handed sword", "Двуручный меч", {"ISWDTB1", "GSW2H01", res::WQSS2}, QuickWeapon, {ProficiencyGreatweapon, FocusSwords}, {{2, 6}}},
{"Rapier", "Рапира", {"ISWDSB1", "GSW1H07", res::WQSSS}, QuickWeapon, {ProficiencyShortsword, FocusDaggers}, {{1, 6}}},
{"Short bow", "Короткий лук", {"IBOWSB1", "GBOW01", res::WQSBW, res::ARARROW}, QuickWeapon, {ProficiencyShortbow, FocusShooting}, {{1, 6}, 0, 0, 60, &ammunition_arrow}},
{"Long bow", "Длинный лук", {"IBOWLB1", "GBOW01", res::WQSBW, res::ARARROW}, QuickWeapon, {ProficiencyLongbow, FocusShooting}, {{1, 8}, 0, 0, 100, &ammunition_arrow}},

{"Waraxe", "Военный топор", {"IAX1HBB", "GHAMM01", res::WQSAX}, QuickWeapon, {ProficiencyWaraxe, FocusAxes}, {{1, 10}}},
{"Bastard sword", "Полутораручный меч", {"ISWDBB1", "GSW1H01", res::WQSS1}, QuickWeapon, {ProficiencyBastardsword, FocusSwords}, {{1, 10}}},
{"Katana", "Катана", {"ISWDBB1", "GSW2H01", res::WQSS2}, QuickWeapon, {ProficiencyCatana, FocusSwords}, {{1, 10}}, 0, 0, 0, {}, "Этот меч больше напоминает двуручный скимитар. Он изогнут, что даёт ему более острое, эф-фективное лезвие."},

{"Leather armor", "Кожанная броня", {"IARMLB1", "GLEAT01"}, Body, {ArmorProfeciencyLight}, {2}},
{"Studded leather armor", "Клепанная кожанная броня", {"IARMSB1", "GLEAT01"}, Body, {ArmorProfeciencyLight}, {3}},
{"Chain shirt", "Кольчужная рубаха", {"IARMCM1", "GCHAN01"}, Body, {ArmorProfeciencyMedium}, {4}},
{"Scale mail", "Чешуйчатый доспех", {"IARMSM1", "GCHAN01"}, Body, {ArmorProfeciencyMedium}, {4}},
{"Chainmail", "Кольчуга", {"IARMCB1", "GCHAN01"}, Body, {ArmorProfeciencyMedium}, {5}},
{"Breastplate", "Нагрудник", {"IARMHM3", "GCHAN01"}, Body, {ArmorProfeciencyMedium}, {5}},
{"Splint mail", "", {"IARMPB1", "GPLAT01"}, Body, {ArmorProfeciencyHeavy}, {6}},
{"Banded mail", "Кольцевой доспех", {"IARMPM1", "GPLAT01"}, Body, {ArmorProfeciencyHeavy}, {7}},
{"Plate mail", "Латы", {"IARMFM1", "GPLAT01"}, Body, {ArmorProfeciencyHeavy}, {8}},
//
{"Shield small", "Малый щит", {"ISHDSB1", "GSHLD01", res::WQSD1}, QuickOffhand, {ShieldProfeciency}, {2}},
{"Large shield", "Средний щит", {"ISHDLB1", "GSHLD03", res::WQSD3}, QuickOffhand, {ShieldProfeciency}, {2}},
{"Tower shield", "Огромный щит", {"ISHDTB1", "GSHLD05", res::WQSD4}, QuickOffhand, {ShieldProfeciency}, {2}},

{"Helm", "Шлем", {"IHELMB1", "GHELM01", res::WQSH1}, Head, {ArmorProfeciencyMedium}},

{"Arrow", "Стрелы", {"IAROWB1", "GAROW01"}, Quiver},

{"Cooper Pieces", "Медь", {"IMISC08", "GMISC87"}, {}, {}, {}, 100, 0, CP},
{"Silver Pieces", "Серебро", {"IMISC08", "GMISC87"}, {}, {}, {}, 100, 0, SP},
{"Gold Pieces", "Золото", {"IMISC07", "GMISC87"}, {}, {}, {}, 100, 0, GP},
{"Platinum Pieces", "Платина", {"IMISC08", "GMISC87"}, {}, {}, {}, 100, 0, PP},

{"Blue quarz", "Голубой кварц", {"IMISC33", "GGEM01"}, {}, {}, {}, 25, 0, 4 * GP},
{"Carved Stone", "Резной камень", {"IBSTONE", "GGEM01"}, {}, {}, {}, 25, 0, 15 * GP},
};
assert_enum(itemi, LastItem)

void item::clear() {
	memset(this, 0, sizeof(*this));
}

int	item::getbonus() const {
	return 0;
}

bool item::istwohand() const {
	switch(type) {
	case Staff:
	case Greataxe:
	case Greatsword:
	case Halberd:
		return true;
	default:
		return false;
	}
}

bool item::isreach() const {
	switch(type) {
	case Staff:
	case Spear:
	case Halberd:
		return true;
	default:
		return false;
	}
}

int	item::getac() const {
	auto r = bsdata<itemi>::elements[type].ai.ac;
	if(r)
		r += getbonus();
	return r;
}

int item::getframe() const {
	return type * 2;
}

feat_s item::getfeat() const {
	return bsdata<itemi>::elements[type].feat[0];
}

const char* item::getfname(int type) {
	return bsdata<itemi>::elements[type].images.avatar;
}

const char* item::getfgname(int type) {
	return bsdata<itemi>::elements[type].images.ground;
}

res::tokens item::getanwear(int type) {
	return bsdata<itemi>::elements[type].images.wear;
}

res::tokens item::getanthrown() const {
	return bsdata<itemi>::elements[type].images.thrown;
}

item_s item::getammunition() const {
	return bsdata<itemi>::elements[type].ai.ammunition ? bsdata<itemi>::elements[type].ai.ammunition->type : NoItem;
}

int item::getarmorindex() const {
	switch(geti().feat[0]) {
	case ArmorProfeciencyLight: return 1;
	case ArmorProfeciencyMedium: return 2;
	case ArmorProfeciencyHeavy: return 3;
	default: return 0;
	}
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

int	item::getcost() const {
	return bsdata<itemi>::elements[type].cost;
}

int	item::getcount() const {
	if(geti().count != 0)
		return count + 1;
	else
		return 1;
}

void item::setcount(int value) {
	auto& ei = geti();
	if(value <= 0)
		clear();
	else if(ei.count) {
		if(value > ei.count)
			value = ei.count;
		count = value - 1;
	}
}