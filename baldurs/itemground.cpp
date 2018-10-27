#include "main.h"

using namespace draw;

adat<itemground, 2048> itemground_data;

rect itemground::getrect() const {
	auto pt = getposition();
	return {pt.x - 48, pt.y - 32, pt.x + 48, pt.y + 32};
}

void itemground::painting(point screen) const {
	color pallette[256];
	auto pt = getposition();
	auto sp = gres(res::GROUND);
	if(!sp)
		return;
	auto fr = sp->get(gettype());
	auto pl = (color*)sp->offs(fr.pallette);
	if(!pl)
		return;
	memcpy(pallette, pl, sizeof(pallette));
	point hotspot = hot.mouse + screen;
	if(!hittest(hotspot))
		pallette[1] = colors::gray;
	image(pt.x - screen.x, pt.y - screen.y, gres(res::GROUND), gettype(), ImagePallette,0xFF, pallette);
}

static int get_item_height(item_s type) {
	switch(type) {
	case Helm:
		return 0;
	case Shortsword:
	case Longsword:
	case Staff:
		return -64;
	default:
		return -128;
	}
}

int itemground::getzorder() const {
	return get_item_height(gettype());
}

point itemground::getposition() const {
	return map::getposition(index, 1);
}

bool itemground::hittest(point position) const {
	auto pt = getposition();
	rect rc = {pt.x - 8, pt.y - 6, pt.x + 8, pt.y + 6};
	return position.in(rc);
}