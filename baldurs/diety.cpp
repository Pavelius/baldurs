#include "main.h"

static struct diety_info {
	const char*	id;
	const char*	name;
} diety_data[] = {{"No diety"},
// Gods
{"Bane", "Бейн"},
{"Chauntea", "Шаунти"},
{"Illmater", "Ильматтер"},
{"Gruumsh", "Грумш"},
{"Helm", "Хельм"},
{"Kelemvor", "Келемвор"},
{"Kossuth", "Коссус"},
{"Lathander", "Латандер"},
{"Lolth", "Ллос"},
{"Mask", "Маска"},
{"Mystra", "Мистра"},
{"Moradin", "Морадин"},
{"Oghma", "Огма"},
{"Shar", "Чар"},
{"Tempus", "Темпус"},
{"Tymora", "Тимора"},
{"Tyr", "Тир"},
{"Umberly", "Умбрелла"},
};
assert_enum(diety, GodUmberly);
getstr_enum(diety);