#include "main.h"

point selectable::getposition() const {
	const rect& rc = getrect();
	return{(short)(rc.x1 + rc.width() / 2), (short)rc.y2};
}

void selectable::painting(point screen) const {
	auto polygon = getpoints();
	auto count = polygon.getcount();
	if(count < 3)
		return;
	color c = colors::green;
	for(unsigned i = 1; i < count; i++)
		draw::line(polygon[i - 1] - screen, polygon[i] - screen, c);
	draw::line(polygon[0] - screen, polygon[count - 1] - screen, c);
}

bool selectable::hittest(point hittest) const {
	rect rc = getrect();
	auto pt = getpoints();
	if(!pt)
		return false;
	if(draw::inside(hittest, pt.begin(), pt.getcount()))
		return true;
	return false;
}