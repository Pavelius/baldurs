#include "main.h"

using namespace draw;

adat<itemground, 2048> itemground_data;

rect itemground::getrect() const {
	auto pt = getposition();
	return {pt.x - 48, pt.y - 32, pt.x + 48, pt.y + 32};
}

void itemground::painting(point screen) const {
	auto pt = getposition();
	image(pt.x - screen.x, pt.y - screen.y, gres(res::GROUND), gettype(), 0);
}

int itemground::getzorder() const {
	return -128;
}

point itemground::getposition() const {
	return map::getposition(index, 1);
}

bool itemground::hittest(point position) const {
	auto pt = getposition();
	rect rc = {pt.x - 8, pt.y - 6, pt.x + 8, pt.y + 6};
	return position.in(rc);
}