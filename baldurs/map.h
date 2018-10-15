#pragma once

enum black_states {
	Blocked = 0xFFFF
};
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
node*					createpath(short unsigned start, int size);
void					createwave(short unsigned start, int size);
extern unsigned short	getcost(unsigned short index);
int						getfree(unsigned short index, int radius, int size);
unsigned short			getpathcost(unsigned short index);
int						getrange(unsigned short i0, unsigned short i1);
inline unsigned short	getindex(int x, int y) { return y * 256 + x; }
int						getnodecount();
inline int				getx(short unsigned index) { return index % 8; }
inline int				gety(short unsigned index) { return index / 256; }
extern unsigned char	height; // current map height
extern bool				isblock(unsigned short index);
extern bool				isblock(unsigned short index, int size);
bool					ispassable(unsigned short i0, unsigned short i1, int size);
node*					removeall(node* p);
node*					remove(node* p);
bool					route(short unsigned start, short unsigned goal);
extern void				set(short unsigned index, bool isblock);
void					set(short unsigned index, bool isblock, int size);
move_directions			to(move_directions d, move_directions d1);
unsigned short			to(unsigned short index, move_directions d);
extern unsigned char	width; // current map width
}