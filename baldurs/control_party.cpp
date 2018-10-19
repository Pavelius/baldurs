#include "main.h"

using namespace draw;

static void choose_character() {
	auto& player = players[hot.param];
	if(player) {
		int r = dlgcho("��� ������� � ����������?", "��������", "�������");
		switch(r) {
		case 1: player.generate("��������� ���������"); break;
		case 2: player.clear(); break;
		}
	} else {
		player.clear();
		player.generate("��������� ���������");
	}
}

void creature::create_party() {
	while(ismodal()) {
		background(res::GUICARBB, 0);
		label(279, 22, 242, 32, "�������� ������", 2);
		auto all_done = false;
		auto x = 428;
		auto y = 80;
		for(int i = 0; i < 6; i++) {
			auto& e = players[i];
			if(e) {
				button(x, y + 1, cmpr(choose_character, i), 0, res::GBTNBFRM, e.getname());
				draw::image(647, y, res::PORTS, e.getportrait());
				all_done = true;
			} else {
				button(x, y + 1, cmpr(choose_character, i), 0, res::GBTNBFRM, "������� ���������");
				draw::image(647, y, res::PORTS, 0);
			}
			y += 69;
		}
		button(105, 495, cmpr(buttoncancel), 0, res::GBTNSTD, "�����", KeyEscape);
		button(322, 495, cmpr(buttoncancel), all_done ? 0 : Disabled, res::GBTNMED, "��� ��������");
		button(576, 495, cmpr(buttonok), all_done ? 0 : Disabled, res::GBTNSTD, "������");
		domodal();
	}
}