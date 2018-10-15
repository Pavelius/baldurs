#include "main.h"

using namespace draw;

static unsigned char colors_hair[] = {79, 110, 5, 3, 0};
static unsigned char colors_skin[] = {107, 8, 114, 9, 10, 85, 84, 12, 16, 15, 17, 108, 106, 113, 14, 13, 105, 83};
static unsigned char colors_major[] = {41, 39, 38, 47, 54, 66, 51, 40, 100, 49, 58, 60, 22, 46, 63, 94, 107, 50, 62, 37, 65};
static unsigned char colors_minor[] = {65, 27, 49, 60, 66, 26, 41, 37, 58, 14, 46, 98, 53, 48, 24, 56, 40, 54, 103, 25, 102, 91, 47, 63, 51, 45, 38, 39};

struct cmcl : runable {
	constexpr cmcl() : index(), elements() {}
	constexpr cmcl(unsigned char& index, aref<unsigned char> elements) : index(&index), elements(elements) {}
	void				execute() const;
	void				choose() const;
	static cmcl			current;
	static void			pick_color();
private:
	unsigned char*		index;
	aref<unsigned char>	elements;
};

cmcl cmcl::current;

static void choose_color() {
	cmcl::current.choose();
}

void cmcl::pick_color() {
	if(current.index) {
		*current.index = hot.param;
		buttonok();
	}
}

void cmcl::execute() const {
	current = *this;
	draw::execute(choose_color);
}

void cmcl::choose() const {
	if(!elements)
		return;
	screenshoot push(true);
	while(ismodal()) {
		push.restore();
		image(300, 112, res::COLOR, 0);
		auto x = 320, y = 163, inc = 28;
		for(unsigned i = 0; i < elements.count; i++) {
			int px = i % 6;
			int py = i / 6;
			int id = elements.data[i + 1];
			picker(x + px * inc, y + py * inc, id, 0, cmpr(pick_color, id));
		}
		button(322, 332, cmpr(buttoncancel), 0, res::GBTNMED, "Отмена", KeyEscape);
		label(331, 134, 135, 22, "Выбирайте цвет");
		domodal();
	}
}

void draw::picker(int x, int y, unsigned char index, int type, const runable& cmd) {
	color temp[256];
	button_states state;
	unsigned char ns = 0;
	unsigned char ps = 1;
	if(type == 0) {
		ns = 2;
		ps = 3;
	}
	auto result = button(x, y, cmd, 0, res::INVBUT2, ns, ns, ps, ns, 0, 0, &state, false);
	if(state == ButtonPressed) {
		x += 2;
		y += 2;
	}
	sprite* pb = gres(res::COLGRAD);
	if(!pb)
		return;
	memcpy(temp, pb->offs(pb->get(type).pallette), sizeof(color) * 16);
	set(temp, index);
	draw::image(x, y, pb, type, 0, 0xFF, temp);
}

void actor::pickcolors(const point skin, const point hair, const point major, const point minor) {
	pickcolors(skin, hair, major, minor, colors);
}

void actor::pickcolors(const point skin, const point hair, const point major, const point minor, coloration& colors) {
	picker(skin.x, skin.y, colors.skin, 1, cmcl(colors.skin, colors_skin));
	picker(hair.x, hair.y, colors.hair, 1, cmcl(colors.hair, colors_hair));
	picker(major.x, major.y, colors.major, 1, cmcl(colors.major, colors_major));
	picker(minor.x, minor.y, colors.minor, 1, cmcl(colors.minor, colors_minor));
}