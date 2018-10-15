#include "main.h"

point selectable::getposition() const {
	const rect& rc = getrect();
	return{(short)(rc.x1 + rc.width() / 2), (short)rc.y2};
}

void selectable::painting(point screen) const {
	auto polygon = getpoints();
	if(polygon.count < 3)
		return;
	color c = colors::green;
	for(unsigned i = 1; i < polygon.count; i++)
		draw::line(polygon.data[i - 1] - screen, polygon.data[i] - screen, c);
	draw::line(polygon.data[0] - screen, polygon.data[polygon.count - 1] - screen, c);
}

bool selectable::hittest(point hittest) const {
	rect rc = getrect();
	auto pt = getpoints();
	if(!pt.data)
		return false;
	if(draw::inside(hittest, pt.data, pt.count))
		return true;
	return false;
}