#include "main.h"

using namespace draw;

static void single_player() {
	dlgmsg("Пока еще не реализовано.");
}

static void new_game() {
	creature::create_party();
}

static void load_game() {}

static void generate_treasure() {
	treasure e;
	e.generate(1);
	e.place();
}

static void quick_load() {
	msdbg("Создание партии персонажей...");
	players[0].create(Fighter, Human, Male, Helpful);
	players[1].create(Cleric, Dwarf, Male, Helpful);
	players[2].create(Paladin, Human, Male, Helpful);
	players[3].create(Ranger, HalfElf, Female, Helpful);
	players[4].create(Rogue, Elf, Female, Helpful);
	players[5].create(Wizard, Human, Male, Helpful);
	creature::moveto("AR4000");
	map::drop(map::getindex(21, 51), ChainMail);
	generate_treasure();
	creature::create(Goblin, Hostile, {1376, 2800}, 6, 4);
}

static void join_game() {}

static void setting() {}

static void exit_game() {}

static void start_scene() {
	auto fid = rand() % 2;
	while(ismodal()) {
		int x = 170, y = 92;
		background(res::START, fid);
		button(x + 396, y + 68, cmpr(single_player), 0, res::GBTNMED2, "Один игрок");
		//
		button(x + 396, y + 156 + 0 * 32, cmpr(new_game), 0, res::GBTNMED2, "Новая игра");
		button(x + 396, y + 156 + 1 * 32, cmpr(load_game), 0, res::GBTNMED2, "Загрузить игру");
		button(x + 396, y + 156 + 2 * 32, cmpr(quick_load), 0, res::GBTNMED2, "Быстрая загрузка");
		button(x + 396, y + 156 + 3 * 32, cmpr(join_game), Disabled, res::GBTNMED2, "Присоединиться к игре");
		//
		button(x + 396, y + 304 + 0 * 32, cmpr(setting), 0, res::GBTNMED2, "Настройки");
		button(x + 396, y + 304 + 1 * 32, cmpr(exit_game), 0, res::GBTNMED2, "Выход из игры", KeyEscape);
		domodal();
	}
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
	player.update_portrait();
	player.generate("Генерация персонажа");
}

void archive_pack(const char* arc_name, const char* folder);
void archive_unpack(const char* arc_name, const char* folder);

int main(int argc, char* argv[]) {
	srand(clock());
#ifdef _DEBUG
	io::file::makedir("temp");
	archive_pack("test.sav", "D:/projects/xface/test/art/pictures");
	archive_unpack("test.sav", "temp");
	util_main();
#endif
	draw::initialize();
	if(!draw::pallette)
		return -1;
	//test_character();
	setlayout(start_scene);
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}