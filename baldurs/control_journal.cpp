#include "main.h"

using namespace draw;

void creature::journal() {
	while(ismodal()) {
		int x = 0, y = 0;
		background(res::GUIJRLN);
		label(234, 24, 205, 28, "Журнал", 2); // STONEBIG
		label(66, 90, 651, 275, "255, 255, 246");
		rectb({727, 64, 739, 368}, colors::white);
		button(460, 18, cmpr(buttonparam, 4), 0, res::GBTNJBTN, 0, 1, 2, 3, 0, 0, 0);
		button(525, 18, cmpr(buttonparam, 5), 0, res::GBTNJBTN, 4, 5, 6, 7, 0, 0, 0);
		//rectb({66, 67, 170, 20}, {0, 200, 200});
		menumodal();
	}
}