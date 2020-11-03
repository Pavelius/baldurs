#pragma once

const short unsigned Blocked = 0xFFFF;
enum move_directions {
	Center,
	Left, Up, Right, Down,
	LeftUp, RightUp, RightDown, LeftDown
};
namespace map {
typedef short unsigned	indext;
typedef indext(*pget)(indext);
void					blockimpassable(indext free_state = 0);
void					createwave(indext start, int size);
void					createwave(indext start, int size, short unsigned max_cost);
unsigned short			getcost(indext index);
int						getfree(indext index, int radius, int size);
int						getrange(indext i0, indext i1);
inline unsigned short	getindex(int x, int y) { return y * 256 + x; }
indext					getminimalcost(indext start, int maximum_range = 5, bool need_line_of_sight = false);
short unsigned			getnextpath(short unsigned node);
indext					getpathindex(short unsigned node);
inline int				getx(indext index) { return index % 256; }
inline int				gety(indext index) { return index / 256; }
extern unsigned char	height; // current map height
void					initialize();
extern bool				isblock(indext index);
extern bool				isblock(indext index, int size);
bool					islineofsight(indext i0, indext i1);
bool					ispassable(indext i0, indext i1, int size);
int						reachable(indext start, pget proc, short unsigned minimal_reach);
void					removeall(short unsigned& ni);
void					remove(short unsigned& ni);
void					route(short unsigned& result, indext start, pget proc, short unsigned minimum_range, short unsigned maximum_range);
extern void				set(indext index, bool isblock);
void					set(indext index, bool isblock, int size);
void					setcost(indext index, short unsigned value);
short unsigned			stepfrom(indext index);
short unsigned			stepto(indext index);
move_directions			to(move_directions d, move_directions d1);
unsigned short			to(indext index, move_directions d);
extern unsigned char	width; // current map width
}