#include "main.h"

static void add(stringbuilder& sb, const char* header, const char* format, ...) {
	sb.add("\n\n[**%1**]: ", header);
	sb.addv(format, xva_start(format));
}

static void add(stringbuilder& sb, const char* header, aref<skill_s> source) {
	auto count = 0;
	for(auto i : source) {
		if(count == 0)
			sb.add("\n\n[**%1**]: ", header);
		else
			sb.add(", ");
		sb.add(getstr(i));
		count++;
	}
}

static void add_feat_item(stringbuilder& sb, feat_s id) {
	auto p = sb.get();
	for(auto& e : bsdata<itemi>()) {
		if(e.feat[0] == id) {
			sb.sep(0, p);
			sb.add(e.name);
		}
	}
	if(!sb.ispos(p))
		sb.add(".");
}

static void add_description(stringbuilder& sb, feat_s id, const char* prefix = 0) {
	auto& ei = bsdata<feati>::elements[id];
	if(id >= ProficiencyAxe && id <= ProficiencyWaraxe) {
		if(prefix)
			sb.add(prefix);
		sb.add("Вы умеете использовать такое оружие как ");
		add_feat_item(sb, id);
	} else if(ei.text && ei.text[0]) {
		if(prefix)
			sb.add(prefix);
		sb.add(ei.text);
	}
}

static void add_required(stringbuilder& sb, feat_s id) {
	auto& ei = bsdata<feati>::elements[id];
	auto p = sb.get();
	auto h = "\n\n[Требует:] ";
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		if(!ei.ability[i])
			continue;
		sb.sep(h, p);
		sb.add("%1 %2i", getstr(i), ei.ability[i]);
	}
	for(auto e : ei.prerequisit) {
		if(!e)
			break;
		sb.sep(h, p);
		sb.add(getstr(e));
	}
	if(ei.base_attack) {
		sb.sep(h, p);
		sb.add("базовая атака %+1i", ei.base_attack);
	}
	if(ei.character_level) {
		sb.sep(h, p);
		sb.add("уровень персонажа %+1i+", ei.character_level);
	}
	if(!sb.ispos(p))
		sb.add(".");
}

static void add_header(stringbuilder& sb, const char* header, const char* text) {
	if(!text)
		return;
	if(header)
		sb.add("\n\n[%1:] ", header);
	sb.add(text);
}

static void add_feat(stringbuilder& sb, feat_s id) {
	sb.add("\n\n[**%1**]", getstr(id));
	add_description(sb, id, ": ");
}

static void add_ability(stringbuilder& sb, race_s id) {
	auto count = 0;
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		if(bsdata<racei>::elements[id].abilities[i] == 0)
			continue;
		if(count == 0)
			sb.add("\n\n[**%1**]\n", getstr(Ability));
		sb.addn("%1 %+2i", getstr(i), bsdata<racei>::elements[id].abilities[i]);
		count++;
	}
}

static void add_skills(stringbuilder& sb, race_s id) {
	auto count = 0;
	for(auto i = FirstSkill; i <= LastSkill; i = (skill_s)(i + 1)) {
		if(bsdata<racei>::elements[id].skills[i] == 0)
			continue;
		if(count == 0)
			sb.add("\n\n[**%1**]\n", getstr(Skill));
		sb.addn("%1 %+2i", getstr(i), bsdata<racei>::elements[id].skills[i]);
		count++;
	}
}

static void add(stringbuilder& sb, const aref<feat_s>& source) {
	for(auto i : source) {
		if(i)
			add_feat(sb, i);
	}
}

static void add(stringbuilder& sb, const char* header, const classa& source) {
	auto count = 0;
	for(auto i = FirstClass; i <= LastClass; i = (class_s)(i+1)) {
		if(!source.data[i])
			continue;
		if(count == 0)
			sb.add("\n\n[**%1**]: ", header);
		else
			sb.add(", ");
		sb.add("%1 %2i", getstr(i), source.data[i]);
		count++;
	}
}

template<> void getrule<race_s>(stringbuilder& sb, race_s id) {
	sb.add(bsdata<racei>::elements[id].text);
	add_ability(sb, id);
	add_skills(sb, id);
	add(sb, bsdata<racei>::elements[id].feats);
}

template<> void getrule<feat_s>(stringbuilder& sb, feat_s id) {
	add_description(sb, id);
	add_header(sb, "Преемущество", bsdata<feati>::elements[id].benefit);
	add_header(sb, "Обычно", bsdata<feati>::elements[id].normal);
	add_required(sb, id);
}

template<> void	getrule<class_s>(stringbuilder& sb, class_s value) {
	auto& ei = bsdata<classi>::elements[value];
	sb.add("Необходимо добавить описание класса");
	add(sb, "Кость хитов", "d%1i", ei.hd);
	add(sb, "Очков навыков", "%1i/уровень", ei.skill_points);
	add(sb, "Классовые навыки", ei.class_skills);
	add(sb, ei.weapon_proficiency);
	add(sb, ei.armor_proficiency);
}

template<> void	getrule<spell_s>(stringbuilder& sb, spell_s value) {
	auto& ei = bsdata<spelli>::elements[value];
	sb.add("Необходимо добавить описание заклинания");
	add(sb, "Школа", getstr(ei.school));
	add(sb, "Уровень", ei.levels);
}