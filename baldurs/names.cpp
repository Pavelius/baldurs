#include "main.h"

static struct name_info {
	race_s		race;
	gender_s	gender;
	const char*	name;
} name_data[] = {{Dwarf, Male, "�����"},
{Dwarf, Male, "��������"},
{Dwarf, Male, "������"},
{Dwarf, Male, "�����"},
{Dwarf, Male, "�������"},
{Dwarf, Male, "�������"},
{Dwarf, Male, "������"},
{Dwarf, Male, "����"},
{Dwarf, Male, "�������"},
{Dwarf, Male, "����"},
{Dwarf, Male, "������"},
{Dwarf, Male, "����"},
{Dwarf, Male, "�������"},
{Dwarf, Male, "�������"},
{Dwarf, Male, "�����"},
{Dwarf, Male, "�����"},
{Dwarf, Male, "�������"},
{Dwarf, Male, "�����"},
{Dwarf, Male, "�������"},
{Dwarf, Male, "�������"},
{Dwarf, Male, "������"},
{Dwarf, Male, "�����"},
{Dwarf, Male, "������"},
{Dwarf, Male, "�������"},
{Dwarf, Male, "�������"},
{Dwarf, Male, "�������"},
{Dwarf, Male, "�����"},
{Dwarf, Male, "������"},
{Dwarf, Male, "�����"},
{Dwarf, Male, "�������"},
//
{Dwarf, Female, "�����"},
{Dwarf, Female, "�������"},
{Dwarf, Female, "������"},
{Dwarf, Female, "��������"},
{Dwarf, Female, "������"},
{Dwarf, Female, "������"},
{Dwarf, Female, "�����"},
{Dwarf, Female, "����"},
{Dwarf, Female, "�����"},
{Dwarf, Female, "�������"},
{Dwarf, Female, "��������"},
{Dwarf, Female, "�������"},
{Dwarf, Female, "�������"},
{Dwarf, Female, "������"},
{Dwarf, Female, "�����"},
{Dwarf, Female, "�������"},
{Dwarf, Female, "������"},
{Dwarf, Female, "��������"},
{Dwarf, Female, "��������"},
{Dwarf, Female, "�������"},
{Dwarf, Female, "����"},
{Dwarf, Female, "������"},
{Dwarf, Female, "�����"},
//
{Elf, Male, "�����"},
{Elf, Male, "�������"},
{Elf, Male, "�������"},
{Elf, Male, "����"},
{Elf, Male, "�����"},
{Elf, Male, "�����"},
{Elf, Male, "�������"},
{Elf, Male, "�����"},
{Elf, Male, "���������"},
{Elf, Male, "��������"},
{Elf, Male, "��������"},
{Elf, Male, "������"},
{Elf, Male, "�������"},
{Elf, Male, "�������"},
{Elf, Male, "���������"},
{Elf, Male, "�������"},
{Elf, Male, "�����"},
{Elf, Male, "�������"},
{Elf, Male, "�����"},
{Elf, Male, "��������"},
{Elf, Male, "�������"},
{Elf, Male, "�������"},
{Elf, Male, "�����"},
{Elf, Male, "�������"},
{Elf, Male, "����"},
{Elf, Male, "�����"},
{Elf, Male, "�������"},
{Elf, Male, "�����"},
{Elf, Male, "������"},
//
{Elf, Female, "�����"},
{Elf, Female, "������"},
{Elf, Female, "�����������"},
{Elf, Female, "��������"},
{Elf, Female, "�������"},
{Elf, Female, "��������"},
{Elf, Female, "������"},
{Elf, Female, "�������"},
{Elf, Female, "�������"},
{Elf, Female, "���������"},
{Elf, Female, "��������"},
{Elf, Female, "�������"},
{Elf, Female, "�������"},
{Elf, Female, "��������"},
{Elf, Female, "�������"},
{Elf, Female, "������"},
{Elf, Female, "��������"},
{Elf, Female, "�������"},
{Elf, Female, "���"},
{Elf, Female, "�����"},
{Elf, Female, "�������"},
{Elf, Female, "�������"},
{Elf, Female, "�������"},
{Elf, Female, "�������"},
{Elf, Female, "���������"},
{Elf, Female, "���"},
{Elf, Female, "���������"},
{Elf, Female, "����"},
{Elf, Female, "�������"},
{Elf, Female, "����"},
//
{Human, Male, "�����"}, // ��������
{Human, Male, "���"},
{Human, Male, "����"},
{Human, Male, "������"},
{Human, Male, "����"},
{Human, Male, "�����"},
{Human, Male, "������"},
{Human, Male, "����"},
{Human, Male, "�����"},
{Human, Male, "������"},
{Human, Male, "������"},
{Human, Female, "������"},
{Human, Female, "����"},
{Human, Female, "����"},
{Human, Female, "��������"},
{Human, Female, "����"},
{Human, Female, "������"},
{Human, Female, "�����"},
{Human, Female, "����"},
{Human, Male, "����"},// ����������
{Human, Male, "����"},
{Human, Male, "���"},
{Human, Male, "������"},
{Human, Male, "���"},
{Human, Male, "������"},
{Human, Male, "����"},
{Human, Male, "�����"},
{Human, Male, "���"},
{Human, Male, "����"},
{Human, Male, "�����"},
{Human, Female, "�������"},
{Human, Female, "�����"},
{Human, Female, "������"},
{Human, Female, "�����"},
{Human, Female, "����"},
{Human, Female, "�����"},
{Human, Female, "��������"},
{Human, Female, "������"},
};

static aref<short unsigned> select(aref<short unsigned> result, gender_s gender, race_s race) {
	auto pb = result.data;
	auto pe = result.data + result.count;
	for(unsigned short i = 0; i < sizeof(name_data) / sizeof(name_data[0]); i++) {
		if(gender && name_data[i].gender != gender)
			continue;
		if(name_data[i].race != race)
			continue;
		if(pb < pe)
			*pb++ = i;
	}
	result.count = pb - result.data;
	return result;
}

const char* creature::random_name(gender_s gender, race_s race) {
	short unsigned source[128];
	if(race == HalfElf) {
		if(d100() < 50)
			race = Human;
		else
			race = Elf;
	}
	auto result = select(source, gender, race);
	return name_data[result.data[rand()%result.count]].name;
}

void creature::random_name() {
	name = random_name(gender, race);
}