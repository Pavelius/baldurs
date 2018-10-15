#include "main.h"

static struct alignment_info {
	const char*		id;
	const char*		name;
} alignment_data[] = {{"No alignment", "Нет мировозрения"},
{"Lawful Good", "Законопослушный добрый"},
{"Neutral Good", "Нейтрально добрый"},
{"Chaotic Good", "Хаотично добрый"},
{"Lawful Neutral", "Законопослушный нейтральный"},
{"True Neutral", "Нейтральный"},
{"Chaotic Neutral", "Хаотично нейтральный"},
{"Lawful Evil", "Законопослушный злой"},
{"Neutral Evil", "Нейтрально злой"},
{"Chaotic Evil", "Хаотично злой"},
};
assert_enum(alignment, ChaoticEvil);
getstr_enum(alignment);