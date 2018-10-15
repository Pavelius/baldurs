#include "main.h"

int			players[6]; // Character's party
int			subjects[64]; // Players selected heroes for orders
static int	spellcasters[] = {Cleric, Druid, Paladin, Ranger, Wizard};

int game::getday(unsigned value) {
	return value / (24 * 60);
}

int game::gethour(unsigned value) {
	return (value / 60) % 24;
}

const char* game::getpassedtime(char* result, const char* result_maximum, unsigned value) {
	result[0] = 0;
	int h = gethour(value);
	int d = getday(value);
	if(d)
		szprints(szsep(result), result_maximum, "%1i %2", d, (d == 1) ? "day" : "days");
	if(h)
		szprints(szsep(result), result_maximum, "%1i %2", h, (h == 1) ? "hour" : "hours");
	return result;
}

void game::use(int player, int rec, int state) {
	if(rec >= FirstDoor && rec <= LastDoor) {
		if(state == -1) {
			if(isopen(rec))
				state = 0;
			else
				state = DoorOpen;
		}
		if(!state) {
			msact(player, "закрыл%а дверь");
			setopen(rec, false);
		} else if(islocked(rec))
			mslog(szt("Door is locked", "Дверь закрыта"));
		else {
			msact(player, szt("open door", "открыл%а дверь"));
			setopen(rec, true);
		}
	}
}

bool game::moveto(const char* origin_area, const char* origin_entrance) {
	if(!origin_area || !origin_area[0])
		return false;
	char area[8];
	char entrance[32];
	zcpy(area, origin_area);
	if(origin_entrance && origin_entrance[0])
		zcpy(entrance, origin_entrance);
	else
		memset(entrance, 0, sizeof(entrance));
	if(!load_map(area))
		return false;
	int rec = find(FirstEntrance, 0, Name, entrance);
	if(!rec)
		rec = FirstEntrance;
	unsigned char orient = bsget(rec, Orientation);
	point pt = point::create(bsget(rec, Position));
	set_pause(false);
	create_player_actors(pt, orient);
	set_camera(pt);
	msdbg("Party moved to area '%1' to entrance '%2'. Blocks: %3i",
		area, entrance[0] ? entrance : "<none>", rmblockcount());
	return true;
}