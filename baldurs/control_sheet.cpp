#include "main.h"

using namespace draw;

enum info_page_s : unsigned char {
	InfoCharacter, InfoWeapons, InfoSkills, InfoStatistic
};

static const char*		ability_short_name[] = {"���", "���", "���", "���", "���", "���"};
static info_page_s		current_step;
static scrolltext		textarea;

static void choose_page() {
	current_step = (info_page_s)hot.param;
}

void switcher(int x, int y, info_page_s id) {
	unsigned flags = 0;
	if(current_step == id)
		flags |= Checked;
	button(x, y, cmpr(choose_page, id), flags, res::GBTNRECB, 0, 0);
}

static void information() {
	setnext(creature::help);
}

static void biography() {}

static void levelup() {}

void creature::sheet() {
	char temp[8192];
	variant elements[128];
	stringcreator sc;
	stringbuilder sb(sc, temp);
	background(res::GUIREC);
	draw::image(20, 79, gres(res::PORTL), getportrait(), 0);
	label(22, 23, 206, 28, getname(), 2);
	label(258, 23, 115, 28, getstr(race));
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		auto y = 78 + 39 * i;
		auto value = get(i);
		label(253, y, 45, 30, ability_short_name[i], 2);
		label(304, y, 32, 30, szprints(temp, zendof(temp), "%1i", getr(i)));
		label(345, y, 32, 30, szprints(temp, zendof(temp), "%+1i", value), 0, (value > 0) ? 0x4D : (value < 0) ? 0x4C : 0);
	}
	label(463, 381, 32, 30, szprints(temp, zendof(temp), "%1i", getac(false)), 3);
	label(585, 378, 54, 16, szprints(temp, zendof(temp), "%1i", gethits()), 3);
	label(585, 399, 54, 16, szprints(temp, zendof(temp), "%1i", gethitsmax()), 3);
	switcher(430, 22, InfoCharacter);
	switcher(512, 22, InfoWeapons);
	switcher(594, 22, InfoSkills);
	switcher(676, 22, InfoStatistic);
	button(256, 307, cmpr(information), 0, res::GBTNSTD, "����������");
	button(256, 361, cmpr(biography), 0, res::GBTNSTD, "���������");
	button(256, 334, cmpr(biography), 0, res::GBTNSTD, "�������");
	button(256, 388, cmpr(biography), 0, res::GBTNSTD, "Cutomize");
	button(655, 379, cmpr(levelup), Disabled, res::GBTNSTD, "������� �������");
	temp[0] = 0;
	switch(current_step) {
	case InfoSkills:
		add(sb, FirstSkill, LastSkill, "������");
		add(sb, FirstFeat, LastFeat, "�����������");
		break;
	case InfoCharacter:
		addinfo(sb);
		break;
	}
	view({410, 64, 406 + 349, 64 + 288}, {768, 64, 780, 358}, temp, textarea);
}