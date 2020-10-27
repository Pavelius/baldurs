#include "main.h"

static_assert(sizeof(variant) == sizeof(int), "variant must be sizeof(int)");

#define BSVRN(T) &bsdata<T>::source, {FO(T, name), FO(T, text)}

BSDATA(varianti) = {{"NoVariant", "Нет"},
{"Ability", "Способности", {Strenght, Charisma}, BSVRN(abilityi)},
{"Alignment", "Мировозрение", {LawfulGood, ChaoticEvil}, BSVRN(alignmenti)},
{"Apearance", "Внешность"},
{"Class", "Класс", {Bard, Wizard}, BSVRN(classi)},
{"Container", "Контейнер", {}, &bsdata<container>::source},
{"Creature", "Существо", {}, &bsdata<creature>::source},
{"Diety", "Божество", {GodBane, GodUmberly}, BSVRN(dietyi)},
{"Door", "Дверь", {}, &bsdata<door>::source},
{"Gender", "Пол", {Male, Female}, BSVRN(genderi)},
{"Feat", "Особенность", {FirstFeat, LastFeat}, BSVRN(feati), {FO(feati, benefit), FO(feati, normal)}},
{"Item", "Предмет", {}, BSVRN(itemi)},
{"ItemCont", "Предмет в контейнере", {}, &bsdata<itemcont>::source},
{"ItemGround", "Предмет на земле", {}, &bsdata<itemground>::source},
{"Name", "Имя"},
{"Position", "Позиция"},
{"Race", "Раса", {FirstRace, LastRace}, BSVRN(racei)},
{"Region", "Регион", {}, &bsdata<region>::source},
{"School", "Школа", {}, BSVRN(schooli)},
{"Skills", "Навыки", {FirstSkill, LastSkill}, BSVRN(skilli)},
{"Spells", "Заклинания", {FirstSpell, LastSpell}, BSVRN(spelli)},
{"Finish", "Готово"},
{"Variant", "Вариант", {}, BSVRN(varianti)},
};
assert_enum(varianti, Variant)

template<> const char* getstr<variant>(variant id) {
	auto& ei = bsdata<varianti>::elements[id.type];
	if(!ei.locale[0] || !ei.source)
		return "Нет имени";
	auto p = ei.source->ptr(id.value);
	auto pn = *((const char**)((char*)p + ei.locale[0]));
	return pn ? pn : "Нет имени";
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
	auto& ei = bsdata<varianti>::elements[type];
	if(type == Feat)
		bsdata<feati>::elements[value].addhead(sb);
	else if(ei.locale[1] && ei.source) {
		auto p = ei.source->ptr(value);
		auto pd = *((const char**)((char*)p + ei.locale[1]));
		if(pd && pd[0])
			sb.add(pd);
		else
			sb.add("Описание отсутствует и его необходимо добавить.");
	}
	switch(type) {
	case Ability: bsdata<abilityi>::elements[value].addinfo(sb); break;
	case Class: bsdata<classi>::elements[value].addinfo(sb); break;
	case Feat: bsdata<feati>::elements[value].addinfo(sb); break;
	case Race: bsdata<racei>::elements[value].addinfo(sb); break;
	case Skill: bsdata<skilli>::elements[value].addinfo(sb); break;
	case Spell: bsdata<spelli>::elements[value].addinfo(sb); break;
	}
}