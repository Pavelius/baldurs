#include "main.h"

using namespace draw;

static void(*next_proc)();

static void single_player() {
	dlgmsg("���� ��� �� �����������.");
}

static void new_game() {
	creature::create_party();
}

static void load_game() {}

static void quick_load() {
	players[0].create(Fighter, Human, Male);
	players[1].create(Cleric, Dwarf, Male);
	players[2].create(Paladin, Human, Male);
	players[3].create(Ranger, HalfElf, Female);
	players[4].create(Rogue, Elf, Female);
	players[5].create(Wizard, Human, Male);
	creature::moveto("AR1000");
}

static void join_game() {}

static void setting() {}

static void exit_game() {}

static void start_scene() {
	auto fid = rand() % 2;
	while(ismodal()) {
		background(res::START, fid);
		//rectf({10, 10, 110, 500}, colors::black);
		//blit(*draw::canvas, 10, 10, pallette.width*4, pallette.height, 0, pallette, 0, 0, pallette.width, pallette.height/4);
		int x = 170, y = 92;
		button(x + 396, y + 68, cmpr(single_player), 0, res::GBTNMED2, "���� �����");
		//
		button(x + 396, y + 156 + 0 * 32, cmpr(new_game), 0, res::GBTNMED2, "����� ����");
		button(x + 396, y + 156 + 1 * 32, cmpr(load_game), 0, res::GBTNMED2, "��������� ����");
		button(x + 396, y + 156 + 2 * 32, cmpr(quick_load), 0, res::GBTNMED2, "������� ��������");
		button(x + 396, y + 156 + 3 * 32, cmpr(join_game), Disabled, res::GBTNMED2, "�������������� � ����");
		//
		button(x + 396, y + 304 + 0 * 32, cmpr(setting), 0, res::GBTNMED2, "���������");
		button(x + 396, y + 304 + 1 * 32, cmpr(exit_game), 0, res::GBTNMED2, "����� �� ����", KeyEscape);
		domodal();
	}
}

void draw::setnext(void(*proc)()) {
	next_proc = proc;
	breakmodal(1);
}

bool draw::isnext(void(*proc)()) {
	return next_proc == proc;
}

extern int util_main();

static void test_character() {
	creature player; player.clear();
	player.set(Male);
	player.setportrait(2);
	player.set(Dwarf);
	player.set(Cleric);
	player.set(LawfulNeutral);
	player.set(Strenght, 13);
	player.set(Dexterity, 8);
	player.set(Constitution, 14);
	player.set(Intellegence, 10);
	player.set(Wisdow, 16);
	player.set(Charisma, 12);
	player.set(Endurance);
	player.set(Diplomacy, 4);
	player.set(KnowledgeHistory, 4);
	player.generate("��������� ���������");
}

int main(int argc, char* argv[]) {
	srand(clock());
#ifdef _DEBUG
	util_main();
#endif
	draw::initialize();
	if(!draw::pallette)
		return -1;
	next_proc = start_scene;
	while(next_proc)
		next_proc();
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}