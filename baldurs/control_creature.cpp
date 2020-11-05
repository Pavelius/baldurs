#include "main.h"

enum generation_event_s {
	NoGenerationEvent,
	PreviousPortrait, NextPortrait,
	Plus, Minus,
	CreateNew,
};

using namespace draw;

BSDATA(genstepi) = {{Gender, "Выбирайте пол", Male, Female},
{Race, "Выбирайте расу", Dwarf, Human},
{Class, "Выбирайте класс", FirstClass, LastClass},
{Alignment, "Выбирайте мировозрение", LawfulGood, ChaoticEvil},
{Ability, "Способности"},
{Skill, "Навыки"},
{Apearance, "Внешний вид"},
{Name, "Имя"},
};
BSDATAF(genstepi)

static variant			current_variant;
static char				description[1024 * 16];
static ctext		description_control;
static char				ability_cost[] = {0, 1, 2, 3, 4, 5, 6, 8, 10, 13, 16};

static void biography() {}

static void import_character() {}

static int getabilitycost(const char* ability, const char* modifier) {
	auto result = 0;
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		auto value = ability[i] - 8;
		if(modifier)
			value += modifier[i];
		if(value < 0)
			value = 0;
		result += maptbl(ability_cost, value);
	}
	return result;
}

static void select_variant() {
	current_variant = hot.param;
	stringbuilder sb(description);
	current_variant.addinfo(sb);
}

static void next() {
	breakmodal(current_variant);
}

static void button_plus() {
	hot.key = Plus;
}

static void button_minus() {
	hot.key = Minus;
}

static aref<variant> createlist(aref<variant> result, variant v1, variant v2) {
	auto pb = result.begin();
	auto pe = result.end();
	for(auto i = v1; i.value <= v2.value; i.value++) {
		if(pb < pe)
			*pb++ = i;
	}
	result.count = pb - result.data;
	qsort(result.data, result.count, sizeof(result.data[0]), compare_variant);
	return result;
}

static void command_buttons(variant_s step = NoVariant) {
	button(35, 550, biography, 0, (step==Finish) ? 0 : Disabled, res::GBTNSTD, "Биография");
	button(204, 550, import_character, 1, 0, res::GBTNSTD, "Импортировать");
	button(342, 550, buttoncancel, 2, 0, res::GBTNSTD, "Назад", KeyEscape);
	button(647, 550, buttonparam, CreateNew, (step == NoVariant || step != Gender) ? 0 : Disabled, res::GBTNSTD, "Начать заново");
}

variant creature::choose(const char* title, const char* step_title, varianta& elements) const {
	while(ismodal()) {
		background(res::GUICGB, 0);
		draw::image(23, 151, res::PORTL, getportrait());
		label(174, 22, 452, 29, title, 2);
		label(173, 65, 453, 20, step_title);
		int nid = 0;
		for(auto e : elements) {
			unsigned flags = (current_variant == e) ? Checked : 0;
			if(!isallow(e))
				flags |= Disabled;
			if(button(274, 113 + 35 * nid, nid, flags, res::GBTNLRG, getstr(e), Alpha + '1' + nid))
				execute(select_variant, e);
			nid++;
		}
		command_buttons();
		button(478, 550, next, 0, current_variant ? 0 : Disabled, res::GBTNSTD, "Далее", KeyEnter);
		view({570, 153, 754, 478}, {765, 151, 777, 481}, description, description_control);
		domodal();
	}
	current_variant = 0;
	return getresult();
}

static int choose_portrait(const char* title, const char* step_title, gender_s gender) {
	adat<short, 128> portrait_indicies;
	portrait_indicies.clear();
	auto col = portraiti::getelements();
	for(auto& e : col) {
		if(e.is(gender))
			portrait_indicies.add(col.indexof(&e));
	}
	if(!portrait_indicies)
		return -1;
	auto index = 0;
	while(ismodal()) {
		background(res::GUICGB, 0);
		draw::image(23, 151, res::PORTL, 0);
		label(174, 22, 452, 29, title, 2);
		label(173, 65, 453, 20, step_title);
		image(252, 61, gres(res::GUIA), 1, 0);
		image(23, 151, res::PORTL, 0);
		image(295, 115, gres(res::PORTL), portrait_indicies[index], 0);
		if(button(260, 265, 0, res::GBTNPOR, 0, 0, 1, -1, 0, KeyLeft, 0))
			execute(buttonparam, PreviousPortrait);
		if(button(512, 265, 0, res::GBTNPOR, 0, 2, 3, -1, 0, KeyRight, 0))
			execute(buttonparam, NextPortrait);
		command_buttons();
		button(478, 550, buttonok, 0, 0, res::GBTNSTD, "Далее", KeyEnter);
		view({570, 153, 754, 478}, {765, 151, 777, 481}, description, description_control);
		domodal();
		switch(hot.key) {
		case NextPortrait:
			if(index < portrait_indicies.getcount())
				index++;
			break;
		case PreviousPortrait:
			if(index > 0)
				index--;
			break;
		}
	}
	if(!getresult())
		return -1;
	return portrait_indicies[index];
}

void actor::choose_apearance(const char* title, const char* step_title) {
	coloration ch; ch.set(getportrait());
	while(ismodal()) {
		background(res::GUICGB, 0);
		draw::image(23, 151, res::PORTL, getportrait());
		draw::image(252, 61, gres(res::GUIHSB), 0, 0);
		label(174, 22, 452, 29, title, 2);
		label(173, 65, 453, 20, step_title);
		paperdoll(400, 215, ch, getrace(), getgender(), getclass());
		//
		label(328, 291, 152, 20, "Внешность", 3);
		labell(325, 325, 195, 20, "Главный цвет кожи");
		labell(325, 365, 195, 20, "Главный цвет волос");
		label(328, 399, 152, 20, "Экипировка", 3);
		labell(325, 432, 195, 20, "Основной цвет одежды");
		labell(325, 472, 195, 20, "Второстепенный цвет одежды");
		pickcolors({272, 314}, {272, 354}, {272, 422}, {272, 462}, ch);
		command_buttons();
		button(478, 550, buttonok, 0, 0, res::GBTNSTD, "Далее", KeyEnter);
		domodal();
	}
	if(getresult()) {
		colors.major = ch.major;
		colors.minor = ch.minor;
		colors.skin = ch.skin;
		colors.hair = ch.hair;
	}
}

static variant choose_gender(const creature& player, const char* title, const char* step_title) {
	const int dy = 198;
	while(ismodal()) {
		background(res::GUICGB, 0);
		draw::image(23, 151, res::PORTL, player.getportrait());
		label(174, 22, 452, 29, title, 2);
		label(173, 65, 453, 20, step_title);
		draw::image(252, 61, gres(res::GUISEX), 0, 0);
		int y = 270;
		button(274, y + dy * 0, select_variant, variant(Male), (current_variant == variant(Male)) ? Checked : 0, res::GBTNLRG, "Мужчина", Alpha + '1');
		button(274, y + dy * 1, select_variant, variant(Female), (current_variant == variant(Female)) ? Checked : 0, res::GBTNLRG, "Женщина", Alpha + '2');
		command_buttons();
		button(478, 550, next, 0, current_variant ? 0 : Disabled, res::GBTNSTD, "Далее", KeyEnter);
		view({570, 153, 754, 478}, {765, 151, 777, 481}, description, description_control);
		domodal();
	}
	current_variant = 0;
	return getresult();
}

bool creature::choose_skills(const char* title, const char* step_title, varianta& elements, char points, char points_per_skill, bool interactive) {
	if(!interactive) {
		elements.shuffle();
		for(auto v : elements) {
			if(!isclass((skill_s)v.value))
				continue;
			auto value = points_per_skill - skills[(skill_s)v.value];
			if(value <= 0)
				continue;
			if(value > points)
				value = points;
			points -= value;
			skills[(skill_s)v.value] += value;
			if(points <= 0)
				break;
		}
		return true;
	}
	struct scroll : public cbox {
		varianta&			elements;
		creature			copy;
		creature&			player;
		char				points_per_skill, points;
		const char* getname(stringbuilder& sb, int n) const {
			return getstr((skill_s)elements[n].value);
		}
		void row(rect rc, int i) const override {
			auto id = (skill_s)elements[i].value;
			int value = player.get(id);
			int value_cost = player.getcost(id);
			int value_maximum = points_per_skill / value_cost;
			int dy = rc.height() - 8;
			labell(rc.x1, rc.y1, 160, dy, getstr(id), 0, (value_cost > 1) ? 5 : 0);
			char temp[260]; stringbuilder sb(temp);
			sb.add("%+1i", player.get(id));
			label(rc.x1 + 180, rc.y1, 20, dy, temp);
			unsigned flags = 0;
			if(player.skills[id] <= copy.skills[id])
				flags = Disabled;
			button(rc.x1 + 230, rc.y1 - 3, button_minus, i, flags, res::GBTNMINS, 0);
			flags = 0;
			if(player.skills[id] >= value_maximum || points < value_cost)
				flags = Disabled;
			button(rc.x1 + 211, rc.y1 - 3, button_plus, i, flags, res::GBTNPLUS, 0);
			if(area(rc) == AreaHilitedPressed) {
				variant iv = id;
				stringbuilder sb(description);
				iv.addinfo(sb);
			}
		}
		int getmaximum() const override {
			return elements.getcount();
		}
		int getpixelsperline() const override {
			return 36;
		}
		scroll(creature& player, varianta& elements) : cbox({280, 154, 526, 506}, {529, 154, 540, 506}),
			player(player), copy(player), elements(elements) {
		}
	} table(*this, elements);
	char temp[32];
	table.points = points;
	table.points_per_skill = points_per_skill;
	while(ismodal()) {
		background(res::GUICGB, 0);
		draw::image(23, 151, res::PORTL, getportrait());
		label(174, 22, 452, 29, title, 2);
		label(173, 65, 453, 20, step_title);
		draw::image(252, 61, gres(res::GUIFEAT), 0, 0);
		label(279, 116, 205, 28, "Осталось очков навыков");
		sznum(temp, table.points);
		label(492, 118, 32, 28, temp);
		table.view();
		command_buttons();
		button(478, 550, buttonok, 0, (table.points <= 1) ? 0 : Disabled, res::GBTNSTD, "Далее", KeyEnter);
		view({570, 153, 754, 478}, {765, 151, 777, 481}, description, description_control);
		domodal();
		switch(hot.key) {
		case Plus:
			skills[table.elements[hot.param].value]++;
			table.points -= getcost((skill_s)table.elements[hot.param].value);
			break;
		case Minus:
			skills[table.elements[hot.param].value]--;
			table.points += getcost((skill_s)table.elements[hot.param].value);
			break;
		}
	}
	return getresult() != 0;
}

bool creature::choose_feats(const char* title, const char* step_title, varianta& elements, const unsigned* minimal, char points, bool interactive) {
	if(!points)
		return true;
	if(!interactive) {
		elements.shuffle();
		for(auto v : elements) {
			if(!isallow(v))
				continue;
			set((feat_s)v.value);
			if(--points <= 0)
				break;
		}
		return true;
	}
	struct scroll : clist {
		varianta&	elements;
		creature	copy;
		creature&	player;
		char		points;
		void row(rect rc, int i) override {
			auto id = (feat_s)elements[i].value;
			auto value = player.get(id);
			auto dy = rc.height() - 8;
			auto isallow = player.isallow(id);
			labell(rc.x1, rc.y1, 200, dy, getstr(id), 0, isallow ? 0 : 5);
			unsigned flags = 0;
			if(copy.is(id) || !player.is(id))
				flags = Disabled;
			button(rc.x1 + 230, rc.y1 - 3, button_minus, i, flags, res::GBTNMINS, 0);
			flags = 0;
			if(!isallow || player.is(id) || points <= 0)
				flags = Disabled;
			button(rc.x1 + 211, rc.y1 - 3, button_plus, i, flags, res::GBTNPLUS, 0);
			if(area(rc) == AreaHilitedPressed) {
				stringbuilder sb(description);
				variant iv = id;
				iv.addinfo(sb);
			}
		}
		scroll(creature& player, varianta& elements) : player(player), copy(player), elements(elements) {
			maximum = elements.count;
		}
	} table(*this, elements);
	char temp[32];
	table.points = points;
	while(ismodal()) {
		background(res::GUICGB, 0);
		draw::image(23, 151, res::PORTL, getportrait());
		label(174, 22, 452, 29, title, 2);
		label(173, 65, 453, 20, step_title);
		image(252, 61, gres(res::GUIFEAT), 1, 0);
		label(279, 116, 205, 28, "Осталось особенностей");
		sznum(temp, table.points);
		label(492, 118, 32, 28, temp);
		view({280, 154, 526, 506}, {529, 154, 540, 506}, 36, table);
		command_buttons();
		button(478, 550, buttonok, 0, (table.points <= 0) ? 0 : Disabled, res::GBTNSTD, "Далее", KeyEnter);
		view({570, 153, 754, 478}, {765, 151, 777, 481}, description, description_control);
		domodal();
		switch(hot.key) {
		case Plus:
			set((feat_s)table.elements[hot.param].value);
			table.points--;
			break;
		case Minus:
			remove((feat_s)table.elements[hot.param].value);
			table.points++;
			break;
		}
	}
	return getresult() != 0;
}

bool creature::choose_ability(const char* title, const char* step_title, bool add_race) {
	char temp[32]; stringbuilder sb(temp);
	char ability[6] = {10, 10, 10, 10, 10, 10};
	const auto cost_maximum = 25;
	while(ismodal()) {
		background(res::GUICGB, 0);
		draw::image(23, 151, res::PORTL, getportrait());
		label(174, 22, 452, 29, title, 2);
		label(173, 65, 453, 20, step_title);
		draw::image(252, 61, gres(res::GUIA), 0, 0);
		const int dy = 36;
		for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
			label(276, 155 + i * dy, 120, 28, getstr(i));
			auto value = ability[i];
			if(add_race)
				value += bsdata<racei>::elements[getrace()].abilities[i];
			sb.clear(); sb.add("%1i", value);
			label(414, 155 + i * dy, 33, 28, temp);
			auto bonus = value / 2 - 5;
			sb.clear(); sb.add("%+1i", value / 2 - 5);
			label(454, 155 + i * dy, 33, 28, temp, 0, (bonus > 0) ? 0x4D : (bonus < 0) ? 0x4C : 0);
			unsigned flags = 0;
			if(ability[i] >= 18 || getabilitycost(ability, 0) >= cost_maximum)
				flags = Disabled;
			button(491, 152 + i * dy, button_plus, i, flags, res::GBTNPLUS, 0);
			flags = 0;
			if(ability[i] <= 8)
				flags = Disabled;
			button(509, 152 + i * dy, button_minus, i, flags, res::GBTNMINS, 0);
		}
		label(276, 118, 208, 28, "Очков атрибутов");
		auto current_cost = getabilitycost(ability, 0);
		auto left_cost = cost_maximum - current_cost;
		sznum(temp, left_cost);
		label(492, 118, 32, 28, temp);
		command_buttons();
		button(478, 550, buttonok, 0, (left_cost == 0) ? 0 : Disabled, res::GBTNSTD, "Далее", KeyEnter);
		view({570, 153, 754, 478}, {765, 151, 777, 481}, description, description_control);
		domodal();
		switch(hot.key) {
		case Plus: ability[hot.param]++; break;
		case Minus: ability[hot.param]--; break;
		}
	}
	if(!getresult())
		return false;
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		creature::ability[i] = ability[i];
	return true;
}

static genstepi* findstep(variant_s value) {
	for(auto& e : bsdata<genstepi>()) {
		if(e.step == value)
			return &e;
	}
	return 0;
}

static const char* choose_name(const char* title) {
	screenshoot screen(true);
	char temp[260] = {0};
	while(ismodal()) {
		screen.restore();
		image(262, 171, gres(res::GUICNAME), 0, 0);
		button(280, 256, buttoncancel, 0, 0, res::GBTNSTD, "Назад", KeyEscape);
		label(283, 194, 233, 20, "Введите имя");
		field({286, 229, 506, 249}, temp, sizeof(temp));
		button(402, 256, buttonok, 1, temp[0] ? 0 : Disabled, res::GBTNSTD, "Далее", KeyEnter);
		domodal();
	}
	if(getresult())
		return szdup(temp);
	return 0;
}

static variant_s choose_step(creature& player, const char* title, const char* step_title, variant_s current) {
	stringbuilder sb(description);
	player.addinfo(sb, current);
	while(ismodal()) {
		background(res::GUICGB, 0);
		draw::image(23, 151, res::PORTL, player.getportrait());
		label(174, 22, 452, 29, title, 2);
		label(173, 65, 453, 20, step_title);
		int nid = 0;
		for(auto& e : bsdata<genstepi>()) {
			unsigned flags = (current == e.step) ? 0 : Disabled;
			button(274, 113 + 35 * nid, buttonok, nid, flags, res::GBTNLRG, getstr(e.step), Alpha + '1' + nid);
			nid++;
		}
		command_buttons(current);
		button(478, 550, next, 1, (current == Finish) ? 0 : Disabled, res::GBTNSTD, "Закончить", KeyEnter);
		view({570, 153, 754, 478}, {765, 151, 777, 481}, description, description_control);
		domodal();
		switch(hot.key) {
		case CreateNew:
			if(dlgask("Вы действительно хотите начать заново создавать этого персонажа?")) {
				player.clear(); sb.clear(); description[0] = 0;
				return bsdata<genstepi>::elements[0].step;
			}
			break;
		}
	}
	if(!getresult())
		return NoVariant;
	return current;
}

static variant_s prevstep(variant_s v) {
	auto result = NoVariant;
	for(auto e : bsdata<genstepi>()) {
		if(e.step == v)
			return result;
		result = e.step;
	}
	return result;
}

static variant_s nextstep(variant_s v) {
	auto need_break = false;
	if(v == NoVariant)
		return Gender;
	for(auto e : bsdata<genstepi>()) {
		if(need_break)
			return e.step;
		if(e.step == v)
			need_break = true;
	}
	return Finish;
}

void creature::generate(const char* title) {
	variant var;
	varianta elements;
	auto step = nextstep(NoVariant);
	while(true) {
		auto si = findstep(step);
		auto e1 = choose_step(*this, title, "Главное меню", step);
		if(e1 == step) {
			switch(e1) {
			case Finish:
				return;
			case Gender:
				var = choose_gender(*this, title, "Выбор пола");
				if(var) {
					auto portrait = choose_portrait(title, "Выбор портрета", (gender_s)var.value);
					if(portrait != -1) {
						set((gender_s)var.value);
						setportrait(portrait);
					}
				}
				break;
			case Ability:
				choose_ability(title, si->name, true);
				break;
			case Skill:
				choose_skills(title, elements, true);
				break;
			case Apearance:
				choose_apearance(title, si->name);
				break;
			case Name:
				if(true) {
					auto p = choose_name(si->name);
					if(p)
						name = p;
				}
				break;
			default:
				if(e1 == Class) {
					auto random = bsdata<racei>::elements[getrace()].favorite;
					if(!random)
						random = (class_s)xrand(Bard, Wizard);
					hot.param = variant(random);
					select_variant();
				}
				elements.select(si->from, si->to);
				elements.sort();
				var = choose(title, si->name, elements);
				if(var)
					set(var);
				break;
			}
			step = nextstep(step);
		} else if(e1 == NoVariant) {
			if(si > bsdata<genstepi>::begin()) {
				clear(si[-1].step);
				step = si[-1].step;
			} else
				return;
		}
	}
}