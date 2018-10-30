#include "main.h"

static const char* levels_rome[6] = {"I", "II", "III", "IV", "V", "VI"};
static const char* levels_naming[6] = {"1st", "2nd", "3rd", "4th", "5th", "6th"};

enum prerequisit_s : unsigned char {
	NoPrerequisit,
	ProfecienceWithWeapon, ThreeOrMoreNaturalAttack,
};

struct feat_info {
	const char*		id;
	const char*		name;
	char			ability[6];
	feat_s			prerequisit[4];
	char			base_attack;
	char			character_level;
	const char*		text;
	const char*		benefit;
	prerequisit_s	prerequisit_special;
} feat_data[] = {{"None"},
{"Alertness", "������������"},
{"Armor Profeciency (Light)", "������� ������ �����"},
{"Armor Profeciency (Medium)", "������� ������� �����", {}, {ArmorProfeciencyLight}},
{"Armor Profeciency (Heavy)", "������� ������� �����", {}, {ArmorProfeciencyLight, ArmorProfeciencyMedium}},
{"Blind-Fight", "�������� �������"},
{"Cleave", "����������", {13}, {PowerAttack}},
{"Combat Casting", "������ ����������"},
{"Combat Expertise", "������ ����", {0, 0, 0, 13}, {}, 0, 0, "�� ������ �������� ������ ������� �������� ��� � �����, ��� � � ������."},
{"Combat Reflexes", "������ ��������", {}, {},0,0, "�� �������� ����������� �������� �������� �����, ���� ��� ��������� ������ ������������.", "���� ���� ��������� � �������� ���� ��������, �� �������� �������� ������������� �����. �� ���������� ����� ������������ ����� ��������."},
{"Deadly Precision", "����������� ��������", {0, 15}, {}, 5},
{"Deflects Arrows", "��������� �����", {0, 13}, {ImprovedUnarmedStrike}},
{"Dodge", "���������", {0, 13}},
{"Endurance", "������������"},
{"Far Shoot", "������� �������", {}, {PointBlankShoot}},
{"Greate Fortitude", "������� ���������"},
{"Improved Critical", "���������� �����������", {}, {}, 8, 0, ""},
{"Improved Disarm", "���������� ��������������", {0, 0, 0, 13}, {CombatExpertise}},
{"Improved Feint", "���������� ����", {0, 0, 0, 13}, {CombatExpertise}},
{"Improved Initiative", "���������� ����������"},
{"Improved Overrun", "���������� ������", {13}, {PowerAttack}},
{"Improved Sunder", "���������� ����������", {13}, {PowerAttack}},
{"Improved Trip", "���������� ��������", {0, 0, 0, 13}, {CombatExpertise}},
{"Improved Unarmed Strike", "���������� ���������� ���"},
{"Iron Will", "�������� ����"},
{"Leadership", "���������", {}, {}, 0, 6},
{"Lighting Reflexes", "������������ ��������"},
{"Mobiliy", "�����������", {0, 13}, {Dodge}},
{"Multiattack", "��������� ����", {}, {}, 0, 0, ""},
{"Point-Blank Shoot", "������� ���������"},
{"Power Attack", "������ �����", {13}},
{"Precise Shoot", "������ �������", {}, {PointBlankShoot}},
{"Rapid Shoot", "������� �������", {13}, {PointBlankShoot}},
{"Rapid Reload", "������� �����������"},
{"Shield Profeciency", "�������� �����"},
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
{"Greatweapon Proficiency", "�������� ������� �������", {ProficiencyLongsword}},
{"Heavy Crossbow Proficiency", "�������� ������� ���������", {}},
{"Mace Proficiency", "�������� �������", {}},
{"Simple weapon Proficiency", "�������� ������� �������", {}},
{"Spear Proficiency", "�������� ������", {}},
{"Longbow Proficiency", "�������� ������� �����", {}},
{"Longsword Proficiency", "�������� ������� �����", {}},
{"Shortbow Proficiency", "�������� �������� �����", {}},
{"Scimitar Proficiency", "�������� ������", {}},
{"Shortsword Proficiency", "�������� �������� �����", {}},
{"Bastardsword Proficiency", "�������� ���������� �����", {ProficiencyLongsword}},
{"Catana Proficiency", "�������� �������", {}},
{"Waraxe Proficiency", "�������� ������� �������", {ProficiencyAxe}},
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
assert_enum(feat, LastFeat);
getstr_enum(feat);

bool creature::isallow(feat_s id) const {
	for(auto e = Strenght; e <= Charisma; e = (ability_s)(e+1)) {
		auto value = feat_data[id].ability[e];
		if(value && getr(e) < value)
			return false;
	}
	for(auto e : feat_data[id].prerequisit) {
		if(e && !is(e))
			return false;
	}
	if(feat_data[id].base_attack && getbab() < feat_data[id].base_attack)
		return false;
	if(feat_data[id].character_level && getcharlevel() < feat_data[id].character_level)
		return false;
	if(feat_data[id].prerequisit_special)
		return false;
	return true;
}

bool creature::isallow(feat_s id, const unsigned char* ability, char character_level, char base_attack) {
	for(auto e = Strenght; e <= Charisma; e = (ability_s)(e + 1)) {
		auto value = feat_data[id].ability[e];
		if(value && ability[e] < value)
			return false;
	}
	if(feat_data[id].base_attack && base_attack < feat_data[id].base_attack)
		return false;
	if(feat_data[id].character_level && character_level < feat_data[id].character_level)
		return false;
	return true;
}

static void add_description(stringbuilder& sb, feat_s id, const char* prefix = 0) {
	if(feat_data[id].text && feat_data[id].text[0]) {
		if(prefix)
			sb.add(prefix);
		sb.add(feat_data[id].text);
	}
}

static void add_required(stringbuilder& sb, feat_s id) {
	auto p = sb.getpos();
	auto h = "\n\n[�������:] ";
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i+1)) {
		if(!feat_data[id].ability[i])
			continue;
		sb.sep(h, p);
		sb.add("%1 %2i", getstr(i), feat_data[id].ability[i]);
	}
	for(auto e : feat_data[id].prerequisit) {
		if(!e)
			break;
		sb.sep(h, p);
		sb.add(getstr(e));
	}
	if(feat_data[id].base_attack) {
		sb.sep(h, p);
		sb.add("������� ����� %+1i", feat_data[id].base_attack);
	}
	if(feat_data[id].character_level) {
		sb.sep(h, p);
		sb.add("������� ��������� %+1i+", feat_data[id].character_level);
	}
	if(!sb.ispos(p))
		sb.add(".");
}

static void add_header(stringbuilder& sb, const char* header, const char* text) {
	if(!text)
		return;
	if(header)
		sb.add("\n\n[%1:] ", header);
	sb.add(text);
}

template<> void getrule<feat_s>(stringbuilder& sb, feat_s id) {
	add_description(sb, id);
	add_header(sb, "������������", feat_data[id].benefit);
	add_required(sb, id);
}

void add_feat(stringbuilder& sb, feat_s id) {
	sb.add("\n\n[**%1**]", getstr(id));
	add_description(sb, id, ": ");
}

//void get_race_description(char* result, int id) {
//	char* p;
//	szprint(result, "###%1\n", bsgets(id, Name));
//	const char* pb = races[id - FirstRace].benefit;
//	// Descritpion
//	if(pb) {
//		zcat(result, pb);
//		zcat(result, "\n");
//	}
//	// Abilities
//	p = zend(result);
//	for(int i = FirstAbility; i <= LastAbility; i++) {
//		int v = get_race_modifier(id, i);
//		if(v)
//			szprint(strnew(p, "[Abilities]:\n"), " %1 %+2i\n", bsgets(i, Name), v);
//	}
//	szprint(zend(p), "[Base speed]: %1i ft.\n", get_base_speed(id));
//	// Proficiency
//	p = zend(result);
//	for(int i = ProfeciencyArmor; i <= ProficiencyUnarmed; i++) {
//		int v = get_race(id, i);
//		if(v)
//			szprint(strnew(p, "[Proficiency]:\n"), " %1\n", bsgets(i, Name), v);
//	}
//	// Racial bonus
//	p = zend(result);
//	for(int i = FirstSkill; i <= LastSkill; i++) {
//		int v = get_race(id, i);
//		if(v)
//			szprint(strnew(p, "[Racial bonus]:\n"), " %1 %+2i\n", bsgets(i, Name), v);
//	}
//	// Best class
//	if(true) {
//		int v = get_race(id, BestClass);
//		if(v) {
//			if(v == Class)
//				szprint(zend(p), "[Favored class]: Any\n", bsgets(v, Name));
//			else
//				szprint(zend(p), "[Favored class]: %1\n", bsgets(v, Name));
//		}
//	}
//}

//void get_skill_description(char* result, int id) {
//	szprint(result, "###%1\n", bsgets(id, Name));
//	const char* pb = get_benefit(skills, id);
//	const char* ps = get_special(skills, id);
//	szprint(zend(result), "[Key Ability]: ");
//	int count = 0;
//	int ka = get_skill_ability(id);
//	if(ka) {
//		count++;
//		szprint(zend(result), "%1", bsgets(ka, Name));
//	}
//	if(is_skill(id, ArmorCheckPenalty)) {
//		if(count++)
//			zcat(result, "; ");
//		zcat(result, "Armor check penalty");
//	}
//	if(is_skill(id, TrainedOnly)) {
//		if(count++)
//			zcat(result, "; ");
//		zcat(result, "Trained only");
//	}
//	zcat(result, "\n");
//	if(pb && pb[0]) {
//		zcat(result, pb);
//		zcat(result, "\n");
//	}
//	if(ps && ps[0]) {
//		zcat(result, "[Special]: ");
//		zcat(result, ps);
//		zcat(result, "\n");
//	}
//}