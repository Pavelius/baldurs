#include "main.h"

using namespace draw;

void draw::dlgmsg(const char* text) {
	if(!text)
		return;
	screenshoot e(true);
	while(ismodal()) {
		e.restore();
		image(261, 173, res::GUIERR, 1);
		//rectb({286, 200, 512, 262}, colors::green);
		textf(286, 200, (512 - 286), text);
		button(346, 276, cmpr(buttonok), 0, res::GBTNSTD, "Продолжить", KeyEnter);
		domodal();
	}
}

bool draw::dlgask(const char* text) {
	screenshoot e(true);
	while(ismodal()) {
		e.restore();
		draw::image(258, 184, res::GUIERR, 2);
		label(286, 208, 221, 72, text);
		button(278, 289, cmpr(buttonok), 0, res::GBTNSTD, "OK", KeyEnter);
		button(398, 289, cmpr(buttoncancel), 0, res::GBTNSTD, "Отмена", KeyEscape);
		domodal();
	}
	return getresult();
}

static void choose_answer() { breakmodal(hot.param); }

int draw::dlgcho(const char* text, const char* a1, const char* a2) {
	screenshoot e(true);
	while(ismodal()) {
		e.restore();
		draw::image(262, 144, res::GMPMCHRB, 0);
		label(285, 168, 232, 20, text);
		button(285, 196, cmpr(choose_answer, 1), 0, res::GBTNLRG2, a1, Alpha + '1');
		button(285, 228, cmpr(choose_answer, 2), 0, res::GBTNLRG2, a2, Alpha + '2');
		button(344, 260, cmpr(buttoncancel), 0, res::GBTNSTD, "Отмена", KeyEscape);
		domodal();
	}
	return getresult();
}