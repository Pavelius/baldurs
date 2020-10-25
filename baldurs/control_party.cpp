#include "main.h"

using namespace draw;

static void choose_character() {
	auto& player = players[hot.param];
	if(player) {
		int r = dlgcho("Что сделать с персонажем?", "Изменить", "Удалить");
		switch(r) {
		case 1: player.generate("Генерация персонажа"); break;
		case 2: player.clear(); break;
		}
	} else {
		player.clear();
		player.generate("Генерация персонажа");
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
			auto& e = players[i];
			if(e) {
				if(button(x, y + 1, 0, res::GBTNBFRM, e.getname()))
					execute(choose_character, i);
				draw::image(647, y, res::PORTS, e.getportrait());
				all_done = true;
			} else {
				if(!button(x, y + 1, 0, res::GBTNBFRM, "Создать персонажа"))
					execute(choose_character, i);
				draw::image(647, y, res::PORTS, 0);
			}
			y += 69;
		}
		button(105, 495, buttoncancel, 0, res::GBTNSTD, "Выход", KeyEscape);
		button(322, 495, buttoncancel, all_done ? 0 : Disabled, res::GBTNMED, "Все очистить");
		button(576, 495, buttonok, all_done ? 0 : Disabled, res::GBTNSTD, "Готово");
		domodal();
	}
}