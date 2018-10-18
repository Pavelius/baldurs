#include "main.h"

static struct variant_info {
	const char*		id;
	const char*		name;
} variant_data[] = {{""},
{"Ability", "Способности"},
{"Alignment", "Мировозрение"},
{"Apearance", "Внешность"},
{"Class", "Класс"},
{"Gender", "Пол"},
{"Feat", "Особенность"},
{"Item", "Предмет"},
{"Race", "Раса"},
{"Skills", "Навыки"},
{"Spells", "Заклинания"},
{"Name", "Имя"},
};
assert_enum(variant, Name);
getstr_enum(variant);

template<> const char* getstr<variant>(variant id) {
	switch(id.type) {
	case Alignment: return getstr(id.alignment);
	case Ability: return getstr(id.ability);
	case Class: return getstr(id.clas);
	case Gender: return getstr(id.gender);
	case Race: return getstr(id.race);
	case Skill: return getstr(id.skill);
	case Spell: return getstr(id.spell);
	case Feat: return getstr(id.feat);
	default: return "Нет варианта";
	}
}

template<> void getrule<variant>(stringbuilder& sb, variant id) {
	switch(id.type) {
	case Race: getrule(sb, id.race); break;
	}
}

int compare_variant(const void* v1, const void* v2) {
	auto s1 = getstr(*((variant*)v1));
	auto s2 = getstr(*((variant*)v2));
	return strcmp(s1, s2);
}