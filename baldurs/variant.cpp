#include "main.h"

BSDATA(varianti) = {{""},
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
{"Finish", "Закончить"},
{"Variant", "Вариант"},
};
assert_enum(varianti, Variant)

template<> const char* getstr<variant>(variant id) {
	switch(id.type) {
	case Alignment: return getstr((alignment_s)id.value);
	case Ability: return getstr((ability_s)id.value);
	case Class: return getstr((class_s)id.value);
	case Gender: return getstr((gender_s)id.value);
	case Race: return getstr((race_s)id.value);
	case Skill: return getstr((skill_s)id.value);
	case Spell: return getstr((spell_s)id.value);
	case Feat: return getstr((feat_s)id.value);
	case Variant: return getstr((variant_s)id.value);
	default: return "Нет варианта";
	}
}

int compare_variant(const void* v1, const void* v2) {
	auto s1 = getstr(*((variant*)v1));
	auto s2 = getstr(*((variant*)v2));
	return strcmp(s1, s2);
}