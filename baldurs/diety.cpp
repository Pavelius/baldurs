#include "main.h"

static struct diety_info {
	const char*	id;
	const char*	name;
} diety_data[] = {{"No diety"},
// Gods
{"Bane", "����"},
{"Chauntea", "������"},
{"Illmater", "���������"},
{"Gruumsh", "�����"},
{"Helm", "�����"},
{"Kelemvor", "��������"},
{"Kossuth", "������"},
{"Lathander", "��������"},
{"Lolth", "����"},
{"Mask", "�����"},
{"Mystra", "������"},
{"Moradin", "�������"},
{"Oghma", "����"},
{"Shar", "���"},
{"Tempus", "������"},
{"Tymora", "������"},
{"Tyr", "���"},
{"Umberly", "��������"},
};
assert_enum(diety, GodUmberly);
getstr_enum(diety);