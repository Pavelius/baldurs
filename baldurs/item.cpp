#include "main.h"

static_assert(sizeof(item) == 4, "Struct 'item_t' can be only 'int' sized");
static item ammunition_arrow(Arrow);

static itemi::poweri sword_powers[] = {{},
{Minor, 1},
{Minor, 2},
{Medium, 3},
{Medium, 2, "%1 поджигания", Fire},
{Medium, 2, "%1 обморожения", Cold},
};

BSDATA(itemi) = {{"NoItem", {"IHANDGF", "GGEM01"}},
{"Club", {"ICLUBB1", "GBLUN01", res::WQSMC}, QuickOffhand, {ProficiencyClub, FocusMaces}, {}, {{Bludgeon, 1, 6}}},
{"Hammer", {"IHAMMB1", "GHAMM01", res::WQSWH}, QuickOffhand, {ProficiencyMace, FocusMaces}, {}, {{Bludgeon, 1, 6}}},
{"Mace", {"IMACEB1", "GBLUN06", res::WQSMC}, QuickOffhand, {ProficiencyMace, FocusMaces}, {}, {{Bludgeon, 1, 6, 1}}},
{"Spear", {"ISPERB1", "GSPER01", res::WQSSP}, QuickWeapon, {ProficiencySpear, FocusPolearm}, {}, {{Pierce, 1, 8}}},
{"Staff", {"ISTAFB1", "GSTAF01", res::WQSQS}, QuickWeapon, {ProficiencySimple, FocusPolearm}, {TwoHanded}, {{Bludgeon, 1, 6}}},
{"Crossbow", {"IXBOWL01", "GXBOW01", res::WQSBW}, QuickWeapon, {ProficiencyCrossbow, FocusShooting}, {}, {{Pierce, 1, 8}}},
{"HeavyCrossbow", {"IXBOWH01", "GXBOW01", res::WQSBW}, QuickWeapon, {ProficiencyHeavyCrossbow, FocusShooting}, {}, {{Pierce, 1, 10}}},
{"Sling", {"ISLNGB1", "GSLNG01", res::WQSSL}, QuickWeapon, {ProficiencySimple}, {}, {{Bludgeon, 1, 4}, 0, Range50}},

{"BattleAxe", {"IAX1HB2", "GAX1H01", res::WQSAX}, QuickWeapon, {ProficiencyAxe, FocusAxes}, {}, {{Slashing, 1, 8}}, 0, 0, 0, {}},
{"Dagger", {"IDAGGB1", "GDAGG01", res::WQSDD}, QuickOffhand, {ProficiencyDagger, FocusDaggers}, {}, {{Pierce, 1, 4}}},
{"Greataxe", {"IAX2HB1", "GHALB01", res::WQSHB}, QuickWeapon, {ProficiencyGreatweapon, FocusAxes}, {TwoHanded}, {{Slashing, 1, 12}}, 0, 0, 0, {}},
{"Halberd", {"IHALBB1", "GHALB01", res::WQSHB}, QuickWeapon, {ProficiencyGreatweapon, FocusPolearm}, {TwoHanded}, {{Slashing, 1, 10}}},
{"Handaxe", {"IAX1HB1", "GAX1H01", res::WQSAX}, QuickOffhand, {ProficiencyAxe, FocusAxes}, {}, {{Slashing, 1, 6}}},
{"Scimitar", {"ISWDCB2", "GSW1H07", res::WQSSS}, QuickOffhand, {ProficiencyScimitar, FocusSwords}, {}, {{Slashing, 1, 6}}, 0, 0, 0, {}},
{"Longsword", {"ISWDLB1", "GSW1H01", res::WQSS1}, QuickWeapon, {ProficiencyLongsword, FocusSwords}, {}, {{Slashing, 1, 8}}, 0, 0, 0, sword_powers},
{"Shortsword", {"ISWDSB1", "GSW1H07", res::WQSSS}, QuickOffhand, {ProficiencyShortsword, FocusSwords}, {}, {{Slashing, 1, 6}}},
{"TwoHandedSword", {"ISWDTB1", "GSW2H01", res::WQSS2}, QuickWeapon, {ProficiencyGreatweapon, FocusSwords}, {TwoHanded}, {{Slashing, 2, 6}}},
{"Rapier", {"ISWDSB1", "GSW1H07", res::WQSSS}, QuickWeapon, {ProficiencyShortsword, FocusDaggers}, {}, {{Pierce, 1, 6}}},
{"ShortBow", {"IBOWSB1", "GBOW01", res::WQSBW, res::ARARROW}, QuickWeapon, {ProficiencyShortbow, FocusShooting}, {}, {{Pierce, 1, 6}, 0, Range60}},
{"LongBow", {"IBOWLB1", "GBOW01", res::WQSBW, res::ARARROW}, QuickWeapon, {ProficiencyLongbow, FocusShooting}, {}, {{Pierce, 1, 8}, 0, Range100}},

{"Waraxe", {"IAX1HBB", "GHAMM01", res::WQSAX}, QuickWeapon, {ProficiencyWaraxe, FocusAxes}, {}, {{Slashing, 1, 10}}},
{"BastardSword", {"ISWDBB1", "GSW1H01", res::WQSS1}, QuickWeapon, {ProficiencyBastardsword, FocusSwords}, {}, {{Slashing, 1, 10}}},
{"Katana", {"ISWDBB1", "GSW2H01", res::WQSS2}, QuickWeapon, {ProficiencyCatana, FocusSwords}, {}, {{Slashing, 2, 6}}, 0, 0, 0, {}},

{"LeatherArmor", {"IARMLB1", "GLEAT01"}, Body, {ArmorProfeciencyLight}, {}, {{}, 2}},
{"StuddedLeatherArmor", {"IARMSB1", "GLEAT01"}, Body, {ArmorProfeciencyLight}, {}, {{}, 3}},
{"ChainShirt", {"IARMCM1", "GCHAN01"}, Body, {ArmorProfeciencyMedium}, {}, {{}, 4}},
{"ScaleMail", {"IARMSM1", "GCHAN01"}, Body, {ArmorProfeciencyMedium}, {}, {{}, 4}},
{"Chainmail", {"IARMCB1", "GCHAN01"}, Body, {ArmorProfeciencyMedium}, {}, {{}, 5}},
{"Breastplate", {"IARMHM3", "GCHAN01"}, Body, {ArmorProfeciencyMedium}, {}, {{}, 5}},
{"SplintMail", {"IARMPB1", "GPLAT01"}, Body, {ArmorProfeciencyHeavy}, {}, {{}, 6}},
{"BandedMail", {"IARMPM1", "GPLAT01"}, Body, {ArmorProfeciencyHeavy}, {}, {{}, 7}},
{"PlateMail", {"IARMFM1", "GPLAT01"}, Body, {ArmorProfeciencyHeavy}, {}, {{}, 8}},

{"SmallShield", {"ISHDSB1", "GSHLD01", res::WQSD1}, QuickOffhand, {ShieldProfeciency}, {}, {{}, 2}},
{"LargeShield", {"ISHDLB1", "GSHLD03", res::WQSD3}, QuickOffhand, {ShieldProfeciency}, {}, {{}, 2}},
{"TowerShield", {"ISHDTB1", "GSHLD05", res::WQSD4}, QuickOffhand, {ShieldProfeciency}, {}, {{}, 3}},

{"Helm", {"IHELMB1", "GHELM01", res::WQSH1}, Head, {ArmorProfeciencyMedium}, {}, {{}, 0, Touch, 40}},

{"Arrow", {"IAROWB1", "GAROW01"}, Quiver},

{"CooperPieces", {"IMISC08", "GMISC87"}, {}, {}, {}, {}, 100, 0, CP},
{"SilverPieces", {"IMISC08", "GMISC87"}, {}, {}, {}, {}, 100, 0, SP},
{"GoldPieces", {"IMISC07", "GMISC87"}, {}, {}, {}, {}, 100, 0, GP},
{"PlatinumPieces", {"IMISC08", "GMISC87"}, {}, {}, {}, {}, 100, 0, PP},

{"BlueQuarz", {"IMISC33", "GGEM01"}, {}, {}, {}, {}, 25, 0, 4 * GP},
{"CarvedStone", {"IBSTONE", "GGEM01"}, {}, {}, {}, {}, 25, 0, 15 * GP},
};
assert_enum(itemi, LastItem)

void item::clear() {
	memset(this, 0, sizeof(*this));
}

int	item::getbonus() const {
	return 0;
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
	return geti().ai.ac;
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

bool item::isranged() const {
	return geti().ai.range > Touch;
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
	e.range = bsdata<rangei>::elements[ei.ai.range].get(0);
	if(is(Deadly))
		e.multiplier++;
	if(is(Balanced))
		e.critical++;
	if(is(Precise))
		e.critical++;
	if(!e.range)
		e.range = 5;
	if(is(ReachItem))
		e.range += 5;
	auto pi = getpower();
	if(pi) {
		if(ei.ai.damage) {
			e.bonus += pi->bonus;
			e.damage.b += pi->bonus;
		}
	}
}

const int itemi::poweri::getweight() const {
	return 1;
}

creature* item::getowner() const {
	auto index = bsdata<creature>::source.indexof(this);
	if(index == -1)
		return 0;
	return bsdata<creature>::elements + index;
}

void item::equip(const item& it) {
	auto p = getowner();
	if(p)
		p->dressoff();
	*this = it;
	if(p)
		p->dresson();
}

void item::remove() {
	auto p = getowner();
	if(p)
		p->dressoff();
	clear();
	if(p)
		p->dresson();
}

itemi::poweri* item::getpower() const {
	if(!identified)
		return 0;
	auto& ei = geti();
	if(!ei.powers)
		return 0;
	auto& ep = ei.powers[effect];
	return ep ? &ep : 0;
}

void item::setmagic() {
	auto& ei = geti();
	auto count = ei.powers.getcount();
	if(!count)
		return;
	auto start = 0;
	if(!ei.powers[0].power)
		start++;
	effect = xrand(start, count - 1);
}