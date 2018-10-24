#include "main.h"

school_info school_data[] = {{"NoSchool"},
{"Abjuration", "Оберегание"},
{"Conjuration", "Вызов"},
{"Divination", "Предсказание"},
{"Enchantment", "Очарование"},
{"Evocation", "Воплощение"},
{"Illusion", "Иллюзия"},
{"Necromancy", "Некромантия"},
{"Transmutation", "Преобразование"},
};
assert_enum(school, Transmutation);
getstr_enum(school);