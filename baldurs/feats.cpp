#include "main.h"

static const char* levels_rome[6] = {"I", "II", "III", "IV", "V", "VI"};
static const char* levels_naming[6] = {"1st", "2nd", "3rd", "4th", "5th", "6th"};

BSDATA(feati) = {{"None"},
{"Alertness", "������������"},
{"Armor Profeciency (Light)", "������� ������ �����"},
{"Armor Profeciency (Medium)", "������� ������� �����", {}, {ArmorProfeciencyLight}},
{"Armor Profeciency (Heavy)", "������� ������� �����", {}, {ArmorProfeciencyLight, ArmorProfeciencyMedium}},
{"Blind-Fight", "�������� �������"},
{"Cleave", "����������", {13}, {PowerAttack}},
{"Combat Casting", "������ ����������"},
{"Combat Expertise", "������ ����", {0, 0, 0, 13}, {}, 0, 0, "�� ������ �������� ������ ������� �������� ��� � �����, ��� � � ������.", "��������� ��� ����� ��� � �������� ������ �����, ��� � � ������� �������� ����� � ���������� ���, �� �������� ����� -3 � ������ �����, �� +3 � ������ �������.", "��������, �� ���������� ���� ������ ����� ��������� � ������ �� ����� ����� ��� ������� ��������, ������� ��� ���� -4 ����� � ����� � +2 ����� ��������� � ������ �������."},
{"Combat Reflexes", "������ ��������", {}, {},0,0, "�� �������� ����������� �������� �������� �����, ���� ��� ��������� ������ ������������.", "���� ���� ��������� � �������� ���� ��������, �� �������� �������� ������������� �����. �� ���������� ����� ������������ ����� ��������.", "��������, �� ������� ���� �����, �������� ��������� ���� ���� ����� � �����, � �� ����� ��������� ������������� �����, ���� ������� ��������."},
{"Deadly Precision", "����������� ��������", {0, 15}, {}, 5},
{"Deflects Arrows", "��������� �����", {0, 13}, {ImprovedUnarmedStrike}},
{"Dodge", "���������", {0, 13}},
{"Endurance", "������������", {}, {}, 0, 0, "�� ���������� ������������� �����, ��������� ���� �� ����� ���������.", "� ����� ��������, ��� ���������� �������� ����������� ����������, ���������� �� ���������� ���������� �������, � ��� +4 ����� � ��������\n* ������ ����������� ���������� ����������� �� ����� ��������.\n* �������� ������������ �� ����� ����.\n* �������� ������������ ��� ��������� ���������� ����������� �� �������������� �����.\n* �������� ������������ ��� ���������� �������.\n* �������� ������������ ��� ��������� ���������� ����������� �� ������ ��� �����.\n* ���� ������� ��������� ��� ��������� ��������� ����������� �� ������� ��� �������� ���������� �����.\n* ���� ������� ��������� ��� �������������� ����������� �� ���������.\n�� ����� ������ ��� ��������� ����� � ������ ��� ������� ��������, ��� ��������� ��� ���������� ������� � ���, ��� ��������� �� �������.", "�������� ��� ���� �����, ������� ���� � ������� � ����� ������� �������� ��� ��������� ��� �� ������� ������������� �� ����� � ���."},
{"Far Shoot", "������� �������", {}, {PointBlankShoot}, 0, 0, "�� ������ �������� ����� ������� ���������� ��� �������� �� �����, ��������� � ������ ���������� ������.", "��� ������������� ����������� ������, ������ ��� ���, ���� ��� ��������� ������������� �� ��������. ��� ������������� ������������ ������, ��� ������ ��������� ���������� � ��� ����."},
{"Greate Fortitude", "������� ���������"},
{"Improved Critical", "���������� �����������", {}, {}, 8, 0, ""},
{"Improved Disarm", "���������� ��������������", {0, 0, 0, 13}, {CombatExpertise}},
{"Improved Feint", "���������� ����", {0, 0, 0, 13}, {CombatExpertise}},
{"Improved Initiative", "���������� ����������"},
{"Improved Overrun", "���������� ������", {13}, {PowerAttack}},
{"Improved Sunder", "���������� ����������", {13}, {PowerAttack}},
{"Improved Trip", "���������� ��������", {0, 0, 0, 13}, {CombatExpertise}},
{"Improved Unarmed Strike", "���������� ���������� ���"},
{"Iron Will", "�������� ����", {}, {}, 0, 0, "�� ������� � ����, ��� ������� ���������.", "+2 ����� � ��������� ����������� ����."},
{"Leadership", "���������", {}, {}, 0, 6},
{"Lighting Reflexes", "������������ ��������", {}, {}, 0, 0},
{"Mobiliy", "�����������", {0, 13}, {Dodge}},
{"Multiattack", "��������� ����", {}, {}, 0, 0, ""},
{"Point-Blank Shoot", "������� ���������", {}, {}, 0, 0, "�� ������� ��������� �� �������� ����� ����� ��������.", "�� ��������� ����� +1 � ������� ����� � ����������� ��� ������������� ������ ����������� ������, �� ��������� �� 10 ������."},
{"Power Attack", "������ �����", {13}},
{"Precise Shoot", "������ �������", {}, {PointBlankShoot}},
{"Rapid Shoot", "������� �������", {13}, {PointBlankShoot}, 0, 0, "�� ���������� ����������/����������� ������ � ������������� ���������.", "�� ��������� ���� �������������� ����� � ����� �� ���������� �������. ����� ����� ��� ����� ������� ������� ����� �����, �� ������ ����� ����� ����� �2 � ������ ����� (��� ��������������, ��� � �����������)."},
{"Rapid Reload", "������� �����������", {}, {}, 0, 0},
{"Shield Profeciency", "�������� �����", {}, {}, 0, 0},
{"Short On the Run", "������� �� ����", {0, 13}, {Dodge, Mobiliy, PointBlankShoot}, 4},
{"Spell Focus", "����� �� ����� ����������"},
{"Spring Attack", "����� � ��������", {0, 13}, {Dodge, Mobiliy}, 4},
{"Stunning Fist", "������������ ���� �����", {0, 13, 0, 0, 13}, {ImprovedUnarmedStrike}, 8},
{"Toughness", "���������"},
{"Track", "������������"},
{"Two Weapon Fighting", "�������� ����� ��������", {15}},
{"Focus axes", "����� �� �������", {0}, {ProficiencyAxe}, 1},
{"Focus bows", "����� �� �����", {0}, {ProficiencyShortbow}, 1},
{"Focus crossbows", "����� �� ���������", {0}, {ProficiencyCrossbow}, 1},
{"Focus daggers", "����� �� ��������", {0}, {ProficiencyDagger}, 1},
{"Focus greatswords", "����� �� ��������� �����", {0}, {ProficiencyGreatweapon}, 1},
{"Focus longswords", "����� �� ������� �����", {0}, {ProficiencyLongsword}, 1},
{"Focus maces", "����� �� ������", {0}, {ProficiencyMace}, 1},
{"Focus polearm", "����� �� ��������� ������", {0}, {ProficiencySimple}, 1},
{"Focus shortswords", "����� �� �������� �����", {0}, {ProficiencyShortsword}, 1},
{"WhirlwindAttack", "����� ������", {0, 13, 0, 13}, {CombatExpertise, Dodge, Mobiliy, SpringAttack}, 4},
//
{"Axe Proficiency", "�������� �������", {}},
{"Club Proficiency", "�������� �������", {}},
{"Crossbow Proficiency", "�������� ���������", {}},
{"Dagger Proficiency", "�������� ��������", {}},
{"Greatweapon Proficiency", "�������� ������� �������", {}, {ProficiencyLongsword}},
{"Heavy Crossbow Proficiency", "�������� ������� ���������", {}},
{"Mace Proficiency", "�������� �������", {}},
{"Simple weapon Proficiency", "�������� ������� �������", {}},
{"Spear Proficiency", "�������� ������", {}},
{"Longbow Proficiency", "�������� ������� �����", {}},
{"Longsword Proficiency", "�������� ������� �����", {}},
{"Shortbow Proficiency", "�������� �������� �����", {}},
{"Scimitar Proficiency", "�������� ������", {}},
{"Shortsword Proficiency", "�������� �������� �����", {}},
{"Bastardsword Proficiency", "�������� ���������� �����", {}, {ProficiencyLongsword}},
{"Catana Proficiency", "�������� �������", {}},
{"Waraxe Proficiency", "�������� ������� �������", {}, {ProficiencyAxe}},
//
{"FastMovement", "������� ������������"},
{"Illiteracy", "��������������"},
//
{"Hate orc and goblins", "��������� � ����� � ��������", {}, {}, 0, 0, "+1 ����� � ����� ����� (������� ����-�����) � �������� (������� ��������� � ����������)."},
{"Infravision", "����������", {}, {}, 0, 0, "����� ������ � ������� �� 20 ������ � �� �������� ������ ��� �������� � �������."},
{"Immunity to Sleep spell", "��������� � ���������� ���", {}, {}, 0, 0, "��������� � ���������� [���] � ���� ������ �������� �����������, ������� ����� �������."},
{"Find secrets", "������� �������", {}, {}, 0, 0, "��������� � 4 ������ �� ������� ������� ���� ��� ����� - �������� �������� ������ ������."},
{"Lucky", "���������", {}, {}, 0, 0, "����� +1 �� ���� ����-�������."},
{"Precise Thrower", "������ ��������", {}, {}, 0, 0, "+1 � ������ �����, �� ���� ����������� �������."},
{"Save Bonus Vs Echantment", "����� ������ �� ����������", {}, {}, 0, 0, "+2 � ����-������ �� ���������� ����������."},
{"Save Bonus Vs Fear", "����� ������ �� ������", {}, {}, 0, 0, "+2 � ����-������ �� ������."},
{"Save Bonus Vs Illusion", "����� ������ �� �������", {}, {}, 0, 0, "+2 � ����-������ �� ���������� �������."},
{"Save Bonus Vs Poison", "����� ������ �� ���", {}, {}, 0, 0, "+2 � ����-������ �� ����."},
{"Save Bonus Vs Spells", "����� ������ �� ����������", {}, {}, 0, 0, "+2 � ����-������ �� ���������� � �����."},
{"Stability", "������������", {}, {}, 0, 0, "+4 � ������ �� ������� ������� �������� ��� �������� ��� �������� �����."},
{"Stonecunning", "��������� �����", {}, {}, 0, 0, "+2 � ������ ��� ������� ���������� ������� � ����������. ������ ��� ���������� �����."},
};
assert_enum(feati, LastFeat)

void add_feat_item(stringbuilder& sb, feat_s id);

bool creature::isallow(feat_s id) const {
	auto& ei = bsdata<feati>::elements[id];
	for(auto e = Strenght; e <= Charisma; e = (ability_s)(e+1)) {
		auto value = ei.ability[e];
		if(value && getr(e) < value)
			return false;
	}
	for(auto e : ei.prerequisit) {
		if(e && !is(e))
			return false;
	}
	if(ei.base_attack && getbab() < ei.base_attack)
		return false;
	if(ei.character_level && getcharlevel() < ei.character_level)
		return false;
	if(ei.prerequisit_special)
		return false;
	return true;
}

bool creature::isallow(feat_s id, const unsigned char* ability, char character_level, char base_attack) {
	auto& ei = bsdata<feati>::elements[id];
	for(auto e = Strenght; e <= Charisma; e = (ability_s)(e + 1)) {
		auto value = ei.ability[e];
		if(value && ability[e] < value)
			return false;
	}
	if(ei.base_attack && base_attack < ei.base_attack)
		return false;
	if(ei.character_level && character_level < ei.character_level)
		return false;
	return true;
}