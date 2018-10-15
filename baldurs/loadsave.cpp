#include "main.h"

using namespace draw;

const int portx = 19;
const int porty = 22;
const int mapx = 103;
const int mapy = 77;

struct gamefile {
	char		name[32];
	unsigned	gametime; // How many minuts in game terms has been played
	int			realtime;
	point		camera;
	color		screenshot[mapx*mapy];
	color		portraits[6][portx*porty];

	void setscreenshoot() {
		rect rc = {0, 0, 800, 600};
		draw::surface bm;
		bm.resize(mapx, mapy, 32, false);
		bm.bits = (unsigned char*)screenshot;
		this->camera = ::camera;
		draw::location(rc, camera);
		draw::blit(bm, 0, 0, mapx, mapy, 0, *draw::canvas, 0, 0, 800, 600);
		bm.bits = 0;
	}

	void setportrait(int n) {
		if(!players[n])
			return;
		draw::surface br;
		br.resize(portx, porty, 32, false);
		br.bits = (unsigned char*)portraits[n];
		if(true) {
			draw::state push;
			draw::surface bm(42, 42, 32);
			draw::canvas = &bm;
			draw::setclip();
			draw::image(0, 0, res::PORTS, bsget(players[n], Portrait));
			draw::blit(br, 0, 0, br.width, br.height, 0, bm, 0, 0, bm.width, bm.height);
		}
		br.bits = 0;
	}

	void setname(const char* name) {
		zcpy(this->name, name);
	}

	char* getdescription(char* result) {
		if(!gametime)
			return "";
		zcpy(result, "Пролог, ");
		game::gettimepassed(zend(result), gametime);
		return result;
	}

	void remove() {
		char temp[260];
		io::file::remove(filename(temp));
	}

	char* filename(char* result) {
		return szurl(result, "save", name, "sav");
	}

};

void save_game_file(const char* url, gamefile& gm) {
	gm.gametime = game_minute;
	gm.realtime = getdate();
	gm.setscreenshoot();
	for(int i = 0; i < 6; i++)
		gm.setportrait(i);
	io::file file(url, StreamWrite);
	if(!file)
		return;
	file.write(&gm, sizeof(gm));
}

void save_game_file(const char* name) {
	char temp[260];
	gamefile gm;
	int test_size = sizeof(gm);
	memset(&gm, 0, sizeof(gm));
	gm.setname(name);
	save_game_file(gm.filename(temp), gm);
}

bool load_game_file(const char* url, gamefile& gm) {
	memset(&gm, 0, sizeof(gm));
	io::file file(url, StreamRead);
	if(!file)
		return false;
	file.read(&gm, sizeof(gm));
	return true;
}

struct scrollgamefile : public scrolllist {
	int				mode;
	aref<gamefile>	files;

	scrollgamefile(int mode) : mode(mode) {
		files.data = 0;
		files.count = 0;
		update();
	}

	~scrollgamefile() {
		clear();
	}

	void update() {
		clear();
		for(io::file::find ff("save"); ff; ff.next()) {
			if(!szpmatch(ff.name(), "*.sav"))
				continue;
			char temp[260];
			files.reserve();
			if(!load_game_file(ff.fullname(temp), files.data[maximum]))
				continue;
			maximum++;
		}
		if(mode == Save) {
			files.reserve();
			memset(&files.data[maximum], 0, sizeof(files.data[maximum]));
			maximum++;
		}
	}

	void clear() {
		files.clear();
		maximum = 0;
	}

	bool isvalid(void* n, int size) {
		for(int i = 0; i < size; i++) {
			if(((char*)n)[i] != 0)
				return true;
		}
		return false;
	}

	void draw_portrait(gamefile& e, int x, int y, int n) {
		if(isvalid(e.portraits[n], sizeof(e.portraits[n])))
			putbits(x, y, portx, porty, e.portraits[n]);
	}

	void row(rect rc, int n) override {
		char temp[128];
		int x = rc.x1;
		int y = rc.y1;
		gamefile& e = files[origin + n];
		labell(x + 129, y + 9, 345, 18, (e.name[0]) ? e.name : "Пусто");
		labell(x + 129, y + 32, 279, 18, e.getdescription(temp), 0, ColorGhostlyGreishSilver);
		labell(x + 129, y + 51, 279, 18, "Time", 0, ColorGhostlyGreishSilver);
		if(isvalid(e.screenshot, sizeof(e.screenshot)))
			putbits(x, y, mapx, mapy, e.screenshot);
		draw_portrait(e, x + 508 + 28 * 0, y + 7, 0);
		draw_portrait(e, x + 508 + 28 * 1, y + 7, 1);
		draw_portrait(e, x + 508 + 28 * 2, y + 7, 2);
		draw_portrait(e, x + 508 + 28 * 0, y + 7 + 41, 3);
		draw_portrait(e, x + 508 + 28 * 1, y + 7 + 41, 4);
		draw_portrait(e, x + 508 + 28 * 2, y + 7 + 41, 5);
		button(x + 597, y + 5, LoadSaveGame, 0, res::GBTNSTD, (mode == Save) ? szt("Save", "Записать") : szt("Load", "Загрузить"), 0, origin + n);
		button(x + 597, y + 46, DeleteGame, (e.name[0] ? 0 : Disabled), res::GBTNSTD, szt("Delete", "Удалить"), 0, origin + n); //627, 130, 744, 155
	}

};

bool show::save(const char* title, const char* promt, const char* action, char* result, int maximum) {
	draw::screenshoot back;
	while(true) {
		back.restore();
		draw::image(240, 194, gres(res::GUISRRQB), 0, 0);
		label(261, 218, 280, 20, title);
		edit({265, 251, 540, 267}, 0, 0, result, maximum);
		label(261, 279, 280, 20, promt);
		button(259, 309, Cancel, 0, res::GBTNSPB1, bsgets(Cancel, Name)); //259, 309, 361, 333
		button(387, 309, OK, result[0] ? 0 : Disabled, res::GBTNMED, action); //387, 309, 543, 333
		int id = draw::input();
		switch(id) {
		case 0:
		case Cancel:
		case KeyEscape:
			return false;
		case OK:
		case KeyEnter:
			return true;
		}
	}
}

int show::savegame() {
	cursorset		cur;
	scrollgamefile	games(Save);
	while(true) {
		background(res::GUISRSVB);
		label(243, 22, 311, 28, szt("Save game", "Сохранение игры"), 2);
		view({30, 84, 760, 576}, {763, 78, 775, 576}, 102, games);
		button(656, 22, Cancel, 0, res::GBTNSTD, bsgets(Cancel, Name)); //656, 22, 773, 47
		int id = draw::input();
		switch(id) {
		case 0:
			return 0;
		case KeyEscape:
		case Cancel:
			return Next;
		case DeleteGame:
			if(dlgask("Удаление", "Вы действительно хотите удалить этот файл?")) {
				games.files[hot.param].remove();
				games.update();
			}
			break;
		case LoadSaveGame:
			if(games.mode == Save) {
				char temp[260];
				gamefile& e = games.files[hot.param];
				if(!save(
					"Введите имя игры для записи",
					"Пролог, 7 часов",
					"Записать",
					e.name, sizeof(e.name)))
					break;
				save_game_file(e.filename(temp), e);
				return Next;
			} else {
			}
			break;
		}
	}
}