#include "main.h"

BSDATAC(creature, 256)
static ability_s		calculating_ability[] = {Attack, Fortitude, Reflexes, Will};

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

void targetreaction::clear() {
	target.clear();
	method = 0;
}

int	creature::get(ability_s id) const {
	switch(id) {
	case Strenght:
	case Constitution:
	case Dexterity:
	case Intellegence:
	case Wisdow:
	case Charisma:
		return ability[id] / 2 - 5;
	case Will:
	case Fortitude:
	case Reflexes:
		return ability[id] + get(bsdata<abilityi>::elements[id].base);
	default:
		return ability[id];
	}
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
		if(!bsdata<classi>::elements[e].spells)
			continue;
		if(classes[e] >= bsdata<classi>::elements[e].spells[0].minimal)
			result += classes[e] - bsdata<classi>::elements[e].spells[0].minimal + 1;
	}
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

void creature::set(feat_s v) {
	feats[v / 32] |= (1 << (v % 32));
}

void creature::apply(race_s id) {
	for(auto i = Strenght; i <= LastAbility; i = (ability_s)(i + 1))
		ability[i] += bsdata<racei>::elements[id].abilities[i];
	for(auto e : bsdata<racei>::elements[id].feats)
		set(e);
}

void creature::apply(class_s id) {
	for(auto e : bsdata<classi>::elements[id].weapon_proficiency)
		set(e);
	for(auto e : bsdata<classi>::elements[id].armor_proficiency)
		set(e);
	if(getcharlevel() == 1)
		hits_rolled = bsdata<classi>::elements[id].hd;
	else
		hits_rolled += xrand(1, bsdata<classi>::elements[id].hd);
}

int creature::get(skill_s id) const {
	auto r = skills[id];
	r += get(bsdata<skilli>::elements[id].ability);
	r += bsdata<racei>::elements[getrace()].skills[id];
	for(auto e : bsdata<skilli>::elements[id].synergy) {
		if(e && skills[e] >= 5)
			r += 2;
	}
	return r;
}

int creature::getac(bool flatfooted) const {
	auto r = get(ArmorClass);
	if(!flatfooted) {
		r += get(Dexterity);
		r += get(Dodge);
	}
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
	case Alignment: return isallow((alignment_s)id.value);
	case Feat: return isallow((feat_s)id.value);
	case Item: return isallow((item_s)id.value);
	default: return true;
	}
}

void creature::set(variant id) {
	switch(id.type) {
	case Alignment: set((alignment_s)id.value); break;
	case Class: classes[id.value] = 1; break;
	case Gender: setgender((gender_s)id.value); break;
	case Race: setkind((race_s)id.value); break;
	case Spell: setknown((spell_s)id.value); break;
	}
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
	case Ability:
		memset(ability, 0, sizeof(ability));
		break;
	case Alignment:
		alignment = LawfulGood;
		break;
	case Class:
		memset(classes, 0, sizeof(classes));
		break;
	case Gender:
		setkind(Human);
		setgender(NoGender);
		reaction = Undifferent;
		portrait = 0;
		initiative = 0;
		break;
	case Race:
		setkind(Human);
		break;
	case Skill:
		memset(skills, 0, sizeof(skills));
		memset(feats, 0, sizeof(feats));
		break;
	case Apearance:
		clearcolors();
		break;
	case Name:
		name = 0;
		break;
	}
}

void creature::add(stringbuilder& sb, variant v1, variant v2, const char* title, bool sort_by_name) const {
	varianta elements;
	elements.select(v1, v2);
	elements.match(*this, true);
	if(sort_by_name)
		elements.sort();
	add(sb, elements, title);
}

int	creature::getskillpoints() const {
	auto result = 0;
	for(auto e : skills)
		result += e;
	return result;
}

short unsigned creature::getid() const {
	return this - bsdata<creature>::elements;
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
		for(auto s : bsdata<classi>::elements[i].class_skills) {
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
	auto result = bsdata<classi>::elements[id].skill_points;
	result += bsdata<racei>::elements[getrace()].quick_learn;
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
	adat<creature, 6> players;
	for(auto p : game.party)
		players.add(*p);
	game.party.clear();
	map::load(move_to_location);
	for(auto& e : players) {
		auto p = bsdata<creature>::add();
		*p = e; game.party.add(p);
	}
	auto pe = map::find(move_to_entrance);
	if(pe)
		creature::setcamera(pe->position);
	if(!game.formation)
		game.formation = Formation3by2;
	auto index = 0;
	auto start = actor::getbackward(pe->position, 5, pe->orientation);
	auto formation = game.formation;
	for(auto p : game.party) {
		if(!(*p))
			continue;
		p->stop();
		p->setorientation(pe->orientation);
		p->setposition(map::getfree(p->getposition(start, pe->position, formation, index++), p->getsize()));
	}
	select_all();
	msdbg("Зашли в область [%1]", move_to_location);
	draw::setpagedef(adventure);
	draw::setpage(adventure);
}

void creature::updategame() {
	for(auto& e : bsdata<floattext>()) {
		if(e)
			e.update();
	}
	for(auto& e : bsdata<creature>()) {
		if(e)
			e.update();
	}
}

void creature::finish() {
	dresson();
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
		if(bsdata<classi>::elements[value.value].spells) {
			auto si = spell_level - 1;
			const auto& c = bsdata<classi>::elements[value.value];
			const auto& e = c.spells[si];
			auto m = classes[value.value] - e.minimal;
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
		wears[e].equip(it);
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

const item* creature::getwear(slot_s id) const {
	switch(id) {
	case QuickWeapon: return wears + QuickWeapon + active_weapon * 2;
	case QuickOffhand: return wears + QuickOffhand + active_weapon * 2;
	default: return wears + id;
	}
}

bool creature::choose_skills(const char* title, varianta& elements, bool interactive) {
	auto type = getclass();
	auto race = getrace();
	ability[ArmorClass] += 10;
	apply(race);
	apply(type);
	apply(race, 1, interactive);
	apply(type, 1, interactive);
	auto feat_points = 1 + bsdata<racei>::elements[race].quick_learn;
	elements.select(FirstFeat, ProficiencyWaraxe, *this, false);
	elements.match(*this, false);
	elements.sort();
	if(!choose_feats(title, "Выбор особенностей", elements, feats, feat_points, interactive))
		return false;
	elements.select(FirstSkill, LastSkill, *this);
	elements.sort();
	if(!choose_skills(title, "Выбор навыков", elements, getpoints(getclass()) * 4, 4, interactive))
		return false;
	return true;
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
	clear();
	setgender(gender);
	setkind(race);
	this->reaction = reaction;
	this->classes[type] = 1;
	for(auto a = Strenght; a <= Charisma; a = (ability_s)(a + 1))
		ability[a] = roll_4d6();
	varianta elements;
	choose_skills("Случайная генерация", elements, false);
	portrait = random_portrait();
	update_portrait();
	random_name();
	finish();
}

creature* creature::create(monster_s type, reaction_s reaction, point postition) {
	auto p = bsdata<creature>::add();
	p->create(type, reaction);
	p->stop();
	p->setposition(map::getfree(postition, p->getsize()));
	return p;
}

void creature::create(monster_s type, reaction_s reaction, point position, unsigned char orientation, int count) {
	map::blockimpassable();
	auto formation = Formation3by2;
	auto index = 0;
	point start = getbackward(position, 5, orientation);
	for(int i = 0; i < count; i++) {
		auto p = create(type, reaction, getposition(start, position, formation, index++));
		p->setorientation(orientation);
		p->blockimpassable();
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
	for(auto& e : bsdata<creature>()) {
		if(!e)
			continue;
		auto i = map::getindex(e.getposition(), e.getsize());
		if(index == i)
			return &e;
	}
	return 0;
}

void creature::blockallcreatures() {
	for(auto& e : bsdata<creature>()) {
		if(!e || !e.isready())
			continue;
		e.blockimpassable();
	}
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
	for(auto p : game.party) {
		if(!(*p))
			continue;
		auto pt = p->getposition();
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

void creature::attack(const variant& e) {
	if(e.type == Creature)
		attack(*e.getcreature());
}

void creature::attack(creature& enemy) {
	auto player_index = getindex();
	auto enemy_index = enemy.getindex();
	attacki ai = {}; get(ai, QuickWeapon, enemy);
	res::tokens thrown_res = res::NONE;
	auto thrown_speed = 300;
	if(ai.range && ai.weapon)
		thrown_res = ai.weapon->getanthrown();
	render_attack(rand() % 3, enemy.getposition());
	wait(70);
	if(thrown_res) {
		auto hit_position = enemy.getposition(65);
		auto pa = new moveable(getposition(85), hit_position, thrown_res, thrown_speed);
		pa->wait();
	}
	if(!roll(ai))
		return;
	auto damage = ai.damage.roll();
	if(ai.iscritical(ai.critical))
		damage *= 2 + ai.multiplier;
	enemy.damage(damage);
	enemy.render_hit(enemy.gethits() <= 0);
	enemy.wait();
}

void creature::get(attacki& result, item* weapon, bool isoffhand) const {
	memset(&result, 0, sizeof(result));
	result.weapon = weapon;
	if(!result.weapon || !(*result.weapon))
		result.weapon = 0;
	if(result.weapon) {
		result.weapon->apply(result);
		if(result.weapon->isranged()) {
			if(!isoffhand)
				result.bonus += get(Dexterity);
			if(is(FarShoot))
				result.range = result.range * 15 / 10;
		} else {
			if(isoffhand)
				result.bonus += get(Strenght);
			else {
				result.bonus += get(Strenght);
				result.damage.b += get(Strenght);
			}
		}
	} else {
		if(!isoffhand) {
			result.damage.c = 1;
			result.damage.d = 3;
			result.damage.b += get(Strenght);
		}
	}
	if(is(ImprovedCritical))
		result.critical++;
}

void creature::get(attacki& result, slot_s slot) const {
	get(result, const_cast<item*>(getwear(slot)), slot == QuickOffhand);
}

bool creature::roll(rolli& e) const {
	e.rolled = rand() % 20 + 1;
	e.result = e.rolled + e.bonus;
	return e.result >= e.dc;
}

void creature::get(attacki& result, slot_s slot, const creature& enemy) const {
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
	msdbg("Start combat");
	adat<creature*, 264> elements;
	for(auto& e : bsdata<creature>()) {
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
	msdbg("End combat");
}

bool creature::isplayer() const {
	return game.party.is(const_cast<creature*>(this));
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
	for(auto& e : bsdata<creature>()) {
		if(e)
			map::setcost(e.getindex(), Blocked);
	}
}

void creature::interacting(const targetreaction& e) {
	(this->*e.method)(e.target);
}

void creature::toggle(const variant& e) {
	switch(e.type) {
	case Door:
		if(e.getdoor()->isopen())
			close(e);
		else
			open(e);
		break;
	}
}

void creature::open(const variant& e) {
	if(e.type == Door)
		e.getdoor()->setopened(true);
}

void creature::close(const variant& e) {
	if(e.type == Door)
		e.getdoor()->setopened(false);
}

int creature::getdistance(map::indext goal) const {
	auto start = map::getindex(getposition(), getsize());
	return map::getdistance(start, goal);
}

int creature::getdistance(point position) const {
	return getdistance(map::getindex(position, 1));
}

void creature::interact(const targetreaction& e, short unsigned maximum_range, bool synchronized) {
	point position;
	short unsigned reach = 0;
	switch(e.target.type) {
	case Door:
		if(getmovedistance(e.target.getdoor()->points[0], 0))
			position = e.target.getdoor()->points[0];
		else
			position = e.target.getdoor()->points[1];
		break;
	case Container:
		position = e.target.getcontainer()->launch;
		break;
	case Creature:
		position = e.target.getcreature()->getposition();
		if(e.method == &creature::attack) {
			attacki ai; get(ai, QuickWeapon, *e.target.getcreature());
			reach = map::getrange(ai.range);
		}
		break;
	case ItemGround:
		position = e.target.getitemground()->getposition();
		break;
	case Position:
		position = e.target.getposition();
		break;
	case Region:
		switch(e.target.getregion()->type) {
		case RegionTravel:
			position = e.target.getregion()->getposition();
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
			(this->*e.method)(e.target);
	} else if(move(position, reach, maximum_range)) {
		if(synchronized) {
			wait();
			if(reach && getdistance(position) > reach)
				return;
			if(e.method)
				(this->*e.method)(e.target);
		}
		else
			actor::set(e);
	}
}

int	creature::getmoney() {
	return game.money;
}

void creature::setmoney(int value) {
	game.money = value;
}

int creature::getmovement() const {
	auto result = 30;
	if(is(FastMovement))
		result += 10;
	return result;
}

int	creature::getbodyheight() const {
	switch(getrace()) {
	case Elf:
	case HalfElf: return 40;
	case Gnome: case Dwarf: return 30;
	default: return 50;
	}
}

bool creature::isallow(feat_s id, bool test_feats) const {
	auto& ei = bsdata<feati>::elements[id];
	for(auto e = Strenght; e <= Charisma; e = (ability_s)(e + 1)) {
		auto value = ei.ability[e];
		if(value && getr(e) < value)
			return false;
	}
	if(test_feats) {
		for(auto e : ei.prerequisits) {
			if(!is(e))
				return false;
		}
	}
	if(ei.base_attack && getraw(Attack) < ei.base_attack)
		return false;
	if(ei.character_level && getcharlevel() < ei.character_level)
		return false;
	return true;
}

bool creature::isallow(item_s i) const {
	const auto& ei = bsdata<itemi>::elements[i];
	if(!ei.feat[0])
		return true;
	for(auto e : ei.feat) {
		if(!e)
			break;
		if(is(e))
			return true;
	}
	return false;
}

bool creature::isallow(alignment_s i) const {
	for(auto e = FirstClass; e <= LastClass; e = (class_s)(e + 1)) {
		if(!classes[e])
			continue;
		for(auto v : bsdata<classi>::elements[e].alignment_restrict) {
			if(v == i)
				return false;
		}
	}
	return true;
}

bool creature::have(variant id) const {
	switch(id.type) {
	case Alignment: return alignment == id.value;
	case Class: return classes[id.value] != 0;
	case Gender: return getgender() == id.value;
	case Feat: return is((feat_s)id.value);
	case Race: return getrace() == id.value;
	case Reaction: return reaction == id.value;
	case Skill: return skills[id.value]!=0;
	default: return false;
	}
}

void creature::addvar(variant id, char bonus) {
	switch(id.type) {
	case Ability: ability[id.value] += bonus; break;
	default: break;
	}
}

int creature::getraw(ability_s id) const {
	int r, n;
	switch(id) {
	case Attack:
		return classes[Barbarian] + classes[Fighter] + classes[Paladin] + classes[Ranger]
			+ ((classes[Bard] + classes[Cleric] + classes[Druid] + classes[Monk] + classes[Rogue]) * 3) / 4
			+ (classes[Sorcerer] + classes[Wizard]) / 2;
	case Reflexes:
	case Fortitude:
	case Will:
		r = ability[id];
		for(auto i = FirstClass; i <= LastClass; i = (class_s)(i + 1)) {
			if(!classes[i])
				continue;
			if(isgood(i, id))
				n = maptbl(good_save, classes[i]);
			else
				n = maptbl(bad_save, classes[i]);
			if(r < n)
				r = n;
		}
		r += get(getability(id));
		switch(id) {
		case Fortitude:
			if(is(GreateFortitude))
				r += 2;
			break;
		case Will:
			if(is(IronWill))
				r += 2;
			break;
		case Reflexes:
			if(is(LightingReflexes))
				r += 2;
			break;
		}
		return r;
	default:
		return 0;
	}
}

void creature::dress(int m) {
	// Abilities
	for(auto a : calculating_ability)
		ability[a] += getraw(a)*m;
	// Equipment
	for(auto s = Head; s <= LastQuickItem; s = (slot_s)(s+1)) {
		if(!wears[s])
			continue;
		ability[ArmorClass] += wears[s].getac() * m;
		ability[DeflectCritical] += wears[s].geti().ai.deflect_critical * m;
	}
}

variant creature::choose_target() const {
	auto max_cost = rangei::getsquare(get(Movement));
	map::blockimpassable(0);
	blockallcreatures();
	map::createwave(map::getindex(getposition(), getsize()), 1);
	return choose_position(0, max_cost);
}