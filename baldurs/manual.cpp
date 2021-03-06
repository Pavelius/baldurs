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

static void add_required(stringbuilder& sb, feat_s id) {
	auto& ei = bsdata<feati>::elements[id];
	auto p = sb.get();
	auto h = "\n\n[�������:] ";
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
		sb.add("������� ����� %+1i", ei.base_attack);
	}
	if(ei.character_level) {
		sb.sep(h, p);
		sb.add("������� ��������� %+1i+", ei.character_level);
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
	bsdata<feati>::elements[id].addhead(sb, ": ");
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
	for(auto i = FirstClass; i <= LastClass; i = (class_s)(i + 1)) {
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

static void addm(stringbuilder& sb, const char* header) {
	sb.addn(header);
	sb.add(": ");
}

static void addm(stringbuilder& sb, const char* header, const char* format, ...) {
	addm(sb, header);
	sb.addv(format, xva_start(format));
}

static void addv(stringbuilder& sb, const dicei& e) {
	if(e.b)
		sb.add("%1id%2i%+3i", e.c, e.d, e.b);
	else
		sb.add("%1id%2i", e.c, e.d);
}

static void adddamage(stringbuilder& sb, const attacki& ai) {
	addv(sb, ai.damage);
	sb.add(" ");
	sb.add(getstr(ai.damage.type));
	if(ai.weapon) {
		auto pp = ai.weapon->getpower();
		if(pp) {
			auto power = pp->power;
			if(power) {
				switch(power.type) {
				case DamageVariant:
					sb.add(" � 1d6 %1", getstr(power));
					break;
				case Race:
					sb.add(" � ������������� 2d6 ���� ���� %1", getstr(power));
					break;
				}
			}
		}
	}
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
static void add_skill_ability(stringbuilder& sb, const char* header, ability_s id) {
	varianta elements;
	for(auto e = FirstSkill; e <= LastSkill; e = (skill_s)(e + 1)) {
		if(bsdata<skilli>::elements[e].ability == id)
			elements.add(e);
	}
	elements.sort();
	add(sb, header, elements);
}

static void add_skill_synergy(stringbuilder& sb, const char* header, skill_s id) {
	varianta elements;
	for(auto e : bsdata<skilli>::elements[id].synergy) {
		if(e)
			elements.add(e);
	}
	elements.sort();
	add(sb, header, elements);
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
		add(sb, Strenght, Charisma, "��������", false);
	if(isallowstep(step, Skill)) {
		add(sb, FirstSkill, LastSkill, "������");
		add(sb, FirstFeat, WhirlwindAttack, "�����������");
		add(sb, ProficiencyAxe, ProficiencyWaraxe, "����������� ������");
	}
}

static void addm(stringbuilder& sb, const char* header, const feata& feats) {
	auto p = sb.get();
	for(auto i = FirstFeat; i <= LastFeat; i = (feat_s)(i + 1)) {
		if(!feats.is(i))
			continue;
		if(sb.ispos(p))
			sb.addn("%1: ", header);
		else
			sb.add(", ");
		sb.add(getstr(i));
	}
}

static void addweapon(stringbuilder& sb, const creature* p, item* pi, bool isoffhand) {
	attacki ai; p->get(ai, pi, isoffhand);
	if(!ai)
		return;
	if(!(*pi))
		return;
	sb.addn("%+1 %+2i �� ", "������� �����", ai.bonus);
	sb.add(pi->getname());
	sb.addn("��� ��������� ");
	adddamage(sb, ai);
}

static void addweapons(stringbuilder& sb, const creature* p, slot_s weapon, const char* header) {
	auto p1 = (item*)p->getwearr(weapon);
	auto p2 = (item*)p->getwearr((slot_s)(weapon + 1));
	if(!p1 || !p2)
		return;
	if(!(*p1) && !(*p2))
		return;
	sb.addh(header);
	addweapon(sb, p, p1, false);
	addweapon(sb, p, p2, true);
}

void item::addinfo(stringbuilder& sb) const {
	auto& ei = bsdata<itemi>::elements[type];
	if(ei.text && ei.text[0])
		sb.add(ei.text);
	else
		sb.add("���������� �������� �������� ��������.");
	auto bonus = getbonus();
	sb.add("\n\n[��������������]");
	if(ei.ai.deflect_critical)
		sb.addn("���� %1i%% ���������� ����������� ���� �� ��� � ������� ����.", ei.ai.deflect_critical);
	attacki ai = {}; apply(ai);
	if(ai.bonus)
		addm(sb, "����� � �����", "%+1i", ai.bonus);
	if(ai.damage) {
		addm(sb, "����");
		adddamage(sb, ai);
		addm(sb, "����������� ����");
		sb.add("��� ");
		if(ai.critical)
			sb.add("%1i-20", 20 - ai.critical);
		else
			sb.add("20", 20 - ai.critical);
		sb.add(" ���� �%1i", 2 + ai.multiplier);
	}
	auto ac = getac();
	if(ac)
		addm(sb, getstr(ArmorClass), "%+1i", ac);
	auto pi = getpower();
	if(pi) {
		if(pi->bonus)
			addm(sb, "��������� �����", "%+1i", pi->bonus);
		auto power = pi->power;
		if(power) {
		}
	}
	if(ei.feat[0])
		addm(sb, "�������", getstr(ei.feat[0]));
	addm(sb, "���", "%1i ������", ei.weight);
}

void spelli::addinfo(stringbuilder& sb) const {
	add(sb, "�����", getstr(school));
	add(sb, "�������", levels);
}

void skilli::addinfo(stringbuilder& sb) const {
	add_skill_synergy(sb, "��������� ��������", (skill_s)(this - bsdata<skilli>::elements));
}

void classi::addinfo(stringbuilder& sb) const {
	add(sb, "����� �����", "d%1i", hd);
	if(spell_ability)
		add(sb, "����������� ����������", getstr(spell_ability));
	add(sb, "����� �������", "%1i/�������", skill_points);
	add(sb, "��������� ������", class_skills);
	add(sb, "��������� ������", Club, Katana, weapon_proficiency, "�� ���������� ������");
	add(sb, "��������� �����", LeatherArmor, Helm, armor_proficiency, "�� ����� �����");
}

void racei::addinfo(stringbuilder& sb) const {
	auto id = (race_s)(this - bsdata<racei>::elements);
	add_ability(sb, id);
	add_skills(sb, id);
	add(sb, feats);
}

void feati::addhead(stringbuilder& sb, const char* prefix) const {
	auto id = (feat_s)(this - bsdata<feati>::elements);
	if(prefix)
		sb.add(prefix);
	if(id >= ProficiencyAxe && id <= ProficiencyWaraxe) {
		sb.add("�� ������ ������������ ����� ������ ��� ");
		add_feat_item(sb, id);
	} else if(id >= FocusAxes && id <= FocusSwords) {
		sb.add("+1 � �����, ���� ���������� ����� ������ ��� ");
		add_feat_item(sb, id, 1);
	} else if(text && text[0])
		sb.add(text);
}

void feati::addinfo(stringbuilder& sb) const {
	auto id = (feat_s)(this - bsdata<feati>::elements);
	add_header(sb, "������������", benefit);
	add_header(sb, "������", normal);
	add_required(sb, id);
}

void abilityi::addinfo(stringbuilder& sb) const {
	auto id = (ability_s)(this - bsdata<abilityi>::elements);
	add_skill_ability(sb, "������", id);
}

void creature::addinfo(stringbuilder& sb) const {
	sb.addh("������ - ������� ��������� %1i", getcharlevel());
	for(auto e = FirstClass; e <= LastClass; e = (class_s)(e + 1)) {
		if(classes[e])
			sb.addn("%1: %2i", getstr(e), classes[e]);
	}
	sb.addn("������� �����: �����");
	sb.addh("����");
	sb.addn("�������: %1i", experience);
	sb.addn("��������� �������: %1i", 2000);
	sb.addh("����");
	sb.addn(getstr(getrace()));
	sb.addh("������������");
	sb.addn(getstr(alignment));
	sb.addh("����-������");
	for(auto e = Fortitude; e <= Will; e = (ability_s)(e + 1))
		sb.addn("%1: %+2i", getstr(e), get(e));
	sb.addh("����������� ���������");
	sb.addn("��������� ���: %1i ������", getmaxcarry());
}

void creature::addinfocombat(stringbuilder& sb) const {
	auto active_slot = (slot_s)(QuickWeapon + active_weapon * 2);
	addweapons(sb, this, active_slot, "������ �����");
	for(auto i = QuickWeapon; i < QuickWeapon + 4 * 2; i = (slot_s)(i + 2))
		if(i != active_slot)
			addweapons(sb, this, i, "������� �����");
}