#include "main.h"

static struct alignment_info {
	const char*		id;
	const char*		name;
} alignment_data[] = {{"No alignment", "��� ������������"},
{"Lawful Good", "��������������� ������"},
{"Neutral Good", "���������� ������"},
{"Chaotic Good", "�������� ������"},
{"Lawful Neutral", "��������������� �����������"},
{"True Neutral", "�����������"},
{"Chaotic Neutral", "�������� �����������"},
{"Lawful Evil", "��������������� ����"},
{"Neutral Evil", "���������� ����"},
{"Chaotic Evil", "�������� ����"},
};
assert_enum(alignment, ChaoticEvil);
getstr_enum(alignment);