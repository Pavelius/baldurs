#include "main.h"

using namespace draw;

static void nothing() {
}

void creature::options() {
	while(ismodal()) {
		background(res::STONEOPT);
		label(279, 23, 242, 30, "���������", 2);
		button(497, 68, nothing, 0, 0, res::GBTNLRG2, "��������� ����");
		button(497, 98, nothing, 1, 0, res::GBTNLRG2, "�������� ����");
		button(497, 128, nothing, 2, 0, res::GBTNLRG2, "����� �� ����");
		button(497, 168, nothing, 3, 0, res::GBTNLRG2, "�������");
		button(497, 198, nothing, 4, 0, res::GBTNLRG2, "����");
		button(497, 228, nothing, 5, 0, res::GBTNLRG2, "����");
		button(497, 298, nothing, 6, 0, res::GBTNLRG2, "������");
		button(497, 268, nothing, 7, 0, res::GBTNLRG2, "����������");
		button(555, 338, nothing, 8, 0, res::GBTNSTD, "������");
		label(353, 386, 95, 16, "NORMAL");
		menumodal();
	}
}