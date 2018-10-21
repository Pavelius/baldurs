#include "main.h"

using namespace draw;

draw::surface		draw::pallette;

static void apply_header(int header, int col) {
	switch(header) {
	case 1: draw::font = metrics::h1; break;
	case 2: draw::font = metrics::h2; break;
	case 3: draw::font = metrics::h3; break;
	}
	switch(col) {
	case 0:
		break;
	case Disabled:
		draw::fore = colors::gray;
		break;
	default:
		draw::fore = *((color*)pallette.ptr(3, col));
		break;
	}
}

//int labelb(int x, int y, int width, int height, const char * name, int header, int color) {
//	draw::state push;
//	apply_header(header, color);
//	return draw::text({x, y, x + width, y + height}, name);
//}

int draw::label(int x, int y, int width, int height, const char * name, int header, int color, bool border) {
	draw::state push;
	apply_header(header, color);
	if(border)
		rectb({x, y, x + width, y + height}, colors::red);
	return draw::text({x, y, x + width, y + height}, name, AlignCenterCenter);
}

int draw::labell(int x, int y, int width, int height, const char * name, int header, int color) {
	draw::state push;
	apply_header(header, color);
	return draw::textc(x, y + (height - draw::texth()) / 2, width, name);
}

int draw::labelr(int x, int y, int width, int height, const char * name, int header, int color) {
	draw::state push;
	apply_header(header, color);
	draw::text(x + width - draw::textw(name), y + (height - draw::texth()) / 2, name);
	return 0;
}