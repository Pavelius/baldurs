#include "main.h"

using namespace draw;

static void nothing() {
}

void creature::options() {
	while(ismodal()) {
		background(res::STONEOPT);
		label(279, 23, 242, 30, "���������", 2);
		button(497, 68, nothing, 0, res::GBTNLRG2, "��������� ����");
		button(497, 98, nothing, 0, res::GBTNLRG2, "�������� ����");
		button(497, 128, nothing, 0, res::GBTNLRG2, "����� �� ����");
		button(497, 168, nothing, 0, res::GBTNLRG2, "�������");
		button(497, 198, nothing, 0, res::GBTNLRG2, "����");
		button(497, 228, nothing, 0, res::GBTNLRG2, "����");
		button(497, 298, nothing, 0, res::GBTNLRG2, "������");
		button(497, 268, nothing, 0, res::GBTNLRG2, "����������");
		button(555, 338, nothing, 0, res::GBTNSTD, "������");
		label(353, 386, 95, 16, "NORMAL");
		menumodal();
	}
}