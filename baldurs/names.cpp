#include "main.h"

static struct name_info {
	race_s		race;
	gender_s	gender;
	const char*	name;
} name_data[] = {{Dwarf, Male, "Адрик"},
{Dwarf, Male, "Альберих"},
{Dwarf, Male, "Баренд"},
{Dwarf, Male, "Баэрн"},
{Dwarf, Male, "Броттор"},
{Dwarf, Male, "Бруенор"},
{Dwarf, Male, "Вондал"},
{Dwarf, Male, "Вэйт"},
{Dwarf, Male, "Гардаин"},
{Dwarf, Male, "Даин"},
{Dwarf, Male, "Даррак"},
{Dwarf, Male, "Делг"},
{Dwarf, Male, "Килдрак"},
{Dwarf, Male, "Моргран"},
{Dwarf, Male, "Орсик"},
{Dwarf, Male, "Оскар"},
{Dwarf, Male, "Рангрим"},
{Dwarf, Male, "Рюрик"},
{Dwarf, Male, "Таклинн"},
{Dwarf, Male, "Торадин"},
{Dwarf, Male, "Тордек"},
{Dwarf, Male, "Торин"},
{Dwarf, Male, "Травок"},
{Dwarf, Male, "Траубон"},
{Dwarf, Male, "Ульфгар"},
{Dwarf, Male, "Фаргрим"},
{Dwarf, Male, "Флинт"},
{Dwarf, Male, "Харбек"},
{Dwarf, Male, "Эберк"},
{Dwarf, Male, "Эйнкиль"},
//
{Dwarf, Female, "Артин"},
{Dwarf, Female, "Бардрин"},
{Dwarf, Female, "Вистра"},
{Dwarf, Female, "Гуннлода"},
{Dwarf, Female, "Гурдис"},
{Dwarf, Female, "Дагнал"},
{Dwarf, Female, "Диеза"},
{Dwarf, Female, "Илде"},
{Dwarf, Female, "Катра"},
{Dwarf, Female, "Кристид"},
{Dwarf, Female, "Лифтраса"},
{Dwarf, Female, "Мардред"},
{Dwarf, Female, "Одхильд"},
{Dwarf, Female, "Рисвин"},
{Dwarf, Female, "Саннл"},
{Dwarf, Female, "Торбера"},
{Dwarf, Female, "Торгга"},
{Dwarf, Female, "Фалкрунн"},
{Dwarf, Female, "Финеллен"},
{Dwarf, Female, "Хельджа"},
{Dwarf, Female, "Хлин"},
{Dwarf, Female, "Эльдет"},
{Dwarf, Female, "Эмбер"},
//
{Elf, Male, "Адран"},
{Elf, Male, "Арамиль"},
{Elf, Male, "Араннис"},
{Elf, Male, "Ауст"},
{Elf, Male, "Аэлар"},
{Elf, Male, "Бейро"},
{Elf, Male, "Берриан"},
{Elf, Male, "Варис"},
{Elf, Male, "Галинндан"},
{Elf, Male, "Ивеллиос"},
{Elf, Male, "Иммераль"},
{Elf, Male, "Каррик"},
{Elf, Male, "Куарион"},
{Elf, Male, "Лаусиан"},
{Elf, Male, "Миндартис"},
{Elf, Male, "Паэлиас"},
{Elf, Male, "Перен"},
{Elf, Male, "Риардон"},
{Elf, Male, "Ролен"},
{Elf, Male, "Совелисс"},
{Elf, Male, "Тамиорн"},
{Elf, Male, "Таривол"},
{Elf, Male, "Терен"},
{Elf, Male, "Хадарай"},
{Elf, Male, "Химо"},
{Elf, Male, "Хэйян"},
{Elf, Male, "Эниалис"},
{Elf, Male, "Эрдан"},
{Elf, Male, "Эреван"},
//
{Elf, Female, "Адрие"},
{Elf, Female, "Альтеа"},
{Elf, Female, "Анастрианна"},
{Elf, Female, "Андрасте"},
{Elf, Female, "Антинуа"},
{Elf, Female, "Бетринна"},
{Elf, Female, "Бирель"},
{Elf, Female, "Вадания"},
{Elf, Female, "Валанте"},
{Elf, Female, "Джеленетт"},
{Elf, Female, "Друсилиа"},
{Elf, Female, "Йелениа"},
{Elf, Female, "Каэлинн"},
{Elf, Female, "Квеленна"},
{Elf, Female, "Квиласи"},
{Elf, Female, "Кейлет"},
{Elf, Female, "Ксанафия"},
{Elf, Female, "Лешанна"},
{Elf, Female, "Лиа"},
{Elf, Female, "Миали"},
{Elf, Female, "Мэриэль"},
{Elf, Female, "Найвара"},
{Elf, Female, "Сариэль"},
{Elf, Female, "Силакви"},
{Elf, Female, "Теирастра"},
{Elf, Female, "Тиа"},
{Elf, Female, "Фелосиаль"},
{Elf, Female, "Шава"},
{Elf, Female, "Шанайра"},
{Elf, Female, "Энна"},
//
{Human, Male, "Айвор"}, // Даммарцы
{Human, Male, "Бор"},
{Human, Male, "Глэр"},
{Human, Male, "Григор"},
{Human, Male, "Иган"},
{Human, Male, "Козеф"},
{Human, Male, "Миваль"},
{Human, Male, "Орел"},
{Human, Male, "Павел"},
{Human, Male, "Сергор"},
{Human, Male, "Фодель"},
{Human, Female, "Алетра"},
{Human, Female, "Зора"},
{Human, Female, "Кара"},
{Human, Female, "Катернин"},
{Human, Female, "Мара"},
{Human, Female, "Натали"},
{Human, Female, "Ольма"},
{Human, Female, "Тана"},
{Human, Male, "Блас"},// Иллусканцы
{Human, Male, "Бран"},
{Human, Male, "Гет"},
{Human, Male, "Ландер"},
{Human, Male, "Лют"},
{Human, Male, "Малсер"},
{Human, Male, "Стор"},
{Human, Male, "Таман"},
{Human, Male, "Урт"},
{Human, Male, "Фрат"},
{Human, Male, "Эндер"},
{Human, Female, "Амафрея"},
{Human, Female, "Бетха"},
{Human, Female, "Вестра"},
{Human, Female, "Кетра"},
{Human, Female, "Мара"},
{Human, Female, "Ольга"},
{Human, Female, "Силифрей"},
{Human, Female, "Цефрея"},
};

static aref<short unsigned> select(aref<short unsigned> result, gender_s gender, race_s race) {
	auto pb = result.data;
	auto pe = result.data + result.count;
	for(unsigned short i = 0; i < sizeof(name_data) / sizeof(name_data[0]); i++) {
		if(gender && name_data[i].gender != gender)
			continue;
		if(name_data[i].race != race)
			continue;
		if(pb < pe)
			*pb++ = i;
	}
	result.count = pb - result.data;
	return result;
}

const char* creature::random_name(gender_s gender, race_s race) {
	short unsigned source[128];
	if(race == HalfElf) {
		if(d100() < 50)
			race = Human;
		else
			race = Elf;
	}
	auto result = select(source, gender, race);
	return name_data[result.data[rand()%result.count]].name;
}

void creature::random_name() {
	name = random_name(gender, race);
}