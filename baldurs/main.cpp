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

static creature* create(class_s type, race_s race, gender_s gender, reaction_s reaction = Helpful) {
	auto p = bsdata<creature>::add();
	p->create(type, race, gender, reaction);
	game.party.add(p);
	return p;
}

static void quick_load() {
	msdbg("Создание партии персонажей...");
	auto p1 = create(Fighter, Human, Male);
	create(Cleric, Dwarf, Male);
	create(Paladin, Human, Male);
	create(Ranger, HalfElf, Female);
	create(Rogue, Elf, Female);
	create(Wizard, Human, Male);
	for(auto p : game.party)
		p->random_equip(false);
	creature::moveto("AR4000");
	map::drop(map::getindex(21, 51), ChainMail);
	generate_treasure();
	p1->say("Ну что? Надерем пару задниц?");
	//creature::create(Goblin, Hostile, {1108, 2209}, 6, 4);
}

static void join_game() {}

static void setting() {}

static void exit_game() {}

static void start_scene() {
	auto fid = rand() % 2;
	while(ismodal()) {
		int x = 170, y = 92;
		background(res::START, fid);
		button(x + 396, y + 68, single_player, 0, 0, res::GBTNMED2, "Один игрок");
		button(x + 396, y + 156 + 0 * 32, new_game, 1, 0, res::GBTNMED2, "Новая игра");
		button(x + 396, y + 156 + 1 * 32, load_game, 2, 0, res::GBTNMED2, "Загрузить игру");
		button(x + 396, y + 156 + 2 * 32, quick_load, 3, 0, res::GBTNMED2, "Быстрая загрузка");
		button(x + 396, y + 156 + 3 * 32, join_game, 4, Disabled, res::GBTNMED2, "Присоединиться к игре");
		button(x + 396, y + 304 + 0 * 32, setting, 5, 0, res::GBTNMED2, "Настройки");
		button(x + 396, y + 304 + 1 * 32, exit_game, 6, 0, res::GBTNMED2, "Выход из игры", KeyEscape);
		domodal();
	}
}

extern int util_main();

void archive_pack(const char* arc_name, const char* folder);
void archive_unpack(const char* arc_name, const char* folder);

static bool test_variant_position() {
	auto s1 = sizeof(variant);
	point p1 = {1024, 3451};
	variant v1 = p1;
	if(v1.type != Position)
		return false;
	point p2 = v1.getposition();
	return p1 == p2;
}

static void test_generate() {
	creature player; player.clear();
	player.set(Human);
	player.set(Male);
	player.setportrait(12);
	player.set(Strenght, 15);
	player.set(Dexterity, 11);
	player.set(Constitution, 14);
	player.set(Intellegence, 8);
	player.set(Wisdow, 8);
	player.set(Charisma, 8);
	player.set(Fighter, 1);
	varianta elements;
	player.choose_skills("Тест выбора навыков", elements, true);
}

int main(int argc, char* argv[]) {
	//srand(clock());
	srand(1000002);
	gamei::localization("ru", false);
	gamei::localization("debug", true);
	if(!test_variant_position())
		return -1;
	draw::initialize();
	if(!draw::pallette)
		return -1;
	util_main();
	//test_character();
	//test_generate();
	setlayout(start_scene);
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}