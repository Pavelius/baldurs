#include "main.h"

enum generation_event_s {
	NoGenerationEvent,
	PreviousPortrait, NextPortrait,
	Plus, Minus,
	CreateNew,
};

using namespace draw;

static variant			current_variant;
static char				description[1024 * 16];
static scrolltext		description_control;
static char				ability_cost[] = {0, 1, 2, 3, 4, 5, 6, 8, 10, 13, 16};

static void biography() {}

static void import_character() {}

static int getcost(const char* ability, const char* modifier) {
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

const char* creature::getdescription(variant id) {
	stringcreator dr;
	stringbuilder sb(dr, description);
	sb.add("###%1\n", getstr(id));
	switch(id.type) {
	case Race: getrule(sb, id.race); break;
	case Alignment: getrule(sb, id.alignment); break;
	case Feat: getrule(sb, id.feat); break;
	}
	return description;
}

static void select_variant() {
	current_variant = hot.param;
	creature::getdescription(current_variant);
}

static void next() {
	breakmodal(current_variant);
}

static void button_plus() {
	auto hi = hot;
	hi.key = Plus;
	execute(hi);
}

static void button_minus() {
	auto hi = hot;
	hi.key = Minus;
	execute(hi);
}

static aref<variant> createlist(aref<variant> result, variant v1, variant v2) {
	auto pb = result.begin();
	auto pe = result.end();
	for(auto i = v1; i.number <= v2.number; i.number++) {
		if(pb < pe)
			*pb++ = i;
	}
	result.count = pb - result.data;
	qsort(result.data, result.count, sizeof(result.data[0]), compare_variant);
	return result;
}

static void command_buttons(variant_s step = NoVariant) {
	button(35, 550, cmpr(biography), (step==Finish) ? 0 : Disabled, res::GBTNSTD, "Биография");
	button(204, 550, cmpr(import_character), 0, res::GBTNSTD, "Импортировать");
	button(342, 550, cmpr(buttoncancel), 0, res::GBTNSTD, "Назад", KeyEscape);
	button(647, 550, cmpr(buttonparam, CreateNew), (step == NoVariant || step != Gender) ? 0 : Disabled, res::GBTNSTD, "Начать заново");
}

static variant choose(const creature& player, const char* title, const char* step_title, aref<variant> elements) {
	while(ismodal()) {
		background(res::GUICGB, 0);
		draw::image(23, 151, res::PORTL, player.getportrait());
		label(174, 22, 452, 29, title, 2);
		label(173, 65, 453, 20, step_title);
		int nid = 0;
		for(auto e : elements) {
			unsigned flags = (current_variant == e) ? Checked : 0;
			button(274, 113 + 35 * nid, cmpr(select_variant, e), flags, res::GBTNLRG, getstr(e), Alpha + '1' + nid);
			nid++;
		}
		command_buttons();
		button(478, 550, cmpr(next), current_variant ? 0 : Disabled, res::GBTNSTD, "Далее", KeyEnter);
		view({570, 153, 754, 478}, {765, 151, 777, 481}, description, description_control);
		domodal();
	}
	current_variant = 0;
	return getresult();
}

static int choose_portrait(const char* title, const char* step_title, gender_s gender) {
	adat<short, 128> portrait_indicies;
	portrait_indicies.clear();
	auto col = portrait_info::getelements();
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
		button(260, 265, cmpr(buttonparam, PreviousPortrait), 0, res::GBTNPOR, 0, 0, 1, -1, 0, KeyLeft, 0, false);
		button(512, 265, cmpr(buttonparam, NextPortrait), 0, res::GBTNPOR, 0, 2, 3, -1, 0, KeyRight, 0, false);
		command_buttons();
		button(478, 550, cmpr(buttonok), 0, res::GBTNSTD, "Далее", KeyEnter);
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

//static int spells(int step, int* elements, int* result, int maximum) {
//	char temp[32];
//	int check = -1;
//	if(elements[0] == 0)
//		return Next;
//	result[0] = 0;
//	int maximum_available = zlen(elements);
//	szprint(description, "You may memorize %1i spells from %2's spells list.", maximum, "clerics");
//	draw::screenshoot copy;
//	while(true) {
//		int count = zlen(result);
//		copy.restore();
//		draw::image(252, 61, gres(res::GUISPL), 1, 0);
//		button(478, 550, Next, (count != maximum) ? Disabled : 0, res::GBTNSTD, 0, KeyEnter);
//		label(173, 65, 453, 20, "Memorize spells");
//		label(279, 116, 205, 28, "Spells left");
//		sznum(temp, maximum - count);
//		label(493, 116, 33, 28, temp);
//		int x = 272;
//		int y = 160;
//		int inc = 43;
//		for(int i = 0; i < maximum_available; i++) {
//			button_states state;
//			int rec = elements[i];
//			rect rc;
//			rc.x1 = x + (i % 6)*inc;
//			rc.y1 = y + (i / 6)*inc;
//			button(rc.x1, rc.y1, elements[i], zchr(result, rec) ? Checked : 0, res::SPLBUT, "", 0, 0, &state);
//			int pi = bsget(rec, Portrait);
//			if(true) {
//				draw::image(rc.x1 + 4 + (state == ButtonPressed ? 2 : 0),
//					rc.y1 + 4 + (state == ButtonPressed ? 2 : 0),
//					gres(res::SPELLS), pi, 0);
//			}
//		}
//		footer();
//		int id = draw::input();
//		switch(id) {
//		case 0:
//			return 0;
//		case Back:
//		case Next:
//			return id;
//		default:
//			if(id >= FirstSpell && id <= LastSpell) {
//				get_spell_description(description, id);
//				if(zchr(result, id)) {
//					//zremove(result, id);
//				} else if(count < maximum)
//					zcat(result, id);
//			}
//			break;
//		}
//	}
//}

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
		button(478, 550, cmpr(buttonok), 0, res::GBTNSTD, "Далее", KeyEnter);
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
		button(274, y + dy * 0, cmpr(select_variant, variant(Male)), (current_variant == variant(Male)) ? Checked : 0, res::GBTNLRG, "Мужчина", Alpha + '1');
		button(274, y + dy * 1, cmpr(select_variant, variant(Female)), (current_variant == variant(Female)) ? Checked : 0, res::GBTNLRG, "Женщина", Alpha + '2');
		command_buttons();
		button(478, 550, cmpr(next), current_variant ? 0 : Disabled, res::GBTNSTD, "Далее", KeyEnter);
		view({570, 153, 754, 478}, {765, 151, 777, 481}, description, description_control);
		domodal();
	}
	current_variant = 0;
	return getresult();
}

bool creature::choose_skills(const char* title, const char* step_title, aref<variant> elements, const char* minimal, char points, char points_per_skill, bool interactive) {
	if(!interactive) {
		zshuffle(elements.data, elements.count);
		for(auto v : elements) {
			if(!isclass(v.skill))
				continue;
			auto value = points_per_skill - skills[v.skill];
			if(value <= 0)
				continue;
			if(value > points)
				value = points;
			points -= value;
			skills[v.skill] += value;
			if(points <= 0)
				break;
		}
		return true;
	}
	struct scroll : public scrolllist {
		variant*			elements;
		const char*			minimal;
		creature&			player;
		char				points_per_skill, points;
		void row(rect rc, int i) override {
			char temp[260];
			auto id = elements[i].skill;
			int value = player.get(id);
			int value_cost = player.getcost(id);
			int value_maximum = points_per_skill / value_cost;
			int dy = rc.height() - 8;
			labell(rc.x1, rc.y1, 160, dy, getstr(id), 0, (value_cost > 1) ? 5 : 0);
			szprints(temp, zendof(temp), "%+1i", player.get(id));
			label(rc.x1 + 180, rc.y1, 20, dy, temp);
			unsigned flags = 0;
			if(player.skills[id] <= minimal[id])
				flags = Disabled;
			button(rc.x1 + 230, rc.y1 - 3, cmpr(button_minus, i), flags, res::GBTNMINS, 0);
			flags = 0;
			if(player.skills[id] >= value_maximum || points < value_cost)
				flags = Disabled;
			button(rc.x1 + 211, rc.y1 - 3, cmpr(button_plus, i), flags, res::GBTNPLUS, 0);
			if(area(rc) == AreaHilitedPressed)
				getdescription(id);
		}
		scroll(creature& player, const aref<variant>& elements, const char* minimal) : player(player), minimal(minimal), elements(elements.data) {
			maximum = elements.count;
		}
	} table(*this, elements, minimal);
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
		view({280, 154, 526, 506}, {529, 154, 540, 506}, 36, table);
		command_buttons();
		button(478, 550, cmpr(buttonok), (table.points <= 1) ? 0 : Disabled, res::GBTNSTD, "Далее", KeyEnter);
		view({570, 153, 754, 478}, {765, 151, 777, 481}, description, description_control);
		domodal();
		switch(hot.key) {
		case Plus:
			skills[table.elements[hot.param].skill]++;
			table.points -= getcost(table.elements[hot.param].skill);
			break;
		case Minus:
			skills[table.elements[hot.param].skill]--;
			table.points += getcost(table.elements[hot.param].skill);
			break;
		}
	}
	return getresult() != 0;
}

bool creature::choose_feats(const char* title, const char* step_title, aref<variant> elements, const unsigned* minimal, char points, bool interactive) {
	if(!points)
		return true;
	if(!interactive) {
		zshuffle(elements.data, elements.count);
		for(auto v : elements) {
			//if(is(v.feat))
			//	continue;
			if(!isallow(v))
				continue;
			set(v.feat);
			if(--points <= 0)
				break;
		}
		return true;
	}
	struct scroll : public scrolllist {
		variant*			elements;
		const unsigned*		minimal;
		creature&			player;
		char				points;
		void row(rect rc, int i) override {
			auto id = elements[i].feat;
			int value = player.get(id);
			int dy = rc.height() - 8;
			auto isallow = player.isallow(id);
			labell(rc.x1, rc.y1, 200, dy, getstr(id), 0, isallow ? 0 : 5);
			unsigned flags = Disabled;
			if(player.is(id))
				flags = 0;
			button(rc.x1 + 230, rc.y1 - 3, cmpr(button_minus, i), flags, res::GBTNMINS, 0);
			flags = 0;
			if(!isallow || player.is(id) || points <= 0)
				flags = Disabled;
			button(rc.x1 + 211, rc.y1 - 3, cmpr(button_plus, i), flags, res::GBTNPLUS, 0);
			if(area(rc) == AreaHilitedPressed)
				getdescription(id);
		}
		bool is(feat_s id) const {
			return (minimal[id / 32] & (1 << (id % 32))) != 0;
		}
		scroll(creature& player, const unsigned* minimal, const aref<variant>& elements) :
			player(player), minimal(minimal), elements(elements.data) {
			maximum = elements.count;
		}
	} table(*this, minimal, elements);
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
		button(478, 550, cmpr(buttonok), (table.points <= 0) ? 0 : Disabled, res::GBTNSTD, "Далее", KeyEnter);
		view({570, 153, 754, 478}, {765, 151, 777, 481}, description, description_control);
		domodal();
		switch(hot.key) {
		case Plus:
			set(table.elements[hot.param].feat);
			table.points--;
			break;
		case Minus:
			remove(table.elements[hot.param].feat);
			table.points++;
			break;
		}
	}
	return getresult() != 0;
}

static void choose_ability(creature& player, const char* title, const char* step_title) {
	char temp[32];
	char ability[6] = {10, 10, 10, 10, 10, 10};
	const auto cost_maximum = 25;
	while(ismodal()) {
		background(res::GUICGB, 0);
		draw::image(23, 151, res::PORTL, player.getportrait());
		label(174, 22, 452, 29, title, 2);
		label(173, 65, 453, 20, step_title);
		draw::image(252, 61, gres(res::GUIA), 0, 0);
		const int dy = 36;
		for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
			label(276, 155 + i * dy, 120, 28, getstr(i));
			auto value = ability[i] + race_data[player.getrace()].abilities[i];
			sznum(temp, value);
			label(414, 155 + i * dy, 33, 28, temp);
			szprints(temp, zendof(temp), "%+1i", value / 2 - 5);
			label(454, 155 + i * dy, 33, 28, temp, 0, (value > 0) ? 0x4D : (value < 0) ? 0x4C : 0);
			char modifiers[6];
			memset(modifiers, 0, sizeof(modifiers)); modifiers[i] = 1;
			unsigned flags = 0;
			if(ability[i] >= 18 || getcost(ability, modifiers) > cost_maximum)
				flags = Disabled;
			button(491, 152 + i * dy, cmpr(button_plus, i), flags, res::GBTNPLUS, 0);
			flags = 0;
			if(ability[i] <= 8)
				flags = Disabled;
			button(509, 152 + i * dy, cmpr(button_minus, i), flags, res::GBTNMINS, 0);
		}
		label(276, 118, 208, 28, "Очков атрибутов");
		auto current_cost = getcost(ability, 0);
		auto left_cost = cost_maximum - current_cost;
		sznum(temp, left_cost);
		label(492, 118, 32, 28, temp);
		command_buttons();
		button(478, 550, cmpr(buttonok), (left_cost == 0) ? 0 : Disabled, res::GBTNSTD, "Далее", KeyEnter);
		view({570, 153, 754, 478}, {765, 151, 777, 481}, description, description_control);
		domodal();
		switch(hot.key) {
		case Plus: ability[hot.param]++; break;
		case Minus: ability[hot.param]--; break;
		}
	}
	if(getresult()) {
		for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
			player.set(i, ability[i] + race_data[player.getrace()].abilities[i]);
	}
}

static struct step_info {
	variant_s		step;
	const char*		name;
	variant			from, to;
} steps[] = {{Gender, "Выбирайте пол", Male, Female},
{Race, "Выбирайте расу", FirstRace, LastRace},
{Class, "Выбирайте класс", FirstClass, LastClass},
{Alignment, "Выбирайте мировозрение", LawfulGood, ChaoticEvil},
{Ability, "Способности"},
{Skill, "Навыки"},
{Apearance, "Внешний вид"},
{Name, "Имя"},
};

static step_info* find(variant_s value) {
	for(auto& e : steps) {
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
		button(280, 256, cmpr(buttoncancel), 0, res::GBTNSTD, "Назад", KeyEscape);
		label(283, 194, 233, 20, "Введите имя");
		field({286, 229, 506, 249}, temp, sizeof(temp));
		button(402, 256, cmpr(buttonok), temp[0] ? 0 : Disabled, res::GBTNSTD, "Далее", KeyEnter);
		domodal();
	}
	if(getresult())
		return szdup(temp);
	return 0;
}

static variant_s choose_step(creature& player, const char* title, const char* step_title, variant_s current) {
	stringcreator sc;
	stringbuilder sb(sc, description);
	player.getdescription(sb);
	while(ismodal()) {
		background(res::GUICGB, 0);
		draw::image(23, 151, res::PORTL, player.getportrait());
		label(174, 22, 452, 29, title, 2);
		label(173, 65, 453, 20, step_title);
		int nid = 0;
		for(auto& e : steps) {
			unsigned flags = (current == e.step) ? 0 : Disabled;
			button(274, 113 + 35 * nid, cmpr(buttonok), flags, res::GBTNLRG, getstr(e.step), Alpha + '1' + nid);
			nid++;
		}
		command_buttons(current);
		button(478, 550, cmpr(next), (current == Finish) ? 0 : Disabled, res::GBTNSTD, "Закончить", KeyEnter);
		view({570, 153, 754, 478}, {765, 151, 777, 481}, description, description_control);
		domodal();
		switch(hot.key) {
		case CreateNew:
			if(dlgask("Вы действительно хотите начать заново создавать этого персонажа?")) {
				player.clear();
				sb.clear();
				player.getdescription(sb);
				return steps[0].step;
			}
			break;
		}
	}
	if(!getresult())
		return NoVariant;
	return current;
}

void creature::generate(const char* title) {
	variant var;
	variant elements[128];
	while(true) {
		auto step = getstep();
		auto si = find(step);
		auto e1 = choose_step(*this, title, "Главное меню", step);
		if(e1 == step) {
			switch(e1) {
			case Finish:
				return;
			case Gender:
				var = choose_gender(*this, title, "Выбор пола");
				if(var) {
					auto portrait = choose_portrait(title, "Выбор портрета", var.gender);
					if(portrait != -1) {
						set(var.gender);
						setportrait(portrait);
					}
				}
				break;
			case Ability:
				choose_ability(*this, title, si->name);
				break;
			case Skill:
				choose_skills(title, elements);
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
				var = choose(*this, title, si->name, select(elements, si->from, si->to, true));
				if(var)
					set(var);
				break;
			}
		} else if(e1 == NoVariant) {
			if(si > steps)
				clear(si[-1].step);
			else
				return;
		}
	}
}