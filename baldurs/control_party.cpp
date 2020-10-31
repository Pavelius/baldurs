#include "main.h"

using namespace draw;

static void choose_character() {
	auto player_index = hot.param;
	auto player = game.party[player_index];
	if(player) {
		int r = dlgcho("Что сделать с персонажем?", "Изменить", "Удалить");
		switch(r) {
		case 1: player->generate("Генерация персонажа"); break;
		case 2: player->clear(); game.party[player_index] = 0; break;
		}
	} else {
		auto pp = game.party.add();
		if(pp) {
			*pp = bsdata<creature>::add();
			(*pp)->generate("Генерация персонажа");
		}
	}
}

void creature::create_party() {
	while(ismodal()) {
		background(res::GUICARBB, 0);
		label(279, 22, 242, 32, "Создание партии", 2);
		auto all_done = false;
		auto x = 428;
		auto y = 80;
		for(int i = 0; i < 6; i++) {
			auto p = game.party[i];
			if(p) {
				button(x, y + 1, choose_character, i, 0, res::GBTNBFRM, p->getname());
				draw::image(647, y, res::PORTS, p->getportrait());
				all_done = true;
			} else {
				button(x, y + 1, choose_character, i, 0, res::GBTNBFRM, "Создать персонажа");
				draw::image(647, y, res::PORTS, 0);
			}
			y += 69;
		}
		button(105, 495, buttoncancel, 0, 0, res::GBTNSTD, "Выход", KeyEscape);
		button(322, 495, buttoncancel, 0, all_done ? 0 : Disabled, res::GBTNMED, "Все очистить");
		button(576, 495, buttonok, 1, all_done ? 0 : Disabled, res::GBTNSTD, "Готово");
		domodal();
	}
}