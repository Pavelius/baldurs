#include "main.h"

using namespace draw;

static unsigned char colors_hair[] = {79, 110, 5, 3, 0};
static unsigned char colors_skin[] = {107, 8, 114, 9, 10, 85, 84, 12, 16, 15, 17, 108, 106, 113, 14, 13, 105, 83};
static unsigned char colors_major[] = {41, 39, 38, 47, 54, 66, 51, 40, 100, 49, 58, 60, 22, 46, 63, 94, 107, 50, 62, 37, 65};
static unsigned char colors_minor[] = {65, 27, 49, 60, 66, 26, 41, 37, 58, 14, 46, 98, 53, 48, 24, 56, 40, 54, 103, 25, 102, 91, 47, 63, 51, 45, 38, 39};

typedef aref<unsigned char>	colora;

static unsigned char*		current_value;
static colora				current_elements;

static void pick_color() {
	if(current_value) {
		*current_value = hot.param;
		buttonok();
	}
}

static void choose() {
	screenshoot push(true);
	while(ismodal()) {
		push.restore();
		image(300, 112, res::COLOR, 0);
		auto x = 320, y = 163, inc = 28;
		for(unsigned i = 0; i < current_elements.count; i++) {
			int px = i % 6;
			int py = i / 6;
			int id = current_elements.data[i + 1];
			if(picker(x + px * inc, y + py * inc, id, 0))
				execute(pick_color, id);
		}
		button(322, 332, buttoncancel, 0, res::GBTNMED, "Отмена", KeyEscape);
		label(331, 134, 135, 22, "Выбирайте цвет");
		domodal();
	}
}

bool draw::picker(int x, int y, unsigned char index, int type) {
	auto result = false;
	color temp[256];
	button_states state;
	unsigned char ns = 0;
	unsigned char ps = 1;
	if(type == 0) {
		ns = 2;
		ps = 3;
	}
	result = button(x, y, 0, res::INVBUT2, ns, ns, ps, ns, 0, 0, &state, false);
	if(state == ButtonPressed) {
		x += 2;
		y += 2;
	}
	sprite* pb = gres(res::COLGRAD);
	if(!pb)
		return false;
	memcpy(temp, pb->offs(pb->get(type).pallette), sizeof(color) * 16);
	set(temp, index);
	draw::image(x, y, pb, type, 0, 0xFF, temp);
	return result;
}

void actor::pickcolors(const point skin, const point hair, const point major, const point minor) {
	pickcolors(skin, hair, major, minor, colors);
}

static void choose_picker(int x, int y, unsigned char& value, int index, const colora& elements) {
	if(picker(x, y, value, index)) {
		current_elements = elements;
		current_value = &value;
		execute(choose);
	}
}

void actor::pickcolors(const point skin, const point hair, const point major, const point minor, coloration& colors) {
	choose_picker(skin.x, skin.y, colors.skin, 1, colors_skin);
	choose_picker(hair.x, hair.y, colors.hair, 1, colors_hair);
	choose_picker(major.x, major.y, colors.major, 1, colors_major);
	choose_picker(minor.x, minor.y, colors.minor, 1, colors_minor);
}