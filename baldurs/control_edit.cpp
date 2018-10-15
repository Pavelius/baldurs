#include "main.h"

using namespace draw;

int draw::field(rect rc, char* result, unsigned maximum) {
	char temp[8];
	auto n = zlen(result);
	switch(hot.key) {
	case KeyBackspace:
		if(n)
			result[n - 1] = 0;
		break;
	case InputSymbol:
	case InputSymbol | Shift:
		if(hot.param < 0x20)
			break;
		if(true) {
			szput(temp, hot.param);
			unsigned m0 = zlen(result);
			unsigned m1 = zlen(temp);
			if((m0 + m1 + 1) < maximum)
				zcat(result, temp);
		}
		break;
	}
	draw::text(rc.x1, rc.y1, result);
	auto w = draw::textw(result);
	auto m = getframe(6);
	if(m & 1) {
		point pt = {(short)(rc.x1 + w), (short)rc.y1};
		draw::line(pt, {pt.x, (short)(pt.y + draw::texth())}, colors::text);
	}
	return w;
}