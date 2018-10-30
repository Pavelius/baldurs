#include "main.h"

race_info race_data[] = {{"No Race", "��� ����", {{}}},
{"Dwarf", "������", {0, 0, 2, 0, 0, -2}, {{Appraise, 2}, {CraftWeapon, 2}, {CraftArmor, 2}}, {Infravision, HateGoblinoids, Stability, SaveBonusVsPoison, SaveBonusVsSpell, Stonecunning}, 0, "������� �������� �� ���� ���������� � ������� ���������, �� ������������ ������������� ���������� � ���������� ������������, �� ������� �������� �����, � �� ����������� ���������� ������� ���������� ���. �� ������������ �����������, ������������� ������ ���, �������� �� ���� ������������ ���������, ������� ������ ������� ���� ���� ����� ���� �� � ���, ��� ���������� ��������."},
{"Elf", "����", {0, 2, -2, 0, 0, 0}, {{Listen, 2}, {Spot, 2}, {Search, 2}}, {ImmunityToSleepSpell, SaveBonusVsEnchantment, Infravision, ProficiencyLongbow, ProficiencyLongsword, ProficiencyShortsword, ProficiencyShortsword, FindSecretDoors}, 0, "����� ������ ��������������� � ������ �����, �� ������ ��������� ���� ��� **� ������**. ��� �������� �� ����: ������, �����, �����, ������ � ���������� ���������. ������ �������� ���� ��������� � ������� �������. ����� ��������� �������� �� ������ �������, ���, ���������� ���� ������� ���������, ������������ ������ ��������� � ������, ������ � ��������� ������� ���������."},
{"Gnome", "����", {-2, 0, 2, 0, 0, 0}, {{Listen, 2}, {CraftAlchemy, 2}}, {Infravision}, 0, "����� ����� ������� � ��������� ���, �������, �������� � ������������. ������� ����� �������� ������ �� �� ������, ��� ������������ ���������� ����� ����� ����, ���� � ��������������� �����, ��� ��������, ��������� �������, ��� ������� �������� ���������� ��������, �� ���� ���������� �� ���, ����� ������ ����."},
{"Half-elf", "����-����", {}, {{Listen, 1}, {Spot, 1}, {Search, 1}, {Diplomacy, 2}, {GatherInformation, 2}}, {Infravision, SaveBonusVsEnchantment, ImmunityToSleepSpell}, 0, "���� � ����� ����� ����� �������, ������ ���������� ������� �����, ����� ��������� ������. ��� ����� ����� ����������, ��� ��� ����� ����� ����� �������, �� � ��� ������� ������� �������� � �������������� ����.\n\n����� ���������� ������. ���������� �������, ��������� ��������� ����� ������, �������� �������� � ������ ��� �����������. ��� ���������� ��������� ������� �� ���� ���, ���������� ����� ������� ��������� ���������� �������� � ���������, ��� ���� ����������. � ���� ��� ������ �� �������, ��������� ���������� �������, �� ���������� ���������� ��� ��� ����� ����� ���������� ������. ������ ��������� �������� ���� ���������� ���, ����� �� � ��� ����������� ������ �� ���������, ��� ���� ���� ������ ����� �����. � ������ �������, ���������� ����� �����, �������� ��������, ��� ������� �� ����������: ����� �����������, ��������������, ����� �����������, � ��������� �����������. ��������� ��������� ��������� ����� �������� �� �����, � �� ����� ������ ������� ���� � ���, ��� ��� ���������� �� ������. ����������� ������� ���� ����� � ����� � ������� �������, �� �������� ��, ��� ��� ���� ����� ��������� ���� ������������ � ���� ����� �����."},
{"Half-orc", "����-���", {2, 0, 0, -2, 0, -2}, {}, {}, 0, "� ����� �����, ���������� ������� ����� � ����� ��������� � ������ ������� �������, ��������, ���� � ������ � ������, � ������ �� ������� ���������. ��������, ��, ��� ������� �� �������� � ����, �� �� �� ������������ �� ��, �� ������ ��������. ��������� �� ������ �������� �������� ���� ���, � ������������ � �������������� �����, ������� � ����� �� ����� ���� ����������, ���������, � ������ �������."},
{"Halfling", "������", {-2, 2, 0, 0, 0, 0}, {{Climb, 2}, {Jump, 2}, {MoveSilently, 2}, {Listen, 2}}, {Lucky, SaveBonusVsFear, PreciseThrower}, 0, "��������� - �����, �������� ��������, ��������� �����. ��������� ��������� � ����� �����, ������� ����� ������������� ������� ��� ������ �����. ����� ����� ��� ������������, �� ������ ��������� ��������� �� ��� � ����������� � ������������. � ����������� �� �����, ��������� ����� ���� ���������, ������������� (���� ����� ������) ����������, ��� �� ���� ������, ������ ����������� ��������� ���-�� �� ������ � ������������ �� ����� ����. �� ������� �����, ��������� �������� � ����������, ���������� � ����� ��������."},
{"Human", "�������", {}, {}, {}, 1, "����������� ����� � ������ ��������������, �����������, ����������������, ��������, � ������ ����� ����������� � ���������� ��������. ���������, ����� ����� - ��� ����� ������������� ����������, ����������, ����������� � ������������ ���. ������� ��� �������, ������- ��� ����������, ������� ��� �������, ����������� ��� ��������������, ��������� ��� ����������, ��� ��� ����� ������������� �����."},
};
assert_enum(race, Human);
getstr_enum(race);

void add_feat(stringbuilder& sb, feat_s id);

static void add_ability(stringbuilder& sb, race_s id) {
	auto count = 0;
	for(auto i = Strenght; i <= Charisma; i=(ability_s)(i+1)) {
		if(race_data[id].abilities[i] == 0)
			continue;
		if(count == 0)
			sb.addh(getstr(Ability));
		sb.addn("%1 %+2i", getstr(i), race_data[id].abilities[i]);
		count++;
	}
}

static void add_skills(stringbuilder& sb, race_s id) {
	auto count = 0;
	for(auto i = FirstSkill; i <= LastSkill; i = (skill_s)(i + 1)) {
		if(race_data[id].skills[i] == 0)
			continue;
		if(count == 0)
			sb.addh(getstr(Skill));
		sb.addn("%1 %+2i", getstr(i), race_data[id].skills[i]);
		count++;
	}
}

static void add_feats(stringbuilder& sb, race_s id) {
	for(auto i : race_data[id].feats) {
		if(i)
			add_feat(sb, i);
	}
}

template<> void getrule<race_s>(stringbuilder& sb, race_s id) {
	sb.add("##%1\n", race_data[id].name);
	sb.add(race_data[id].text);
	add_ability(sb, id);
	add_skills(sb, id);
	add_feats(sb, id);
}