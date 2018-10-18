#include "main.h"

creature				players[6];
adat<creature, 256>		creature_data;

static char	good_save[] = {2,
2, 3, 3, 4, 4, 5, 5, 6, 6, 7,
7, 8, 8, 9, 9, 10, 10, 11, 11, 12};
static char	bad_save[] = {0,
0, 0, 1, 1, 1, 2, 2, 2, 3, 3,
3, 4, 4, 4, 5, 5, 5, 6, 6, 6};
static short carrying_capacity[][3] = {{1, 2, 3},
{3, 6, 10},// 1
{6, 13, 20},
{10, 20, 30},
{13, 26, 40},
{16, 33, 50},
{20, 40, 60},
{23, 46, 70},
{26, 53, 80},
{30, 60, 90},
{33, 66, 100},// 10
{38, 76, 115},
{43, 86, 130},
{50, 100, 150},
{58, 116, 175},
{66, 133, 200},
{76, 153, 230},
{86, 173, 260},
{100, 200, 300},
{116, 233, 350},
{133, 266, 400},// 20
{153, 306, 460},
{173, 346, 520},
{200, 400, 600},
{233, 466, 700},
{266, 533, 800},
{306, 616, 920},
{346, 693, 1040},
{400, 800, 1200},
{466, 933, 1400},
};

void* creature::operator new(unsigned size) {
	for(auto& e : creature_data) {
		if(!e)
			return &e;
	}
	return creature_data.add();
}

void creature::operator delete (void* data) {
	((creature*)data)->ability[0] = 0;
}

int	creature::get(ability_s id) const {
	return ability[id] / 2 - 5;
}

int	creature::getcharlevel() const {
	auto result = 0;
	for(int i = Barbarian; i <= Wizard; i++)
		result += classes[i];
	return result;
}

int	creature::getlevel() const {
	auto result = 0;
	for(auto value : classes)
		result += value;
	return result;
}

int	creature::getcasterlevel() const {
	auto result = 0;
	for(auto e = FirstClass; e <= LastClass; e = (class_s)(e + 1)) {
		if(!class_data[e].spells)
			continue;
		if(classes[e] >= class_data[e].spells[0].minimal)
			result += classes[e] - class_data[e].spells[0].minimal + 1;
	}
	return result;
}

int creature::get(save_s id) const {
	int r, result = 0;
	for(auto i = FirstClass; i <= LastClass; i = (class_s)(i + 1)) {
		if(!classes[i])
			continue;
		if(isgood(i, id))
			r = maptbl(good_save, classes[i]);
		else
			r = maptbl(bad_save, classes[i]);
		if(result < r)
			result = r;
	}
	result += get(getability(id));
	return result;
}

int	creature::get(spell_s id) const {
	for(auto& e : powers) {
		if(e.id == id)
			return e.count;
	}
	return 0;
}

int	creature::getmaxcarry() const {
	auto value = getr(Strenght);
	return maptbl(carrying_capacity, value)[2];
}

//void creature::set(coloration& e) const {
//	e.skin = colors.skin;
//	e.hair = colors.hair;
//	e.minor = colors.minor;
//	e.major = colors.major;
//	e.armor = 28;
//	e.metal = 30;
//	e.leather = 23;
//}

int creature::getbab() const {
	return classes[Barbarian] + classes[Fighter] + classes[Paladin] + classes[Ranger]
		+ ((classes[Bard] + classes[Cleric] + classes[Druid] + classes[Monk] + classes[Rogue]) * 3) / 4
		+ (classes[Sorcerer] + classes[Wizard]) / 2;
}

void creature::apply(race_s id, bool add_ability) {
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		ability[i] += race_data[id].abilities[i];
}

void creature::apply(class_s id) {
	for(auto e : class_data[id].weapon_proficiency)
		set(e);
	for(auto e : class_data[id].armor_proficiency)
		set(e);
	if(getcharlevel() == 1)
		hits_rolled = class_data[id].hd;
	else
		hits_rolled += xrand(1, class_data[id].hd);
}

int creature::get(skill_s id) const {
	auto r = skills[id];
	r += get(skill_data[id].ability);
	r += race_data[race].skills[id];
	for(auto e : skill_data[id].synergy) {
		if(e && skills[e] >= 5)
			r += 2;
	}
	return r;
}

int creature::getac(bool flatfooted) {
	int r = 10;
	if(!flatfooted) {
		r += get(Dexterity);
		r += get(Dodge);
	}
	r += wears[Body].getac();
	r += wears[Head].getac();;
	r += getweapon().getac();
	r += getoffhand().getac();
	return r;
}

class_s	creature::getclass() const {
	auto result = Commoner;
	auto result_level = 0;
	for(auto i = FirstClass; i <= LastClass; i = (class_s)(i + 1)) {
		if(classes[i] <= result_level)
			continue;
		result = i;
		result_level = classes[i];
	}
	return result;
}

bool creature::isallow(variant id) const {
	switch(id.type) {
	case Alignment: return isallow(id.alignment);
	case Feat: return isallow(id.feat);
	default: return true;
	}
}

void creature::set(variant value) {
	switch(value.type) {
	case Alignment: set(value.alignment); break;
	case Class: classes[value.number] = 1; break;
	case Gender: set(value.gender); break;
	case Race: set(value.race); break;
	case Spell: setknown(value.spell); break;
	}
}

variant_s creature::getstep() const {
	if(!gender)
		return Gender;
	else if(!race)
		return Race;
	else if(!getclass())
		return Class;
	else if(!alignment)
		return Alignment;
	else if(!ability[0])
		return Ability;
	else if(!getskillpoints())
		return Skill;
	else if(!iscolors())
		return Apearance;
	else if(!name)
		return Name;
	return NoVariant;
}

void creature::clear() {
	clear(Race);
	clear(Gender);
	clear(Class);
	clear(Alignment);
	clear(Ability);
	clear(Skill);
	clear(Apearance);
	clear(Name);
}

void creature::clear(variant_s value) {
	switch(value) {
	case Ability: memset(ability, 0, sizeof(ability)); break;
	case Alignment: alignment = NoAlignment; break;
	case Class: memset(classes, 0, sizeof(classes)); break;
	case Gender: gender = NoGender; portrait = 0; break;
	case Race: race = NoRace; break;
	case Skill:
		memset(skills, 0, sizeof(skills));
		memset(feats, 0, sizeof(feats));
		break;
	case Apearance: clearcolors(); break;
	case Name:
		name = 0;
		break;
	}
}

void creature::add(stringbuilder& sb, variant value) const {
	if(!value.number)
		return;
	if(sb)
		sb.add("\n");
	if(value.type != Alignment) {
		sb.add(getstr(value.type));
		sb.add(": ");
	}
	sb.add(getstr(value));
}

void creature::add(stringbuilder& sb, const aref<variant>& elements, const char* title) const {
	if(!elements)
		return;
	sb.addh(title);
	for(auto e : elements) {
		sb.addn(getstr(e));
		switch(e.type) {
		case Skill:
			sb.add(" %+1i (%2i)", get(e.skill), skills[e.skill]);
			break;
		case Ability:
			sb.add(": %1i", ability[e.ability]);
			break;
		}
	}
}

void creature::add(stringbuilder& sb, variant v1, variant v2, const char* title) const {
	variant elements[128];
	add(sb, selecth(elements, v1, v2, true), title);
}

void creature::addinfo(stringbuilder& sb) const {
	sb.addh("Уровни - уровень персонажа %1i", getcharlevel());
	for(auto e = FirstClass; e <= LastClass; e = (class_s)(e + 1)) {
		if(classes[e])
			sb.addn("%1: %2i", getstr(e), classes[e]);
	}
	sb.addn("");
	sb.addn("Любимый класс: Любой");
	sb.addn("");
	sb.addh("Опыт");
	sb.addn("Текущий: %1i", experience);
	sb.addn("Следующий уровень: %1i", 2000);
	sb.addn("");
	sb.addh("Раса");
	sb.addn(getstr(race));
	sb.addn("");
	sb.addh("Мировозрение");
	sb.addn(getstr(alignment));
	sb.addn("");
	sb.addh("Спас-броски");
	for(auto e = Fortitude; e <= Will; e = (save_s)(e + 1))
		sb.addn("%1: %+2i", getstr(e), get(e));
	sb.addn("");
	sb.addh("Способности атрибутов");
	sb.addn("Доступный вес: %1i фунтов", getmaxcarry());
}

void creature::getdescription(stringbuilder& sb) const {
	add(sb, gender);
	add(sb, race);
	add(sb, getclass());
	add(sb, alignment);
}

int	creature::getskillpoints() const {
	auto result = 0;
	for(auto e : skills)
		result += e;
	return result;
}

int	creature::getfeats() const {
	auto result = 0;
	for(auto e = FirstFeat; e <= LastFeat; e = (feat_s)(e + 1))
		if(is(e))
			result++;
	return result;
}

bool creature::isclass(skill_s id) const {
	for(auto i = Commoner; i <= Wizard; i = (class_s)(i + 1)) {
		if(!classes[i])
			continue;
		for(auto s : class_data[i].class_skills) {
			if(s == id)
				return true;
		}
	}
	return false;
}

int	creature::gethitsmax() const {
	return hits_rolled + get(Constitution)*getcharlevel();
}

int	creature::getpoints(class_s id) const {
	auto result = class_data[id].skill_points;
	result += race_data[race].quick_learn;
	result += get(Intellegence);
	if(result <= 0)
		result = 0;
	return result;
}

static char moveto_area[8];
static char moveto_entrace[32];

static void moveto_command() {
	map::load(moveto_area);
	auto pe = map::find(moveto_entrace);
	if(pe)
		creature::setcamera(pe->position);
	if(!settings.formation)
		settings.formation = Formation3by2;
	auto index = 0;
	auto start = actor::getbackward(pe->position, 5, pe->orientation);
	auto formation = settings.formation;
	for(auto& e : players) {
		if(!e)
			continue;
		e.setposition(actor::getposition(start, pe->position, formation, index++));
		e.actor::set(AnimateStand);
	}
	//draw::mslog("Область [%1]", moveto_area);
	creature::adventure();
}

void creature::moveto(const char* location, const char* entrance) {
	zcpy(moveto_area, location);
	if(entrance)
		zcpy(moveto_entrace, entrance);
	else
		moveto_entrace[0] = 0;
	draw::setnext(moveto_command);
}

void creature::updategame() {
	for(auto& e : floattext_data) {
		if(e)
			e.update();
	}
	for(auto& e : creature_data) {
		if(e)
			e.update();
	}
}

void creature::update_levels() {
	hits = gethitsmax();
}

int creature::getprepared(spell_s id, variant type) const {
	for(const auto& e : powers) {
		if(e.id == id && e.type == type)
			return e.count_maximum;
	}
	return 0;
}

int creature::getprepared(variant type) const {
	auto result = 0;
	for(const auto& e : powers) {
		if(e && e.type == type)
			result += e.count_maximum;
	}
	return result;
}

creature::preparation* creature::add(spell_s id, variant type) {
	for(auto& e : powers) {
		if(e.id == id && e.type == type)
			return &e;
	}
	preparation* p = 0;
	for(auto& e : powers) {
		if(!e) {
			p = &e;
			break;
		}
	}
	if(!p)
		p = powers.add();
	if(p) {
		p->type = type;
		p->id = id;
		p->count = 0;
		p->count_maximum = 0;
	}
	return p;
}

void creature::setprepared(spell_s id, variant type, int count) {
	preparation* p = add(id, type);
	if(p)
		p->count_maximum = count;
}

int	creature::getspellslots(variant value, int spell_level) const {
	auto result = 0;
	switch(value.type) {
	case Class:
		if(class_data[value.clas].spells) {
			auto si = spell_level - 1;
			const auto& c = class_data[value.clas];
			const auto& e = c.spells[si];
			auto m = classes[value.clas] - e.minimal;
			if(m < 0)
				return 0;
			if(m >= (int)e.progress.count)
				m = e.progress.count - 1;
			result += e.progress.data[m];
			auto a = get(c.spell_ability) - si;
			if(a > 0)
				result += 1 + a / 4;
		}
		break;
	}
	return result;
}

bool creature::equip(const item it) {
	if(!isallow(it))
		return false;
	for(auto e = Head; e <= QuickOffhand; e = (slot_s)(e + 1)) {
		if(wears[e])
			continue;
		if(!it.is(e))
			continue;
		wears[e] = it;
		return true;
	}
	return false;
}

void creature::add(item it) {
	if(equip(it))
		return;
	for(auto e = Backpack; e <= LastBackpack; e = (slot_s)(e + 1)) {
		if(!wears[e]) {
			wears[e] = it;
			break;
		}
	}
}

const item creature::getwear(slot_s id) const {
	switch(id) {
	case QuickWeapon: return wears[QuickWeapon + active_weapon * 2];
	case QuickOffhand: return wears[QuickOffhand + active_weapon * 2];
	default: return wears[id];
	}
}

aref<variant> creature::selecth(const aref<variant>& source, const variant v1, const variant v2, bool sort_by_name) const {
	auto pb = source.data;
	auto pe = pb + source.count;
	for(auto e = v1; e.number <= v2.number; e.number++) {
		switch(e.type) {
		case Feat:
			if(!is(e.feat))
				continue;
			break;
		case Skill:
			if(skills[e.skill]==0 && get(e.skill)<=0)
				continue;
			break;
		}
		if(pb < pe)
			*pb++ = e;
	}
	auto result = source;
	result.count = pb - source.data;
	if(sort_by_name)
		qsort(result.data, result.count, sizeof(result.data[0]), compare_variant);
	return result;
}

aref<variant> creature::select(const aref<variant>& source, const variant v1, const variant v2, bool sort_by_name) const {
	auto pb = source.data;
	auto pe = pb + source.count;
	if(v1.type == Feat) {
		auto character_level = getcharlevel();
		auto base_attack = getbab();
		for(auto e = v1; e.number <= v2.number; e.number++) {
			if(is(e.feat))
				continue;
			if(!isallow(e.feat, ability, character_level, base_attack))
				continue;
			if(pb < pe)
				*pb++ = e;
		}
	} else {
		for(auto e = v1; e.number <= v2.number; e.number++) {
			if(pb < pe)
				*pb++ = e;
		}
	}
	auto result = source;
	result.count = pb - source.data;
	if(sort_by_name)
		qsort(result.data, result.count, sizeof(result.data[0]), compare_variant);
	return result;
}

bool creature::choose_skills(const char* title, const aref<variant>& elements, bool add_ability, bool interactive) {
	auto type = getclass();
	auto race = getrace();
	apply(race, add_ability);
	apply(type);
	apply(race, 1, interactive);
	apply(type, 1, interactive);
	auto feat_points = 1;
	feat_points += race_data[race].quick_learn;
	if(!choose_feats(title, "Выбор особенностей",
		select(elements, FirstFeat, LastFeat, !interactive), feats, feat_points, interactive))
		return false;
	if(!choose_skills(title, "Выбор навыков",
		select(elements, FirstSkill, LastSkill, !interactive), skills, getpoints(getclass()) * 4, 4, interactive))
		return false;
	return true;
}

void creature::choose_skills(const char* title, const aref<variant>& elements) {
	creature player = *this;
	if(!player.choose_skills(title, elements, false, true))
		return;
	*this = player;
}

void creature::create(class_s type, race_s race, gender_s gender) {
	variant elements[128];
	this->gender = gender;
	this->race = race;
	this->classes[type] = 1;
	for(auto a = Strenght; a <= Charisma; a = (ability_s)(a + 1))
		ability[a] = 10;
	choose_skills("Случайная генерация", elements, true, false);
	update_levels();
	portrait = random_portrait();
	update_portrait();
	random_name();
}

void creature::moveto(aref<creature> players, point destination, formation_s formation) {
	if(!players)
		return;
	auto start = players[0].getposition();
	auto index = 0;
	for(auto& e : players)
		e.move(getposition(start, destination, formation, index++));
}