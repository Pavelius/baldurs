#include "main.h"

static_assert(sizeof(variant) == sizeof(int), "variant must be sizeof(int)");

BSDATA(varianti) = {{""},
{"Ability", "�����������", &bsdata<abilityi>::source},
{"Alignment", "������������", &bsdata<alignmenti>::source},
{"Apearance", "���������"},
{"Class", "�����", &bsdata<classi>::source},
{"Container", "���������", &bsdata<container>::source},
{"Creature", "��������", &bsdata<creature>::source},
{"Door", "�����", &bsdata<door>::source},
{"Gender", "���", &bsdata<genderi>::source},
{"Feat", "�����������", &bsdata<feati>::source},
{"Item", "�������", &bsdata<itemi>::source, {FO(itemi, name), FO(itemi, text)}},
{"ItemCont", "������� � ����������", &bsdata<itemcont>::source},
{"ItemGround", "������� �� �����", &bsdata<itemground>::source},
{"Name", "���"},
{"Position", "�������"},
{"Race", "����", &bsdata<racei>::source},
{"Region", "������", &bsdata<region>::source},
{"Skills", "������", &bsdata<skilli>::source},
{"Spells", "����������", &bsdata<spelli>::source},
{"Finish", "������"},
{"Variant", "�������"},
};
assert_enum(varianti, Variant)

template<> const char* getstr<variant>(variant id) {
	switch(id.type) {
	case Alignment: return getstr((alignment_s)id.value);
	case Ability: return getstr((ability_s)id.value);
	case Class: return getstr((class_s)id.value);
	case Item: return getstr((item_s)id.value);
	case Gender: return getstr((gender_s)id.value);
	case Race: return getstr((race_s)id.value);
	case Skill: return getstr((skill_s)id.value);
	case Spell: return getstr((spell_s)id.value);
	case Feat: return getstr((feat_s)id.value);
	case Variant: return getstr((variant_s)id.value);
	default: return "��� ��������";
	}
}

int compare_variant(const void* v1, const void* v2) {
	auto s1 = getstr(*((variant*)v1));
	auto s2 = getstr(*((variant*)v2));
	return strcmp(s1, s2);
}

variant::variant(const void* v) : type(NoVariant), value(0) {
	for(auto& e : bsdata<varianti>()) {
		if(!e.source)
			continue;
		auto i = e.source->indexof(v);
		if(i == -1)
			continue;
		type = (variant_s)(&e - bsdata<varianti>::elements);
		value = i;
	}
}

void* variant::getpointer(variant_s t) const {
	return (type == t) ? bsdata<varianti>::elements[t].source->ptr(value) : 0;
}

void variant::addinfo(stringbuilder& sb) const {
	switch(type) {
	case Alignment: getrule(sb, (alignment_s)value); break;
	case Class: getrule(sb, (class_s)value); break;
	case Feat: getrule(sb, (feat_s)value); break;
	case Gender: getrule(sb, (gender_s)value); break;
	case Spell: getrule(sb, (spell_s)value); break;
	case Race: getrule(sb, (race_s)value); break;
	}
}