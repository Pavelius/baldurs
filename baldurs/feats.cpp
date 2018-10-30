#include "main.h"

static const char* levels_rome[6] = {"I", "II", "III", "IV", "V", "VI"};
static const char* levels_naming[6] = {"1st", "2nd", "3rd", "4th", "5th", "6th"};

enum prerequisit_s : unsigned char {
	NoPrerequisit,
	ProfecienceWithWeapon, ThreeOrMoreNaturalAttack,
};

struct feat_info {
	const char*		id;
	const char*		name;
	char			ability[6];
	feat_s			prerequisit[4];
	char			base_attack;
	char			character_level;
	const char*		text;
	const char*		benefit;
	prerequisit_s	prerequisit_special;
} feat_data[] = {{"None"},
{"Alertness", "Бдительность"},
{"Armor Profeciency (Light)", "Ношение легкой брони"},
{"Armor Profeciency (Medium)", "Ношение средней брони", {}, {ArmorProfeciencyLight}},
{"Armor Profeciency (Heavy)", "Ношение тяжелой брони", {}, {ArmorProfeciencyLight, ArmorProfeciencyMedium}},
{"Blind-Fight", "Сражение вслепую"},
{"Cleave", "Рассечение", {13}, {PowerAttack}},
{"Combat Casting", "Боевые заклинания"},
{"Combat Expertise", "Боевой опыт", {0, 0, 0, 13}, {}, 0, 0, "Вы хорошо владеете своими боевыми навыками как в атаке, так и в защите."},
{"Combat Reflexes", "Боевые рефлексы", {}, {},0,0, "Вы способны молниеносно наносить ответные атаки, если ваш противник теряет бдительность.", "Если враг оставляет в поединке себя открытым, вы способны провести благоприятные атаки. Их количество равно модификатору вашей Ловкости."},
{"Deadly Precision", "Смертельная меткость", {0, 15}, {}, 5},
{"Deflects Arrows", "Отбивание стрел", {0, 13}, {ImprovedUnarmedStrike}},
{"Dodge", "Уклонение", {0, 13}},
{"Endurance", "Выносливость"},
{"Far Shoot", "Дальний выстрел", {}, {PointBlankShoot}},
{"Greate Fortitude", "Великая стойкость"},
{"Improved Critical", "Улучшенный критический", {}, {}, 8, 0, ""},
{"Improved Disarm", "Улучшенное обезоруживание", {0, 0, 0, 13}, {CombatExpertise}},
{"Improved Feint", "Улучшенный финт", {0, 0, 0, 13}, {CombatExpertise}},
{"Improved Initiative", "Улучшенная инициатива"},
{"Improved Overrun", "Улучшенный натиск", {13}, {PowerAttack}},
{"Improved Sunder", "Улучшенное разрушение", {13}, {PowerAttack}},
{"Improved Trip", "Улучшенная подсечка", {0, 0, 0, 13}, {CombatExpertise}},
{"Improved Unarmed Strike", "Улучшенный рукопашный бой"},
{"Iron Will", "Железная воля"},
{"Leadership", "Лидерство", {}, {}, 0, 6},
{"Lighting Reflexes", "Молниеносные рефлексы"},
{"Mobiliy", "Подвижность", {0, 13}, {Dodge}},
{"Multiattack", "Множество атак", {}, {}, 0, 0, ""},
{"Point-Blank Shoot", "Выстрел навскидку"},
{"Power Attack", "Мощная атака", {13}},
{"Precise Shoot", "Меткий выстрел", {}, {PointBlankShoot}},
{"Rapid Shoot", "Быстрый выстрел", {13}, {PointBlankShoot}},
{"Rapid Reload", "Быстрая перезарядка"},
{"Shield Profeciency", "Владение щитом"},
{"Short On the Run", "Выстрел на бегу", {0, 13}, {Dodge, Mobiliy, PointBlankShoot}, 4},
{"Spell Focus", "Фокус на школе заклинаний"},
{"Spring Attack", "Атака в подскоке", {0, 13}, {Dodge, Mobiliy}, 4},
{"Stunning Fist", "Парализующий удар рукой", {0, 13, 0, 0, 13}, {ImprovedUnarmedStrike}, 8},
{"Toughness", "Живучесть"},
{"Track", "Выслеживание"},
{"Two Weapon Fighting", "Сражение двумя оружиями", {15}},
{"Focus axes", "Фокус на топорах", {0}, {ProficiencyAxe}, 1},
{"Focus bows", "Фокус на луках", {0}, {ProficiencyShortbow}, 1},
{"Focus crossbows", "Фокус на арбалетах", {0}, {ProficiencyCrossbow}, 1},
{"Focus daggers", "Фокус на кинжалах", {0}, {ProficiencyDagger}, 1},
{"Focus greatswords", "Фокус на двуручных мечах", {0}, {ProficiencyGreatweapon}, 1},
{"Focus longswords", "Фокус на длинных мечах", {0}, {ProficiencyLongsword}, 1},
{"Focus maces", "Фокус на булаве", {0}, {ProficiencyMace}, 1},
{"Focus polearm", "Фокус на древковом оружии", {0}, {ProficiencySimple}, 1},
{"Focus shortswords", "Фокус на коротких мечах", {0}, {ProficiencyShortsword}, 1},
{"WhirlwindAttack", "Атака вихрем", {0, 13, 0, 13}, {CombatExpertise, Dodge, Mobiliy, SpringAttack}, 4},
//
{"Axe Proficiency", "Владение топором", {}},
{"Club Proficiency", "Владение дубиной", {}},
{"Crossbow Proficiency", "Владение арбалетом", {}},
{"Dagger Proficiency", "Владение кинжалом", {}},
{"Greatweapon Proficiency", "Владение большим оружием", {ProficiencyLongsword}},
{"Heavy Crossbow Proficiency", "Владение тяжелым арбалетом", {}},
{"Mace Proficiency", "Владение булавой", {}},
{"Simple weapon Proficiency", "Владение простым оружием", {}},
{"Spear Proficiency", "Владение копьем", {}},
{"Longbow Proficiency", "Владение длинным луком", {}},
{"Longsword Proficiency", "Владение длинным мечом", {}},
{"Shortbow Proficiency", "Владение коротким луком", {}},
{"Scimitar Proficiency", "Владение саблей", {}},
{"Shortsword Proficiency", "Владение коротким мечом", {}},
{"Bastardsword Proficiency", "Владение полуторным мечем", {ProficiencyLongsword}},
{"Catana Proficiency", "Владение катаной", {}},
{"Waraxe Proficiency", "Владение военным топором", {ProficiencyAxe}},
//
{"FastMovement", "Быстрое передвижение"},
{"Illiteracy", "Необразованный"},
//
{"Hate orc and goblins", "Ненависть к оркам и гоблинам", {}, {}, 0, 0, "+1 бонус к атаке орков (включая полу-орков) и гоблинов (включая кобольдов и хобовлинов)."},
{"Infravision", "Инфрарение", {}, {}, 0, 0, "Может видеть в темноте на 20 метров и не получает штрафы при сражении в темноте."},
{"Immunity to Sleep spell", "Иммунитет к заклинанию Сон", {}, {}, 0, 0, "Иммунитет к заклинанию [Сон] и всем другим подобным заклинаниям, которые могут усыпить."},
{"Find secrets", "Находит секреты", {}, {}, 0, 0, "Находятсь в 4 метрах от секрета имеется шанс его найти - скрытной делается бросок поиска."},
{"Lucky", "Удачливый", {}, {}, 0, 0, "Бонус +1 ко всем спас-броскам."},
{"Precise Thrower", "Меткий метатель", {}, {}, 0, 0, "+1 к броску атаки, со всем метательным оружием."},
{"Save Bonus Vs Echantment", "Бонус защиты от очарований", {}, {}, 0, 0, "+2 к спас-броску от заклинаний Очарования."},
{"Save Bonus Vs Fear", "Бонус защиты от страха", {}, {}, 0, 0, "+2 к спас-броску от страха."},
{"Save Bonus Vs Illusion", "Бонус защиты от иллюзий", {}, {}, 0, 0, "+2 к спас-броску от заклинаний Иллюзий."},
{"Save Bonus Vs Poison", "Бонус защиты от яда", {}, {}, 0, 0, "+2 к спас-броску от ядов."},
{"Save Bonus Vs Spells", "Бонус защиты от заклинаний", {}, {}, 0, 0, "+2 к спас-броску от заклинаний и магии."},
{"Stability", "Устойчивость", {}, {}, 0, 0, "+4 к защите от попыток сделать подсечку или обросить под натичком назад."},
{"Stonecunning", "Обработка камня", {}, {}, 0, 0, "+2 к Поиску при попытке обнаружить ловушки в подземелье. Делают это аналогично ворам."},
};
assert_enum(feat, LastFeat);
getstr_enum(feat);

bool creature::isallow(feat_s id) const {
	for(auto e = Strenght; e <= Charisma; e = (ability_s)(e+1)) {
		auto value = feat_data[id].ability[e];
		if(value && getr(e) < value)
			return false;
	}
	for(auto e : feat_data[id].prerequisit) {
		if(e && !is(e))
			return false;
	}
	if(feat_data[id].base_attack && getbab() < feat_data[id].base_attack)
		return false;
	if(feat_data[id].character_level && getcharlevel() < feat_data[id].character_level)
		return false;
	if(feat_data[id].prerequisit_special)
		return false;
	return true;
}

bool creature::isallow(feat_s id, const unsigned char* ability, char character_level, char base_attack) {
	for(auto e = Strenght; e <= Charisma; e = (ability_s)(e + 1)) {
		auto value = feat_data[id].ability[e];
		if(value && ability[e] < value)
			return false;
	}
	if(feat_data[id].base_attack && base_attack < feat_data[id].base_attack)
		return false;
	if(feat_data[id].character_level && character_level < feat_data[id].character_level)
		return false;
	return true;
}

static void add_description(stringbuilder& sb, feat_s id, const char* prefix = 0) {
	if(feat_data[id].text && feat_data[id].text[0]) {
		if(prefix)
			sb.add(prefix);
		sb.add(feat_data[id].text);
	}
}

static void add_required(stringbuilder& sb, feat_s id) {
	auto p = sb.getpos();
	auto h = "\n\n[Требует:] ";
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i+1)) {
		if(!feat_data[id].ability[i])
			continue;
		sb.sep(h, p);
		sb.add("%1 %2i", getstr(i), feat_data[id].ability[i]);
	}
	for(auto e : feat_data[id].prerequisit) {
		if(!e)
			break;
		sb.sep(h, p);
		sb.add(getstr(e));
	}
	if(feat_data[id].base_attack) {
		sb.sep(h, p);
		sb.add("базовая атака %+1i", feat_data[id].base_attack);
	}
	if(feat_data[id].character_level) {
		sb.sep(h, p);
		sb.add("уровень персонажа %+1i+", feat_data[id].character_level);
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

template<> void getrule<feat_s>(stringbuilder& sb, feat_s id) {
	add_description(sb, id);
	add_header(sb, "Преемущество", feat_data[id].benefit);
	add_required(sb, id);
}

void add_feat(stringbuilder& sb, feat_s id) {
	sb.add("\n\n[**%1**]", getstr(id));
	add_description(sb, id, ": ");
}

//void get_race_description(char* result, int id) {
//	char* p;
//	szprint(result, "###%1\n", bsgets(id, Name));
//	const char* pb = races[id - FirstRace].benefit;
//	// Descritpion
//	if(pb) {
//		zcat(result, pb);
//		zcat(result, "\n");
//	}
//	// Abilities
//	p = zend(result);
//	for(int i = FirstAbility; i <= LastAbility; i++) {
//		int v = get_race_modifier(id, i);
//		if(v)
//			szprint(strnew(p, "[Abilities]:\n"), " %1 %+2i\n", bsgets(i, Name), v);
//	}
//	szprint(zend(p), "[Base speed]: %1i ft.\n", get_base_speed(id));
//	// Proficiency
//	p = zend(result);
//	for(int i = ProfeciencyArmor; i <= ProficiencyUnarmed; i++) {
//		int v = get_race(id, i);
//		if(v)
//			szprint(strnew(p, "[Proficiency]:\n"), " %1\n", bsgets(i, Name), v);
//	}
//	// Racial bonus
//	p = zend(result);
//	for(int i = FirstSkill; i <= LastSkill; i++) {
//		int v = get_race(id, i);
//		if(v)
//			szprint(strnew(p, "[Racial bonus]:\n"), " %1 %+2i\n", bsgets(i, Name), v);
//	}
//	// Best class
//	if(true) {
//		int v = get_race(id, BestClass);
//		if(v) {
//			if(v == Class)
//				szprint(zend(p), "[Favored class]: Any\n", bsgets(v, Name));
//			else
//				szprint(zend(p), "[Favored class]: %1\n", bsgets(v, Name));
//		}
//	}
//}

//void get_skill_description(char* result, int id) {
//	szprint(result, "###%1\n", bsgets(id, Name));
//	const char* pb = get_benefit(skills, id);
//	const char* ps = get_special(skills, id);
//	szprint(zend(result), "[Key Ability]: ");
//	int count = 0;
//	int ka = get_skill_ability(id);
//	if(ka) {
//		count++;
//		szprint(zend(result), "%1", bsgets(ka, Name));
//	}
//	if(is_skill(id, ArmorCheckPenalty)) {
//		if(count++)
//			zcat(result, "; ");
//		zcat(result, "Armor check penalty");
//	}
//	if(is_skill(id, TrainedOnly)) {
//		if(count++)
//			zcat(result, "; ");
//		zcat(result, "Trained only");
//	}
//	zcat(result, "\n");
//	if(pb && pb[0]) {
//		zcat(result, pb);
//		zcat(result, "\n");
//	}
//	if(ps && ps[0]) {
//		zcat(result, "[Special]: ");
//		zcat(result, ps);
//		zcat(result, "\n");
//	}
//}