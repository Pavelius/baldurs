#include "main.h"

creature				players[6];
adat<creature, 256>		creature_data;
static int				party_money;

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

void targetreaction::clear() {
	target::clear();
	method = 0;
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

int creature::getac(bool flatfooted) const {
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
	return Finish;
}

void creature::clear() {
	actor::clear();
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
	case Gender:
		kind = Character;
		gender = NoGender;
		reaction = Undifferent;
		portrait = 0;
		initiative = 0;
		break;
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
		case Skill: sb.add(" %+1i (%2i)", get(e.skill), skills[e.skill]); break;
		case Ability: sb.add(": %1i", ability[e.ability]); break;
		}
	}
}

void creature::add(stringbuilder& sb, variant v1, variant v2, const char* title, bool sort_by_name) const {
	variant elements[128];
	add(sb, selecth(elements, v1, v2, sort_by_name), title);
}

void creature::addinfo(stringbuilder& sb) const {
	sb.addh("Уровни - уровень персонажа %1i", getcharlevel());
	for(auto e = FirstClass; e <= LastClass; e = (class_s)(e + 1)) {
		if(classes[e])
			sb.addn("%1: %2i", getstr(e), classes[e]);
	}
	sb.addn("");
	sb.addn("Любимый класс: Любой");
	sb.addh("Опыт");
	sb.addn("Текущий: %1i", experience);
	sb.addn("Следующий уровень: %1i", 2000);
	sb.addh("Раса");
	sb.addn(getstr(race));
	sb.addh("Мировозрение");
	sb.addn(getstr(alignment));
	sb.addh("Спас-броски");
	for(auto e = Fortitude; e <= Will; e = (save_s)(e + 1))
		sb.addn("%1: %+2i", getstr(e), get(e));
	sb.addh("Способности атрибутов");
	sb.addn("Доступный вес: %1i фунтов", getmaxcarry());
}

void creature::getdescription(stringbuilder& sb) const {
	add(sb, gender);
	add(sb, race);
	add(sb, getclass());
	add(sb, alignment);
	add(sb, Strenght, Charisma, "Атрибуты", false);
	add(sb, FirstSkill, LastSkill, "Навыки");
	add(sb, FirstFeat, WhirlwindAttack, "Особенности");
	add(sb, ProficiencyAxe, ProficiencyWaraxe, "Доступность оружия");
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

void creature::moveto(const char* location, const char* entrance) {
	char move_to_location[32]; zcpy(move_to_location, location, 8);
	char move_to_entrance[32]; move_to_entrance[0] = 0;
	if(entrance)
		zcpy(move_to_entrance, entrance);
	map::load(move_to_location);
	auto pe = map::find(move_to_entrance);
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
		e.stop();
		e.setorientation(pe->orientation);
		e.setposition(map::getfree(e.getposition(start, pe->position, formation, index++), e.getsize()));
	}
	select_all();
	draw::setpagedef(adventure);
	draw::setpage(adventure);
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
	switch(it.gettype()) {
	case CooperPieces:
	case SilverPieces:
	case GoldPieces:
	case PlatinumPieces:
		setmoney(getmoney() + it.getcost() * it.getcount());
		return;
	}
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
	auto result = source; result.count = 0;
	switch(v1.type) {
	case Skill:
		if(!getskillpoints())
			return result;
		break;
	}
	for(auto e = v1; e.number <= v2.number; e.number++) {
		switch(e.type) {
		case Feat:
			if(!is(e.feat))
				continue;
			break;
		case Skill:
			if(!skills[e.skill])
				continue;
			break;
		case Ability:
			if(ability[e.ability] == 0)
				continue;
			break;
		}
		if(pb < pe)
			*pb++ = e;
	}
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
		select(elements, FirstFeat, ProficiencyWaraxe, !interactive), feats, feat_points, interactive))
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

static int compare_rolls(const void* p1, const void* p2) {
	return *((char*)p2) - *((char*)p1);
}

static int roll_4d6() {
	char temp[4];
	for(auto& e : temp)
		e = 1 + rand() % 6;
	qsort(temp, sizeof(temp) / sizeof(temp[0]), sizeof(temp[0]), compare_rolls);
	return temp[0] + temp[1] + temp[2];
}

void creature::create(class_s type, race_s race, gender_s gender, reaction_s reaction) {
	variant elements[128];
	clear();
	this->kind = Character;
	this->gender = gender;
	this->race = race;
	this->reaction = reaction;
	this->classes[type] = 1;
	for(auto a = Strenght; a <= Charisma; a = (ability_s)(a + 1))
		ability[a] = roll_4d6();
	choose_skills("Случайная генерация", elements, true, false);
	update_levels();
	portrait = random_portrait();
	update_portrait();
	random_name();
}

creature* creature::create(monster_s type, reaction_s reaction, point postition) {
	auto p = creature_data.add();
	p->create(type, reaction);
	p->stop();
	p->setposition(map::getfree(postition, p->getsize()));
	return p;
}

void creature::create(monster_s type, reaction_s reaction, point position, unsigned char orientation, int count) {
	auto formation = Formation3by2;
	auto index = 0;
	point start = getbackward(position, 5, orientation);
	for(int i = 0; i < count; i++) {
		auto p = create(type, reaction, getposition(start, position, formation, index++));
		p->setorientation(orientation);
	}
}

void creature::moveto(aref<creature> players, point destination, formation_s formation) {
	if(!players)
		return;
	auto start = players[0].getposition();
	auto index = 0;
	for(auto& e : players)
		e.move(map::getfree(getposition(start, destination, formation, index++), e.getsize()));
}

creature* creature::getcreature(point position) {
	return getcreature(map::getindex(position));
}

creature* creature::getcreature(short unsigned index) {
	auto result = getcreature(players, index);
	if(!result)
		result = getcreature(creature_data, index);
	return result;
}

creature* creature::getcreature(aref<creature> collection, short unsigned index) {
	for(auto& e : collection) {
		if(!e)
			continue;
		auto i = map::getindex(e.getposition(), e.getsize());
		if(index == i)
			return &e;
	}
	return 0;
}

bool creature::isblock(point value) const {
	if(!value)
		return true;
	int s = getsize();
	int i = map::getindex(value, s);
	return getcreature(i) != 0;
}

int creature::getpartymaxdistance(point position) {
	auto result = 0;
	for(auto& e : players) {
		if(!e)
			continue;
		auto pt = e.getposition();
		auto ds = distance(pt, position);
		if(ds > result)
			result = ds;
	}
	return result;
}

aref<creature*> creature::select(const aref<creature*>& destination, const aref<creature*>& source, const creature* player, bool(creature::*proc)(const creature& e) const, short unsigned range_maximum, short unsigned range_position) {
	auto pb = destination.data;
	auto pe = destination.data + destination.count;
	if(pb < pe) {
		for(auto p : source) {
			if(!(*p))
				continue;
			if(player && !(player->*proc)(*p))
				continue;
			if(range_maximum && map::getrange(map::getindex(p->getposition()), range_position) > range_maximum)
				continue;
			*pb++ = p;
			if(pb >= pe)
				break;
		}
	}
	auto result = destination;
	result.count = pb - result.data;
	return result;
}

short unsigned creature::getindex() const {
	return map::getindex(getposition(), getsize());
}

void creature::attack(const target& e) {
	if(e.type == Creature)
		attack(*e.creature);
}

void creature::attack(creature& enemy) {
	auto player_index = getindex();
	auto enemy_index = enemy.getindex();
	auto reach = map::getrange(getreach());
	player_index = getindex();
	if(reach < map::getrange(player_index, enemy_index))
		return;
	attack_info ai; get(ai, QuickWeapon, enemy);
	if(!roll(ai)) {
		render_attack(rand() % 3, enemy.getposition());
		wait();
		return;
	}
	render_attack(rand() % 3, enemy.getposition());
	wait(50);
	auto damage = ai.roll();
	enemy.damage(damage);
	enemy.render_hit(enemy.gethits() <= 0);
	wait();
	enemy.wait();
}

void creature::get(attack_info& result, slot_s slot) const {
	memset(&result, 0, sizeof(result));
	auto& weapon = wears[slot];
	if(weapon) {
		static_cast<dice&>(result) = weapon.getattack();
		result.weapon = (item*)&weapon;
		if(weapon.isranged()) {
			result.bonus += get(Dexterity);
		} else {
			result.b += get(Strenght);
			result.bonus += get(Strenght);
		}
	} else {
		result.c = 0;
		result.d = imax(1, 1 + get(Strenght));
	}
	if(is(ImprovedCritical))
		result.critical++;
}

bool creature::roll(roll_info& e) const {
	e.rolled = rand() % 20 + 1;
	e.result = e.rolled + e.bonus;
	return e.result >= e.dc;
}

void creature::get(attack_info& result, slot_s slot, const creature& enemy) const {
	get(result, slot);
	result.dc = enemy.getac(false);
}

void creature::damage(int count) {
	hits -= count;
	if(hits < 0) {
		// TODO: Должны выпасть предметы и начислиться опыт. Труп не убирать.
	}
}

creature* creature::getbest(const aref<creature*>& source, bool (creature::*proc)(const creature& opponent) const) const {
	creature* result_data[32];
	auto result = select(result_data, source, this, proc);
	if(result) {
		int result_distance;
		creature* result_creature = 0;
		auto start = getposition();
		for(auto p : result) {
			if(!result_creature) {
				result_creature = p;
				result_distance = distance(start, p->getposition());
			}
			else {
				auto this_distance = distance(start, p->getposition());
				if(this_distance < result_distance) {
					result_creature = p;
					result_distance = this_distance;
				}
			}
		}
		return result_creature;
	}
	return 0;
}

static int compare_initiative(const void* p1, const void* p2) {
	auto c1 = *((creature**)p1);
	auto c2 = *((creature**)p2);
	return c1->getinitiativeroll() - c2->getinitiativeroll();
}

short unsigned creature::getreach() const {
	if(getweapon().isreach())
		return 10;
	return 5;
}

void creature::makecombat() {
	msdbg("Началась битва");
	adat<creature*, 264> elements;
	for(auto& e : players) {
		if(!e)
			continue;
		elements.add(&e);
	}
	for(auto& e : creature_data) {
		if(!e)
			continue;
		elements.add(&e);
	}
	if(!elements)
		return;
	for(auto p : elements)
		p->initiative = 1 + (rand() % 20) + p->getinitiative();
	qsort(elements.data, elements.count, sizeof(elements.data[0]), compare_initiative);
	while(true) {
		auto continue_combat = false;
		for(auto p : elements) {
			if(!p->isready())
				continue;
			if(!p->is(ReadyToBattle)) {
				p->set(ReadyToBattle);
				p->stop();
				continue_combat = true;
				continue;
			}
			auto enemy = p->getbest(elements, &creature::isenemy);
			if(!enemy) {
				continue_combat = false;
				break;
			}
			continue_combat = true;
			slide(p->getposition());
			if(p->isplayer()) {
				p->choose_action();
			} else {
				if(enemy) {
					targetreaction tg(enemy);
					tg.method = &creature::attack;
					p->interact(tg, map::getrange(p->getmovement()) + 1, true);
				} else {
					// TODO: ошибка?
				}
			}
		}
		if(!continue_combat)
			break;
	}
	for(auto p : elements) {
		if(*p && p->gethits() > 0) {
			p->remove(ReadyToBattle);
			p->stop();
		}
	}
	msdbg("Битва закончилась");
}

bool creature::isplayer() const {
	return this >= players
		&& this <= players + sizeof(players) / sizeof(players[0]);
}

bool creature::isenemy(const creature& opponent) const {
	if(!opponent.isready())
		return false;
	return (reaction == Hostile && opponent.reaction == Helpful)
		|| (reaction == Helpful && opponent.reaction == Hostile);
}

int creature::getinitiative() const {
	auto result = get(Dexterity);
	return result;
}

void creature::blockimpassable() const {
	for(auto& e : players) {
		if(e)
			map::setcost(e.getindex(), Blocked);
	}
	for(auto& e : creature_data) {
		if(e)
			map::setcost(e.getindex(), Blocked);
	}
}

void creature::interacting(const targetreaction& e) {
	(this->*e.method)(e);
}

void creature::toggle(const target& e) {
	switch(e.type) {
	case Door:
		if(e.door->isopen())
			close(e);
		else
			open(e);
		break;
	}
}

void creature::open(const target& e) {
	if(e.type == Door)
		e.door->setopened(true);
}

void creature::close(const target& e) {
	if(e.type == Door)
		e.door->setopened(false);
}

void creature::interact(const targetreaction& e, short unsigned maximum_range, bool synchronized) {
	point position;
	short unsigned reach = 0;
	switch(e.type) {
	case Door:
		if(getmovedistance(e.door->points[0], map::getrange(getreach())))
			position = e.door->points[0];
		else
			position = e.door->points[1];
		break;
	case Container:
		position = e.container->launch;
		break;
	case Creature:
		position = e.creature->getposition();
		reach = e.reach;
		break;
	case ItemGround:
		position = e.itemground->getposition();
		break;
	case Position:
		position = e.position;
		break;
	case Region:
		switch(e.region->type) {
		case RegionTravel:
			position = e.region->getposition();
			break;
		default:
			return;
		}
		break;
	default:
		return;
	}
	stop();
	auto index = getindex();
	auto index_new = map::getindex(position);
	if(reach == 0xFFFF) {
		if(e.method)
			(this->*e.method)(e);
	} else if(move(position, maximum_range, reach)) {
		if(synchronized) {
			wait();
			if(e.method)
				(this->*e.method)(e);
		}
		else
			actor::set(e);
	}
}

int	creature::getmoney() {
	return party_money;
}

void creature::setmoney(int value) {
	party_money = value;
}

int creature::getmovement() const {
	auto result = 30;
	if(is(FastMovement))
		result += 10;
	return result;
}