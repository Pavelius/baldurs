#pragma once

const short unsigned Blocked = 0xFFFF;
enum move_directions {
	Center,
	Left, Up, Right, Down,
	LeftUp, RightUp, RightDown, LeftDown
};
namespace map {
struct node {
	short unsigned		index;
	node*				next;
};
node*					addnode();
void					blockimpassable(short unsigned free_state = Blocked - 1);
void					createwave(short unsigned start, int size);
void					createwave(short unsigned start, int size, short unsigned max_cost);
unsigned short			getcost(unsigned short index);
int						getfree(unsigned short index, int radius, int size);
int						getrange(unsigned short i0, unsigned short i1);
inline unsigned short	getindex(int x, int y) { return y * 256 + x; }
int						getnodecount();
inline int				getx(short unsigned index) { return index % 256; }
inline int				gety(short unsigned index) { return index / 256; }
extern unsigned char	height; // current map height
extern bool				isblock(unsigned short index);
extern bool				isblock(unsigned short index, int size);
bool					ispassable(unsigned short i0, unsigned short i1, int size);
int						reachable(short unsigned start, short unsigned(*proc)(short unsigned index), short unsigned minimal_reach);
node*					removeall(node* p);
node*					removeback(node* p);
node*					remove(node* p);
node*					route(short unsigned start, short unsigned(*proc)(short unsigned index), short unsigned maximum_range = 0, short unsigned minimal_reach = 0);
extern void				set(short unsigned index, bool isblock);
void					set(short unsigned index, bool isblock, int size);
void					setcost(short unsigned index, short unsigned value);
short unsigned			stepfrom(short unsigned index);
short unsigned			stepto(short unsigned index);
move_directions			to(move_directions d, move_directions d1);
unsigned short			to(unsigned short index, move_directions d);
extern unsigned char	width; // current map width
}