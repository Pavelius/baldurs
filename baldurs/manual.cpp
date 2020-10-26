#include "main.h"

bool isallowstep(variant_s v, variant_s c) {
	for(auto e : bsdata<genstepi>()) {
		if(e.step == v)
			break;
		if(e.step == c)
			return true;
	}
	return false;
}

static void adds(stringbuilder& sb, const char* header, const char* format, ...) {
	sb.addsep('\n');
	if(header)
		sb.add("[**%1**]: ", header);
	sb.addv(format, xva_start(format));
}

static void add(stringbuilder& sb, const char* header, const char* format, ...) {
	sb.addsep('\n');
	sb.add("\n[**%1**]: ", header);
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

static void add_feat_item(stringbuilder& sb, feat_s id, int index = 0) {
	auto p = sb.get();
	for(auto& e : bsdata<itemi>()) {
		if(e.feat[index] == id) {
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
	} else if(id >= FocusAxes && id <= FocusSwords) {
		if(prefix)
			sb.add(prefix);
		sb.add("+1 к атаке, если исползуете такое оружие как ");
		add_feat_item(sb, id, 1);
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
	for(auto e : ei.prerequisits) {
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

static void add(stringbuilder& sb, const std::initializer_list<feat_s>& source) {
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

static void add(stringbuilder& sb, const char* header, const varianta& source) {
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

static void add(stringbuilder& sb, const char* header, item_s v1, item_s v2, const aref<feat_s>& feats, const char* empthy) {
	if(!feats) {
		if(empthy)
			add(sb, header, empthy);
		return;
	}
	varianta source;
	for(auto v = v1; v <= v2; v = (item_s)(v + 1)) {
		auto& vi = bsdata<itemi>::elements[v];
		if(vi.feat[0] && !feats.is(vi.feat[0]))
			continue;
		source.add(v);
	}
	source.sort();
	add(sb, header, source);
}

void creature::add(stringbuilder& sb, const aref<variant>& elements, const char* title) const {
	if(!elements)
		return;
	sb.addh(title);
	for(auto e : elements) {
		sb.addn(getstr(e));
		switch(e.type) {
		case Skill: sb.add(" %+1i (%2i)", get((skill_s)e.value), skills[e.value]); break;
		case Ability: sb.add(": %1i", ability[e.value]); break;
		}
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
	sb.add(ei.text);
	add(sb, "Кость хитов", "d%1i", ei.hd);
	if(ei.spell_ability)
		add(sb, "Способность заклинаний", getstr(ei.spell_ability));
	add(sb, "Очков навыков", "%1i/уровень", ei.skill_points);
	add(sb, "Классовые навыки", ei.class_skills);
	add(sb, "Доступное оружие", Club, Katana, ei.weapon_proficiency, "не использует оружие");
	add(sb, "Доступная броня", LeatherArmor, Helm, ei.armor_proficiency, "не носит броню");
}

template<> void	getrule<spell_s>(stringbuilder& sb, spell_s value) {
	auto& ei = bsdata<spelli>::elements[value];
	sb.add("Необходимо добавить описание заклинания");
	add(sb, "Школа", getstr(ei.school));
	add(sb, "Уровень", ei.levels);
}

void creature::addinfo(stringbuilder& sb, variant_s step) const {
	sb.clear();
	if(isallowstep(step, Name))
		adds(sb, getstr(Name), getname());
	if(isallowstep(step, Gender))
		adds(sb, getstr(Gender), getstr(getgender()));
	if(isallowstep(step, Race))
		adds(sb, getstr(Race), getstr(getrace()));
	if(isallowstep(step, Class))
		adds(sb, getstr(Class), getstr(getclass()));
	if(isallowstep(step, Alignment))
		adds(sb, 0, getstr(getalignment()));
	if(isallowstep(step, Ability))
		add(sb, Strenght, Charisma, "Атрибуты", false);
	if(isallowstep(step, Skill)) {
		add(sb, FirstSkill, LastSkill, "Навыки");
		add(sb, FirstFeat, WhirlwindAttack, "Особенности");
		add(sb, ProficiencyAxe, ProficiencyWaraxe, "Доступность оружия");
	}
}

static void addm(stringbuilder& sb, const char* header) {
	sb.add("\n");
	sb.add(header);
	sb.add(": ");
}

static void addm(stringbuilder& sb, const char* header, const char* format, ...) {
	addm(sb, header);
	sb.addv(format, xva_start(format));
}

static void addm(stringbuilder& sb, const attacki& e) {
	if(!e)
		return;
	addm(sb, "Повреждения");
	if(e.b)
		sb.add("%1id%2i%+3i", e.c, e.d, e.b);
	else
		sb.add("%1id%2i", e.c, e.d);
}

void item::addinfo(stringbuilder& sb) const {
	auto& ei = bsdata<itemi>::elements[type];
	if(ei.text)
		sb.add(ei.text);
	else
		sb.add("Необходимо добавить описание предмета.");
	auto bonus = getbonus();
	sb.add("\n\n");
	sb.add("[Характеристики]");
	addm(sb, getattack());
	addm(sb, "Вес", "%1i фунтов", ei.weight);
}