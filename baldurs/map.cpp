#include "crt.h"
#include "map.h"

using namespace map;

unsigned char			max_path_width;
unsigned char			max_path_height;
static unsigned short	path_route[256 * 256];
static unsigned short	path_stack[256 * 256];
static unsigned short	path_push;
static unsigned short	path_pop;
static unsigned short	path_goal;
static unsigned short	path_start;
static unsigned short	path_cost;
static int				path_size;
static node				path_nodes[1024 * 8];
unsigned char			map::width;
unsigned char			map::height;

static void snode(unsigned short index) {
	if(index == Blocked)
		return;
	auto a = path_route[index];
	if(a == Blocked)
		return;
	if(path_size > 1 && map::isblock(index, path_size)) {
		path_route[index] = Blocked;
		return;
	}
	int cost = path_cost + map::getcost(index);
	if(a && cost >= a)
		return;
	path_stack[path_push++] = index;
	path_route[index] = cost;
}

static void gnext(int index, unsigned short& level, int& pos) {
	if(index == -1)
		return;
	auto nlevel = path_route[index];
	if(nlevel == Blocked)
		return;
	if(!nlevel)
		return;
	if(nlevel <= level) {
		level = nlevel;
		pos = index;
	}
}

static void block_impassable_route() {
	int iy2 = max_path_height * 256;
	for(int iy = 0; iy < iy2; iy += 256) {
		int ix = iy + max_path_width;
		for(int i = iy; i < ix; i++)
			path_route[i] = map::isblock(i) ? Blocked : 0;
	}
}

node* map::addnode() {
	for(auto& e : path_nodes) {
		if(!e.index) {
			e.next = 0;
			return &e;
		}
	}
	path_nodes[0].index = 0;
	path_nodes[0].next = 0;
	return path_nodes;
}

int map::getnodecount() {
	int result = 0;
	for(auto& e : path_nodes) {
		if(e.index)
			result++;
	}
	return result;
}

unsigned short map::getpathcost(unsigned short index) {
	return path_route[index];
}

move_directions map::to(move_directions d, move_directions d1) {
	switch(d) {
	case Up:
		return d1;
	case Left:
		switch(d1) {
		case Left: return Down;
		case Up: return Left;
		case Right: return Up;
		case Down: return Right;
		}
		break;
	case Right:
		switch(d1) {
		case Left: return Up;
		case Up: return Right;
		case Right: return Down;
		case Down: return Left;
		}
		break;
	case Down:
		switch(d1) {
		case Left: return Right;
		case Up: return Down;
		case Right: return Left;
		case Down: return Up;
		}
		break;
	}
	return Center;
}

short unsigned map::to(short unsigned index, move_directions d) {
	switch(d) {
	case Left:
		if((index & 0xFF) == 0)
			return Blocked;
		return index - 1;
	case LeftUp:
		if((index & 0xFF) == 0)
			return Blocked;
		if((index >> 8) == 0)
			return Blocked;
		return index - 1 - 256;
	case Up:
		if((index >> 8) == 0)
			return Blocked;
		return index - 256;
	case RightUp:
		if((index & 0xFF) >= max_path_width - 1)
			return Blocked;
		if((index >> 8) == 0)
			return Blocked;
		return index - 256 + 1;
	case Right:
		if((index & 0xFF) >= max_path_width - 1)
			return Blocked;
		return index + 1;
	case RightDown:
		if((index & 0xFF) >= max_path_width - 1)
			return Blocked;
		if((index >> 8) >= max_path_height - 1)
			return Blocked;
		return index + 1;
	case Down:
		if((index >> 8) >= max_path_height - 1)
			return Blocked;
		return index + 256;
	case LeftDown:
		if((index & 0xFF) == 0)
			return Blocked;
		if((index >> 8) >= max_path_height - 1)
			return Blocked;
		return index - 1 + 256;
	default:
		return Blocked;
	}
}

bool map::ispassable(unsigned short i0, unsigned short i1, int size) {
	int x0 = getx(i0); int y0 = gety(i0);
	int x1 = getx(i1); int y1 = gety(i1);
	int dx = iabs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -iabs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;
	for(;;) {
		e2 = 2 * err;
		if(e2 >= dy) {
			if(x0 == x1)
				break;
			err += dy;
			if(e2 <= dx) {
				if(isblock(getindex(x0, y0 + sy), size))
					return false;
			}
			x0 += sx;
			if(isblock(getindex(x0, y0), size))
				return false;
		}
		if(e2 <= dx) {
			if(y0 == y1)
				break;
			err += dx;
			if(2 * err >= dy) {
				if(isblock(getindex(x0 + sx, y0), size))
					return false;
			}
			y0 += sy;
			if(isblock(getindex(x0, y0), size))
				return false;
		}
	}
	return true;
}

node* map::removeall(node* p) {
	while(p) {
		p->index = 0;
		p = p->next;
	}
	return 0;
}

node* map::remove(node* p) {
	auto p1 = p->next;
	p->index = 0;
	return p1;
}

int map::getrange(unsigned short i0, unsigned short i1) {
	int x0 = getx(i0); int y0 = gety(i0);
	int x1 = getx(i1); int y1 = gety(i1);
	return imax(iabs(x0 - x1), iabs(y0 - y1));
}

node* map::createpath(short unsigned start, int size) {
	auto s = path_stack + path_push - 2;
	auto p = addnode();
	auto result = p;
	auto dest = start;
	while(s >= path_stack) {
		if(ispassable(start, *s, size))
			dest = *s;
		else {
			p->index = dest;
			p->next = addnode();
			p = p->next;
			start = dest;
			dest = *s;
		}
		s--;
	}
	p->index = dest;
	return result;
}

// First, make wave and see what cell on map is passable
void map::createwave(short unsigned start, int size) {
	block_impassable_route();
	if(start == -1)
		return;
	path_push = 0;
	path_pop = 0;
	path_stack[path_push++] = start;
	path_route[start] = 1;
	path_size = size;
	while(path_push != path_pop) {
		auto pos = path_stack[path_pop++];
		auto cost = path_route[pos];
		if(cost >= 0xFF00)
			break;
		path_cost = cost;
		snode(to(pos, Left));
		snode(to(pos, Right));
		snode(to(pos, Up));
		snode(to(pos, Down));
		//path_cost += 1;
		//snode(moveto(pos, LeftUp));
		//snode(moveto(pos, LeftDown));
		//snode(moveto(pos, RightUp));
		//snode(moveto(pos, RightDown));
	}
	path_pop = 0;
	path_push = 0;
	path_goal = -1;
	path_start = start;
}

// Calculate path step by step to any cell on map analizing create_wave result.
// Go form goal to start and get lowest weight.
// When function return 'path_stack' has step by step path and 'path_push' is top of this path.
bool map::route(short unsigned start, short unsigned goal) {
	path_push = 0;
	path_goal = -1;
	int pos = goal;
	unsigned short level = Blocked;
	path_stack[path_push++] = goal;
	while(pos != start) {
		auto n = pos;
		gnext(to(pos, Left), level, n);
		gnext(to(pos, Right), level, n);
		gnext(to(pos, Up), level, n);
		gnext(to(pos, Down), level, n);
		//gnext(moveto(pos, LeftDown), level, n);
		//gnext(moveto(pos, LeftUp), level, n);
		//gnext(moveto(pos, RightDown), level, n);
		//gnext(moveto(pos, RightUp), level, n);
		if(pos == n)
			return false;
		pos = n;
		path_stack[path_push++] = n;
		level = path_route[pos];
		if(path_push > 65000)
			return false;
	}
	path_goal = goal;
	return true;
}

static bool get_free_space_x(short unsigned& index, int radius, int size) {
	short unsigned px = index & 0xFF;
	short unsigned py = index >> 8;
	int minx = imax(px - radius, 0);
	int maxx = imin(px + radius + 1, (int)max_path_width);
	for(short unsigned scanx = minx; scanx < maxx; scanx++) {
		if(py >= radius) {
			auto i = map::getindex(scanx, py - radius);
			if(!map::isblock(i)) {
				index = i;
				return true;
			}
		}
		if(py + radius < max_path_height) {
			int i = map::getindex(scanx, py + radius);
			if(!map::isblock(i)) {
				index = i;
				return true;
			}
		}
	}
	return false;
}

static bool get_free_space_y(short unsigned& index, int radius, int size) {
	int px = index & 0xFF;
	int py = index >> 8;
	int miny = imax(py - radius, 0);
	int maxy = imin(py + radius + 1, (int)max_path_height);
	for(int scany = miny; scany < maxy; scany++) {
		if(px >= radius) {
			int i = map::getindex(px - radius, scany * 256);
			if(!map::isblock(i, size)) {
				index = i;
				return true;
			}
		}
		if(px + radius < max_path_width) {
			int i = map::getindex(px + radius, scany * 256);
			if(!map::isblock(i, size)) {
				index = i;
				return true;
			}
		}
	}
	return false;
}

bool map::isblock(unsigned short index, int size) {
	if(size <= 1)
		return isblock(index);
	auto xc = getx(index);
	auto yc = gety(index);
	if(xc + size >= map::width || xc - size < 0)
		return true;
	if(yc + size >= map::height || yc - size < 0)
		return true;
	auto y2 = yc + size;
	auto x2 = xc + size;
	for(auto y = yc - size; y < y2; y++) {
		for(auto x = xc - size; x < x2; x++) {
			if(isblock(getindex(x, y)))
				return true;
		}
	}
	return false;
}

int map::getfree(short unsigned index, int radius, int size) {
	int maxr = max_path_width / 2;
	if(maxr > max_path_height)
		maxr = max_path_height;
	for(; radius < maxr; radius++) {
		if(rand() & 1) {
			if(get_free_space_x(index, radius, size))
				break;
			if(get_free_space_y(index, radius, size))
				break;
		} else {
			if(get_free_space_y(index, radius, size))
				break;
			if(get_free_space_x(index, radius, size))
				break;
		}
	}
	return index;
}

void map::set(short unsigned index, bool isblock, int size) {
	if(size == 1) {
		set(index, size);
		return;
	}
	auto xc = getx(index);
	auto yc = gety(index);
	if(xc + size >= map::width || xc - size < 0)
		return;
	if(yc + size >= map::height || yc - size < 0)
		return;
	auto y2 = yc + size;
	auto x2 = xc + size;
	for(auto y = yc - size; y < y2; y++) {
		for(auto x = xc - size; x < x2; x++)
			set(getindex(x, y), isblock);
	}
}