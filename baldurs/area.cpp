#include "main.h"

enum map_states {
	StateExplored = 0x80
};

static unsigned char	max_height;
static sprite*			sprites;
static sprite*			sprites_minimap;
static unsigned char	heightmap[256 * 256];
static unsigned char	statemap[256 * 256];
static unsigned char	lightmap[256 * 256];
static unsigned short	tilemap[64 * 64];
static color			lightpal[256];
BSDATAC(point, 256*256)

static const unsigned char orientations_5b5[25] = {
	6, 7, 8, 9, 10,
	5, 6, 8, 10, 11,
	4, 4, 0, 12, 12,
	3, 2, 0, 14, 13,
	2, 1, 0, 15, 14
};
static const unsigned char orientations_7b7[49] = {
	6, 7, 7, 8, 9, 9, 10,
	5, 6, 7, 8, 9, 10, 11,
	5, 5, 6, 8, 10, 11, 11,
	4, 4, 4, 0, 12, 12, 12,
	3, 3, 2, 0, 14, 13, 13,
	3, 2, 1, 0, 15, 14, 13,
	2, 1, 1, 0, 15, 15, 14,
};

int map::getday(unsigned value) {
	return value / (24 * 60);
}

int map::gethour(unsigned value) {
	return (value / 60) % 24;
}

void map::getpassedtime(stringbuilder& sb, unsigned value) {
	auto h = gethour(value);
	auto d = getday(value);
	if(d)
		sb.add("%1i %2", d, (d == 1) ? "day" : "days");
	if(h)
		sb.add("%1i %2", h, (h == 1) ? "hour" : "hours");
}

unsigned char* map::ptr(const char* name) {
	if(strcmp(name, "LM") == 0)
		return lightmap;
	else if(strcmp(name, "SR") == 0)
		return statemap;
	else if(strcmp(name, "HM") == 0)
		return heightmap;
	return 0;
}

color* map::getpallette() {
	return lightpal;
}

void map::clear() {
	if(sprites) {
		delete sprites;
		sprites = 0;
	}
	if(sprites_minimap) {
		delete sprites_minimap;
		sprites_minimap = 0;
	}
	initialize();
	height = width = max_height = 0;
	memset(tilemap, 0, sizeof(tilemap));
	memset(heightmap, 0, sizeof(heightmap));
	memset(statemap, 0, sizeof(statemap));
	memset(lightmap, 0, sizeof(lightmap));
	memset(lightpal, 0, sizeof(lightpal));
	bsdata<animation>::source.clear();
	bsdata<container>::source.clear();
	bsdata<creature>::source.clear();
	bsdata<door>::source.clear();
	bsdata<door_tile>::source.clear();
	bsdata<entrance>::source.clear();
	bsdata<itemground>::source.clear();
	bsdata<itemcont>::source.clear();
	bsdata<region>::source.clear();
	bsdata<point>::source.clear();
	bsdata<floattext>::source.clear();
}

void map::settile(short unsigned index, short unsigned tile) {
	tilemap[index] = tile;
}

unsigned char map::getorientation(point s, point d) {
	const int osize = 7;
	int dx = d.x - s.x;
	int dy = d.y - s.y;
	int st = (2 * imax(iabs(dx), iabs(dy)) + osize - 1) / osize;
	if(!st)
		return 0;
	int ax = dx / st;
	int ay = dy / st;
	return orientations_7b7[(ay + (osize / 2))*osize + ax + (osize / 2)];
}

unsigned short map::getindex(point pos) {
	return (pos.y / 12) * 256 + pos.x / 16;
}

unsigned short map::getindex(point pos, int size) {
	return (pos.y / 12) * 256 + pos.x / 16;
}

point map::getposition(short unsigned index, int size) {
	return{(short)((index & 0xFF) * 16 + 8), (short)((((unsigned)index) >> 8) * 12 + 6)};
}

color map::getshadow(point s) {
	return lightpal[lightmap[getindex(s, 1)]];
}

unsigned char map::getstate(short unsigned index) {
	return statemap[index];
}

static const char* gmurl(char* temp, const char* name, const char* ext = "ard") {
	zcpy(temp, "data/area/");
	zcat(temp, name);
	if(ext) {
		zcat(temp, ".");
		zcat(temp, ext);
	}
	return temp;
}

static void archive_bitmap(archive& e, unsigned char* output, int output_bpp, int scan_line, int width, int height, color* pal) {
	for(int i = 0; i < height; i++) {
		e.set(output, width*(output_bpp / 8));
		output += scan_line;
	}
	if(pal)
		e.set(pal, sizeof(color) * 256);
}

static bool load_tls_file(const char* name) {
	char temp[260];
	if(sprites)
		delete sprites;
	sprites = (sprite*)loadb(gmurl(temp, name, "pma"));
	return sprites != 0;
}

static bool load_mmp_file(const char* name) {
	char temp[260];
	if(sprites_minimap)
		delete sprites_minimap;
	gmurl(temp, name, 0);
	zcat(temp, "MM.pma");
	sprites_minimap = (sprite*)loadb(temp);
	return sprites != 0;
}

bool archive_ard(io::stream& file, bool writemode, char* sprites_resname) {
	array* references[] = {bsdata<point>::source_ptr, bsdata<door_tile>::source_ptr};
	archive ar(file, writemode, references);
	if(!ar.signature("ARD"))
		return false;
	if(!ar.version(1, 0))
		return false;
	// ���������
	ar.set(map::width);
	ar.set(map::height); max_height = (map::height * 12 + 15) / 16;
	ar.set(sprites_resname, 8);
	// ����� ������
	archive_bitmap(ar, (unsigned char*)tilemap, 16, 64 * sizeof(tilemap[0]), map::width / 4, max_height / 4, 0);
	archive_bitmap(ar, lightmap, 8, 256, map::width, map::height, lightpal);
	archive_bitmap(ar, statemap, 8, 256, map::width, map::height, 0);
	// �������
	ar.set<point>(bsdata<point>::source);
	ar.set<door_tile>(bsdata<door_tile>::source);
	ar.set<region>(bsdata<region>::source);
	ar.set<container>(bsdata<container>::source);
	ar.set<door>(bsdata<door>::source);
	ar.set<entrance>(bsdata<entrance>::source);
	ar.set<animation>(bsdata<animation>::source);
	return true;
}

static bool load_ard_file(const char* name, char* sprite_resname) {
	char temp[260];
	io::file file(gmurl(temp, name));
	if(!file)
		return false;
	map::clear();
	return archive_ard(file, false, sprite_resname);
}

bool map::load(const char* name) {
	char rsts[8];
	res::cleanup();
	if(!load_ard_file(name, rsts))
		return false;
	if(!load_tls_file(rsts))
		return false;
	if(!load_mmp_file(rsts))
		return false;
	worldmap::set(worldmap::getarea(name));
	return true;
}

bool map::isblock(short unsigned index) {
	//0 - Obstacle - impassable, light blocking (������)
	//1 - Sand ? (burgandy)
	//2 - Wood (�������)
	//3 - Wood (����������)
	//4 - Stone - echo-ey (�����-�����)
	//5 - Grass - soft (purple)
	//6 - Water - passable (turquoise)
	//7 - Stone - hard (������-�����)
	//8 - Obstacle - impassable, non light blocking (�����-�����)
	//9 - Wood(red)
	//10 - Wall - impassable (bright green)
	//11 - Water - passable (������)
	//12 - Water - impassable (�����)
	//13 - Roof - impassable (pink)
	//14 - Worldmap exit (������-�����)
	//15 - Grass (�����)
	unsigned char a = statemap[index] & 0x0F;
	return a == 0 || a == 8 || a == 10 || a == 12 || a == 13;
}

const sprite* map::getminimap() {
	return sprites_minimap;
}

const sprite* map::getareasprite() {
	return sprites;
}

int map::gettile(short unsigned index) {
	return tilemap[index];
}

point map::getfree(point position, int size) {
	int i = getindex(position, size);
	if(!isblock(i, size))
		return position;
	return map::getposition(getfree(i, 1, size), size);
}

void map::drop(short unsigned index, item it) {
	auto p = bsdata<itemground>::add();
	if(!p)
		return;
	*((item*)p) = it;
	p->index = index;
}