#include "main.h"

static_assert(sizeof(item) == 4, "Struct 'item_t' can be only 'int' sized");
static item ammunition_arrow(Arrow);

static itemi::poweri sword_powers[] = {{},
{Minor, 1},
{Minor, 2},
};

BSDATA(itemi) = {{"NoItem", "��� ��������", {"IHANDGF", "GGEM01"}},
{"Club", "������", {"ICLUBB1", "GBLUN01", res::WQSMC}, QuickOffhand, {ProficiencyClub, FocusMaces}, {}, {{Bludgeon, 1, 6}}},
{"Hammer", "�����", {"IHAMMB1", "GHAMM01", res::WQSWH}, QuickOffhand, {ProficiencyMace, FocusMaces}, {}, {{Bludgeon, 1, 6}}},
{"Mace", "������", {"IMACEB1", "GBLUN06", res::WQSMC}, QuickOffhand, {ProficiencyMace, FocusMaces}, {}, {{Bludgeon, 1, 6, 1}}},
{"Spear", "�����", {"ISPERB1", "GSPER01", res::WQSSP}, QuickWeapon, {ProficiencySpear, FocusPolearm}, {}, {{Pierce, 1, 8}}},
{"Staff", "�����", {"ISTAFB1", "GSTAF01", res::WQSQS}, QuickWeapon, {ProficiencySimple, FocusPolearm}, {}, {{Bludgeon, 1, 6}}},
{"Crossbow", "�������", {"IXBOWL01", "GXBOW01", res::WQSBW}, QuickWeapon, {ProficiencyCrossbow, FocusShooting}, {}, {{Pierce, 1, 8}}},
{"HeavyCrossbow", "������� �������", {"IXBOWH01", "GXBOW01", res::WQSBW}, QuickWeapon, {ProficiencyHeavyCrossbow, FocusShooting}, {}, {{Pierce, 1, 10}}},
{"Sling", "�����", {"ISLNGB1", "GSLNG01", res::WQSSL}, QuickWeapon, {ProficiencySimple}, {}, {{Bludgeon, 1, 4}}},

{"BattleAxe", "������ �����", {"IAX1HB2", "GAX1H01", res::WQSAX}, QuickWeapon, {ProficiencyAxe, FocusAxes}, {}, {{Slashing, 1, 8}}, 0, 0, 0, {}},
{"Dagger", "������", {"IDAGGB1", "GDAGG01", res::WQSDD}, QuickOffhand, {ProficiencyDagger, FocusDaggers}, {}, {{Pierce, 1, 4}}},
{"Greataxe", "������", {"IAX2HB1", "GHALB01", res::WQSHB}, QuickWeapon, {ProficiencyGreatweapon, FocusAxes}, {}, {{Slashing, 1, 12}}, 0, 0, 0, {}},
{"Halberd", "��������", {"IHALBB1", "GHALB01", res::WQSHB}, QuickWeapon, {ProficiencyGreatweapon, FocusPolearm}, {}, {{Slashing, 1, 10}}},
{"Handaxe", "�����", {"IAX1HB1", "GAX1H01", res::WQSAX}, QuickOffhand, {ProficiencyAxe, FocusAxes}, {}, {{Slashing, 1, 6}}},
{"Scimitar", "�����", {"ISWDCB2", "GSW1H07", res::WQSSS}, QuickOffhand, {ProficiencyScimitar, FocusSwords}, {}, {{Slashing, 1, 6}}, 0, 0, 0, {}},
{"Longsword", "������� ���", {"ISWDLB1", "GSW1H01", res::WQSS1}, QuickWeapon, {ProficiencyLongsword, FocusSwords}, {}, {{Slashing, 1, 8}}, 0, 0, 0, {}},
{"Shortsword", "�������� ���", {"ISWDSB1", "GSW1H07", res::WQSSS}, QuickOffhand, {ProficiencyShortsword, FocusSwords}, {}, {{Slashing, 1, 6}}},
{"TwoHandedSword", "��������� ���", {"ISWDTB1", "GSW2H01", res::WQSS2}, QuickWeapon, {ProficiencyGreatweapon, FocusSwords}, {}, {{Slashing, 2, 6}}},
{"Rapier", "������", {"ISWDSB1", "GSW1H07", res::WQSSS}, QuickWeapon, {ProficiencyShortsword, FocusDaggers}, {}, {{Pierce, 1, 6}}},
{"ShortBow", "�������� ���", {"IBOWSB1", "GBOW01", res::WQSBW, res::ARARROW}, QuickWeapon, {ProficiencyShortbow, FocusShooting}, {}, {{Pierce, 1, 6}}},
{"LongBow", "������� ���", {"IBOWLB1", "GBOW01", res::WQSBW, res::ARARROW}, QuickWeapon, {ProficiencyLongbow, FocusShooting}, {}, {{Pierce, 1, 8}}},

{"Waraxe", "������� �����", {"IAX1HBB", "GHAMM01", res::WQSAX}, QuickWeapon, {ProficiencyWaraxe, FocusAxes}, {}, {{Slashing, 1, 10}}},
{"Bastard sword", "�������������� ���", {"ISWDBB1", "GSW1H01", res::WQSS1}, QuickWeapon, {ProficiencyBastardsword, FocusSwords}, {}, {{Slashing, 1, 10}}},
{"Katana", "������", {"ISWDBB1", "GSW2H01", res::WQSS2}, QuickWeapon, {ProficiencyCatana, FocusSwords}, {}, {{Slashing, 2, 6}}, 0, 0, 0, {}, "���� ��� ������ ���������� ��������� ��������. �� �������, ��� ��� ��� ����� ������, ����������� ������."},

{"Leather armor", "�������� �����", {"IARMLB1", "GLEAT01"}, Body, {ArmorProfeciencyLight}, {}, {{}, 2}},
{"Studded leather armor", "��������� �������� �����", {"IARMSB1", "GLEAT01"}, Body, {ArmorProfeciencyLight}, {}, {{}, 3}},
{"Chain shirt", "���������� ������", {"IARMCM1", "GCHAN01"}, Body, {ArmorProfeciencyMedium}, {}, {{}, 4}},
{"Scale mail", "���������� ������", {"IARMSM1", "GCHAN01"}, Body, {ArmorProfeciencyMedium}, {}, {{}, 4}},
{"Chainmail", "��������", {"IARMCB1", "GCHAN01"}, Body, {ArmorProfeciencyMedium}, {}, {{}, 5}},
{"Breastplate", "���������", {"IARMHM3", "GCHAN01"}, Body, {ArmorProfeciencyMedium}, {}, {{}, 5}},
{"Splint mail", "", {"IARMPB1", "GPLAT01"}, Body, {ArmorProfeciencyHeavy}, {}, {{}, 6}},
{"Banded mail", "��������� ������", {"IARMPM1", "GPLAT01"}, Body, {ArmorProfeciencyHeavy}, {}, {{}, 7}},
{"Plate mail", "����", {"IARMFM1", "GPLAT01"}, Body, {ArmorProfeciencyHeavy}, {}, {{}, 8}},

{"Shield small", "����� ���", {"ISHDSB1", "GSHLD01", res::WQSD1}, QuickOffhand, {ShieldProfeciency}},
{"Large shield", "������� ���", {"ISHDLB1", "GSHLD03", res::WQSD3}, QuickOffhand, {ShieldProfeciency}},
{"Tower shield", "�������� ���", {"ISHDTB1", "GSHLD05", res::WQSD4}, QuickOffhand, {ShieldProfeciency}},

{"Helm", "����", {"IHELMB1", "GHELM01", res::WQSH1}, Head, {ArmorProfeciencyMedium}, {}},

{"Arrow", "������", {"IAROWB1", "GAROW01"}, Quiver},

{"Cooper Pieces", "����", {"IMISC08", "GMISC87"}, {}, {}, {}, {}, 100, 0, CP},
{"Silver Pieces", "�������", {"IMISC08", "GMISC87"}, {}, {}, {}, {}, 100, 0, SP},
{"Gold Pieces", "������", {"IMISC07", "GMISC87"}, {}, {}, {}, {}, 100, 0, GP},
{"Platinum Pieces", "�������", {"IMISC08", "GMISC87"}, {}, {}, {}, {}, 100, 0, PP},

{"Blue quarz", "������� �����", {"IMISC33", "GGEM01"}, {}, {}, {}, {}, 25, 0, 4 * GP},
{"Carved Stone", "������ ������", {"IBSTONE", "GGEM01"}, {}, {}, {}, {}, 25, 0, 15 * GP},
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
	auto r = 0;
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
	if(!isbow())
		return NoItem;
	return Arrow;
}

int item::getarmorindex() const {
	switch(geti().feat[0]) {
	case ArmorProfeciencyLight: return 1;
	case ArmorProfeciencyMedium: return 2;
	case ArmorProfeciencyHeavy: return 3;
	default: return 0;
	}
}

bool item::is(slot_s v) const {
	auto& ei = geti();
	switch(v) {
	case QuickWeapon:
		if(islight())
			return true;
		return  ei.slot == v;
	case LeftFinger:
		return  ei.slot == RightFinger;
	default:
		return ei.slot == v;
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

void item::apply(attacki& e) const {
	auto& ei = geti();
	e.damage = ei.ai.damage;
	e.range = ei.ai.range;
	if(is(Deadly))
		e.multiplier++;
	if(is(Balanced))
		e.critical++;
	if(is(Precise))
		e.critical++;
}

const int itemi::poweri::getweight() const {
	return 1;
}