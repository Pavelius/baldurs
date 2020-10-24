#include "main.h"

BSDATA(genderi) = {{"Transgender", "Транссексуал", "Существа неизвестного пола не могут размножаться или их природа чужда земной."},
{"Male", "Мужчина", "Мужчины обычно сильнее чем женщины и в бошинстве обществ являются доминирущим."},
{"Female", "Женщина", "Женщины обычно мудрее мужчин и предпочитают действовать чужими руками, избегая грубой силы."},
};
assert_enum(genderi, Female)

template<> void	getrule<gender_s>(stringbuilder& sb, gender_s value) {
	sb.add(bsdata<gender_s>::elements[value].text);
}