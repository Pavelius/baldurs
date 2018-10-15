#include "main.h"

enum party_generation_s {
	NoPartyGeneration,
	ChooseCharacter,
};

using namespace draw;

static void choose_character() {
	auto hi = hot;
	hi.key = ChooseCharacter;
	execute(hi);
}

void creature::create_party() {
	while(ismodal()) {
		background(res::GUICARBB, 0);
		label(279, 22, 242, 32, "Создание партии", 2);
		auto all_done = false;
		auto x = 428;
		auto y = 80;
		for(int i = 0; i < 6; i++) {
			auto& e = players[i];
			if(e) {
				button(x, y + 1, cmpr(choose_character, (int)&e), 0, res::GBTNBFRM, e.getname());
				draw::image(647, y, res::PORTS, e.getportrait());
				all_done = true;
			} else {
				button(x, y + 1, cmpr(choose_character, 0), 0, res::GBTNBFRM, "Создать персонажа");
				draw::image(647, y, res::PORTS, 0);
			}
			y += 69;
		}
		button(105, 495, cmpr(buttoncancel), 0, res::GBTNSTD, "Выход", KeyEscape); //105, 495, 222, 520
		//button(322, 495, genparty_change, all_done ? 0 : Disabled, res::GBTNMED, "Заново"); //322, 495, 479, 519
		button(576, 495, cmpr(buttonok), all_done ? 0 : Disabled, res::GBTNSTD, "Готово"); //576, 495, 693, 520
		domodal();
		auto current = 0;
		switch(hot.key) {
		case ChooseCharacter:
			current = hot.param;
			if(players[current]) {
				int r = dlgcho("Что сделать с персонажем?", "Изменить", Disabled, "Удалить", 0);
				switch(r) {
				case 2:
					players[current].clear();
					break;
				}
			} else {
				players[current].clear();
				players[current].generate("Генерация персонажа");
			}
			break;
		}
	}
}