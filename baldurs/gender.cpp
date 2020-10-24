#include "main.h"

BSDATA(genderi) = {{"Transgender", "������������", "�������� ������������ ���� �� ����� ������������ ��� �� ������� ����� ������."},
{"Male", "�������", "������� ������ ������� ��� ������� � � ��������� ������� �������� �����������."},
{"Female", "�������", "������� ������ ������ ������ � ������������ ����������� ������ ������, ������� ������ ����."},
};
assert_enum(genderi, Female)

template<> void	getrule<gender_s>(stringbuilder& sb, gender_s value) {
	sb.add(bsdata<gender_s>::elements[value].text);
}