#include "main.h"

static_assert(sizeof(item) == 4, "Struct 'item_t' can be only 'int' sized");
static item ammunition_arrow(Arrow);

BSDATA(itemi) = {{"No item", "��� ��������", {"IHANDGF", "GGEM01"}},
{"Club", "������", {"ICLUBB1", "GBLUN01", res::WQSMC}, {QuickWeapon, QuickOffhand}, {ProficiencyClub, FocusMaces}, {{1, 6}}},
{"Hammer", "�����", {"IHAMMB1", "GHAMM01", res::WQSWH}, {QuickWeapon, QuickOffhand}, {ProficiencyMace, FocusMaces}, {{1, 6}}},
{"Mace", "������", {"IMACEB1", "GBLUN06", res::WQSMC}, {QuickWeapon, QuickOffhand}, {ProficiencyMace, FocusMaces}, {{1, 6, 1}}},
{"Spear", "�����", {"ISPERB1", "GSPER01", res::WQSSP}, {QuickWeapon}, {ProficiencySpear, FocusPolearm}, {{1, 8}, 0, 0, 20}},
{"Staff", "�����", {"ISTAFB1", "GSTAF01", res::WQSQS}, {QuickWeapon}, {ProficiencySimple, FocusPolearm}, {{1, 6}}},
{"Crossbow", "�������", {"IXBOWL01", "GXBOW01", res::WQSBW}, {QuickWeapon}, {ProficiencyCrossbow, FocusCrossbows}, {{1, 8}, 0, 0, 80}},
{"Heavy crossbow", "������� �������", {"IXBOWH01", "GXBOW01", res::WQSBW}, {QuickWeapon}, {ProficiencyHeavyCrossbow, FocusCrossbows}, {{1, 10}, 0, 0, 120}},
{"Sling", "�����", {"ISLNGB1", "GSLNG01", res::WQSSL}, {QuickWeapon}, {ProficiencySimple}, {{1, 4}, 0, 0, 50}},
//
{"Battle axe", "������ �����", {"IAX1HB2", "GAX1H01", res::WQSAX}, {QuickWeapon}, {ProficiencyAxe, FocusAxes}, {{1, 8}}},
{"Dagger", "������", {"IDAGGB1", "GDAGG01", res::WQSDD}, {QuickWeapon, QuickOffhand}, {ProficiencyDagger, FocusDaggers}, {{1, 4}}},
{"Greataxe", "������", {"IAX2HB1", "GHALB01", res::WQSHB}, {QuickWeapon}, {ProficiencyGreatweapon, FocusAxes}, {{1, 12}}},
{"Halberd", "��������", {"IHALBB1", "GHALB01", res::WQSHB}, {QuickWeapon}, {ProficiencyGreatweapon, FocusPolearm}, {{1, 10}}},
{"Handaxe", "�����", {"IAX1HB1", "GAX1H01", res::WQSAX}, {QuickWeapon, QuickOffhand}, {ProficiencyAxe, FocusAxes}, {{1, 6}}},
{"Scimitar", "�����", {"ISWDCB2", "GSW1H07", res::WQSSS}, {QuickWeapon, QuickOffhand}, {ProficiencyScimitar, FocusShortswords}, {{1, 6}}},
{"Longsword", "������� ���", {"ISWDLB1", "GSW1H01", res::WQSS1}, {QuickWeapon}, {ProficiencyLongsword, FocusLongswords}, {{1, 8}}},
{"Shortsword", "�������� ���", {"ISWDSB1", "GSW1H07", res::WQSSS}, {QuickWeapon, QuickOffhand}, {ProficiencyShortsword, FocusShortswords}, {{1, 6}}},
{"Two handed sword", "��������� ���", {"ISWDTB1", "GSW2H01", res::WQSS2}, {QuickWeapon}, {ProficiencyGreatweapon, FocusGreatswords}, {{2, 6}}},
{"Rapier", "������", {"ISWDSB1", "GSW1H07", res::WQSSS}, {QuickWeapon}, {ProficiencyShortsword, FocusShortswords}, {{1, 6}}}, // TODO: ����� ����� �������
{"Short bow", "�������� ���", {"IBOWSB1", "GBOW01", res::WQSBW, res::ARARROW}, {QuickWeapon}, {ProficiencyShortbow, FocusBows}, {{1, 6}, 0, 0, 60, &ammunition_arrow}},
{"Long bow", "������� ���", {"IBOWLB1", "GBOW01", res::WQSBW, res::ARARROW}, {QuickWeapon}, {ProficiencyLongbow, FocusBows}, {{1, 8}, 0, 0, 100, &ammunition_arrow}},
//
{"Waraxe", "������� �����", {"IAX1HBB", "GHAMM01", res::WQSAX}, {QuickWeapon}, {ProficiencyWaraxe, FocusAxes}, {{1, 10}}},
{"Bastard sword", "�������������� ���", {"ISWDBB1", "GSW1H01", res::WQSS1}, {QuickWeapon}, {ProficiencyBastardsword, FocusLongswords}, {{1, 10}}},
{"Katana", "������", {"ISWDBB1", "GSW2H01", res::WQSS2}, {QuickWeapon}, {ProficiencyCatana, FocusGreatswords}, {{1, 10}}}, // TODO: ����� ����� �������
//
{"Leather armor", "�������� �����", {"IARMLB1", "GLEAT01"}, {Body}, {ArmorProfeciencyLight}, {2}},
{"Studded leather armor", "��������� �������� �����", {"IARMSB1", "GLEAT01"}, {Body}, {ArmorProfeciencyLight}, {3}},
{"Chain shirt", "���������� ������", {"IARMCM1", "GCHAN01"}, {Body}, {ArmorProfeciencyMedium}, {4}},
{"Scale mail", "���������� ������", {"IARMSM1", "GCHAN01"}, {Body}, {ArmorProfeciencyMedium}, {4}},
{"Chainmail", "��������", {"IARMCB1", "GCHAN01"}, {Body}, {ArmorProfeciencyMedium}, {5}},
{"Breastplate", "���������", {"IARMHM3", "GCHAN01"}, {Body}, {ArmorProfeciencyMedium}, {5}},
{"Splint mail", "", {"IARMPB1", "GPLAT01"}, {Body}, {ArmorProfeciencyHeavy}, {6}},
{"Banded mail", "��������� ������", {"IARMPM1", "GPLAT01"}, {Body}, {ArmorProfeciencyHeavy}, {7}},
{"Plate mail", "����", {"IARMFM1", "GPLAT01"}, {Body}, {ArmorProfeciencyHeavy}, {8}},
//
{"Shield small", "����� ���", {"ISHDSB1", "GSHLD01", res::WQSD1}, {QuickOffhand}, {ShieldProfeciency}, {2}},
{"Large shield", "������� ���", {"ISHDLB1", "GSHLD03", res::WQSD3}, {QuickOffhand}, {ShieldProfeciency}, {2}},
{"Tower shield", "�������� ���", {"ISHDTB1", "GSHLD05", res::WQSD4}, {QuickOffhand}, {ShieldProfeciency}, {2}},
//
{"Helm", "����", {"IHELMB1", "GHELM01", res::WQSH1}, {Head}, {ArmorProfeciencyMedium}},
//
{"Arrow", "������", {"IAROWB1", "GAROW01"}, {Quiver}},
//
{"Cooper Pieces", "����", {"IMISC08", "GMISC87"}, {}, {}, {}, 100, 0, CP},
{"Silver Pieces", "�������", {"IMISC08", "GMISC87"}, {}, {}, {}, 100, 0, SP},
{"Gold Pieces", "������", {"IMISC07", "GMISC87"}, {}, {}, {}, 100, 0, GP},
{"Platinum Pieces", "�������", {"IMISC08", "GMISC87"}, {}, {}, {}, 100, 0, PP},
//
{"Blue quarz", "������� �����", {"IMISC33", "GGEM01"}, {}, {}, {}, 25, 0, 4 * GP},
{"Carved Stone", "������ ������", {"IBSTONE", "GGEM01"}, {}, {}, {}, 25, 0, 15 * GP},
};
assert_enum(itemi, LastItem)

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
	for(auto s : bsdata<itemi>::elements[type].slots) {
		if(s == value)
			return true;
	}
	return false;
}

bool item::isranged() const {
	return bsdata<itemi>::elements[type].ai.range != 0;
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

bool item::is(feat_s value) const {
	return bsdata<itemi>::elements[type].feat[0] == value
		|| bsdata<itemi>::elements[type].feat[1] == value;
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
	switch(bsdata<itemi>::elements[type].feat[0]) {
	case ArmorProfeciencyLight: return 1;
	case ArmorProfeciencyMedium: return 2;
	case ArmorProfeciencyHeavy: return 3;
	default: return 0;
	}
}

bool creature::isallow(const item& it) const {
	const auto& ei = bsdata<itemi>::elements[it.gettype()];
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
	return bsdata<itemi>::elements[type].ai.ammunition
		&& bsdata<itemi>::elements[type].ai.ammunition->type == Arrow;
}

bool item::isxbow() const {
	return type == CrossbowLight || type == CrossbowLight;
}

bool item::isthrown() const {
	return type == Sling;
}

const attack_info& item::getattack() const {
	return bsdata<itemi>::elements[type].ai;
}

int	item::getcount() const {
	if(bsdata<itemi>::elements[type].count)
		return count + 1;
	else
		return 1;
}

int	item::getcost() const {
	return bsdata<itemi>::elements[type].cost;
}

void item::setcount(int value) {
	if(value <= 0)
		clear();
	else if(bsdata<itemi>::elements[type].count) {
		if(value > bsdata<itemi>::elements[type].count)
			value = bsdata<itemi>::elements[type].count;
		count = value - 1;
	}
}