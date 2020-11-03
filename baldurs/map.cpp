#include "crt.h"
#include "map.h"

using namespace map;

namespace {
struct nodei {
	indext				index;
	short unsigned		next;
	void				clear() { index = Blocked; next = Blocked; }
	constexpr explicit operator bool() const { return index != Blocked; }
};
}

static move_directions	all_aroud[] = {Left, Right, Up, Down, LeftUp, LeftDown, RightUp, RightDown};
static unsigned short	path_stack[256 * 256];
static unsigned short	path_cost[256 * 256];
static nodei			path_node[1024 * 8];
unsigned char			map::width;
unsigned char			map::height;

static nodei* addnode() {
	for(auto& e : path_node) {
		if(!e)
			return &e;
	}
	return 0;
}

static void addnode(short unsigned& ni, indext index) {
	auto p = addnode();
	if(!p)
		return;
	p->index = index;
	p->next = ni;
	ni = p - path_node;
}

void map::blockimpassable(short unsigned free_state) {
	for(auto y = 0; y < map::height; y++) {
		auto i2 = map::getindex(map::width, y);
		for(auto i = map::getindex(0, y); i < i2; i++)
			path_cost[i] = map::isblock(i) ? Blocked : free_state;
	}
}

unsigned short map::getcost(indext index) {
	return path_cost[index];
}

void map::setcost(indext index, short unsigned value) {
	path_cost[index] = value;
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

short unsigned map::to(indext index, move_directions d) {
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
		if((index & 0xFF) >= width - 1)
			return Blocked;
		if((index >> 8) == 0)
			return Blocked;
		return index - 256 + 1;
	case Right:
		if((index & 0xFF) >= width - 1)
			return Blocked;
		return index + 1;
	case RightDown:
		if((index & 0xFF) >= width - 1)
			return Blocked;
		if((index >> 8) >= height - 1)
			return Blocked;
		return index + 256 + 1;
	case Down:
		if((index >> 8) >= height - 1)
			return Blocked;
		return index + 256;
	case LeftDown:
		if((index & 0xFF) == 0)
			return Blocked;
		if((index >> 8) >= height - 1)
			return Blocked;
		return index - 1 + 256;
	default:
		return Blocked;
	}
}

bool map::ispassable(indext i0, indext i1, int size) {
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

short unsigned map::getnextpath(short unsigned node) {
	if(node == Blocked)
		return Blocked;
	return path_node[node].next;
}

indext map::getpathindex(short unsigned node) {
	if(node == Blocked)
		return Blocked;
	return path_node[node].index;
}

void map::removeall(short unsigned& start) {
	while(start != Blocked)
		remove(start);
}

void map::remove(short unsigned& start) {
	auto p = path_node + start;
	start = p->next;
	p->clear();
}

int map::getrange(indext i0, indext i1) {
	int x0 = getx(i0); int y0 = gety(i0);
	int x1 = getx(i1); int y1 = gety(i1);
	return imax(iabs(x0 - x1), iabs(y0 - y1));
}

// First, make wave and see what cell on map is passable
void map::createwave(indext start, int size) {
	short unsigned path_push = 0;
	short unsigned path_pop = 0;
	path_stack[path_push++] = start;
	path_cost[start] = 1;
	while(path_push != path_pop) {
		auto n = path_stack[path_pop++];
		auto w = path_cost[n] + 1;
		for(auto d : all_aroud) {
			auto i = to(n, d);
			if(path_cost[i] == Blocked)
				continue;
			if(!path_cost[i] || path_cost[i] > w) {
				path_cost[i] = w;
				path_stack[path_push++] = i;
			}
		}
	}
}

void map::createwave(indext start, int size, short unsigned max_cost) {
	short unsigned path_push = 0;
	short unsigned path_pop = 0;
	path_stack[path_push++] = start;
	path_cost[start] = 1;
	while(path_push != path_pop) {
		auto n = path_stack[path_pop++];
		auto w = path_cost[n] + 1;
		if(w > max_cost)
			continue;
		for(auto d : all_aroud) {
			auto i = to(n, d);
			if(path_cost[i] == Blocked)
				continue;
			if(!path_cost[i] || path_cost[i] > w) {
				path_cost[i] = w;
				path_stack[path_push++] = i;
			}
		}
	}
}

indext map::stepto(indext index) {
	auto current_index = Blocked;
	auto current_value = Blocked;
	for(auto d : all_aroud) {
		auto i = to(index, d);
		if(i == Blocked)
			continue;
		if(path_cost[i] < current_value) {
			current_value = path_cost[i];
			current_index = i;
		}
	}
	return current_index;
}

indext map::stepfrom(indext index) {
	auto current_index = Blocked;
	auto current_value = 0;
	for(auto d : all_aroud) {
		auto i = to(index, d);
		if(i >= Blocked-1)
			continue;
		if(path_cost[i] > current_value) {
			current_value = path_cost[i];
			current_index = i;
		}
	}
	return current_index;
}

// Calculate path step by step to any cell on map analizing create_wave result.
// Go form goal to start and get lowest weight.
// When function return 'path_stack' has step by step path and 'path_push' is top of this path.
void map::route(short unsigned& ni, indext goal, pget proc, short unsigned maximum_range) {
	addnode(ni, goal);
	for(auto n = proc(goal); n != Blocked && path_cost[n]; n = proc(n)) {
		auto c = getcost(n);
		if(maximum_range && c > maximum_range)
			continue;
		if(c <= 1)
			break;
		addnode(ni, n);
	}
}

// Calculate path step by step to any cell on map analizing create_wave result.
// Go from goal to start and get lowest weight.
// When function return 'path_stack' has step by step path and 'path_push' is top of this path.
int map::reachable(indext start, pget proc, short unsigned minimal_reach) {
	auto n = proc(start);
	auto w = 0;
	minimal_reach += 1; // Base cost is one
	for(; n != Blocked && path_cost[n] >= 1; n = proc(n)) {
		w += 1;
		if(minimal_reach >= path_cost[n])
			return w;
	}
	return w;
}

static bool get_free_space_x(indext& index, int radius, int size) {
	short unsigned px = index & 0xFF;
	short unsigned py = index >> 8;
	int minx = imax(px - radius, 0);
	int maxx = imin(px + radius + 1, (int)map::width);
	for(short unsigned scanx = minx; scanx < maxx; scanx++) {
		if(py >= radius) {
			auto i = map::getindex(scanx, py - radius);
			if(!map::isblock(i)) {
				index = i;
				return true;
			}
		}
		if(py + radius < map::height) {
			int i = map::getindex(scanx, py + radius);
			if(!map::isblock(i)) {
				index = i;
				return true;
			}
		}
	}
	return false;
}

static bool get_free_space_y(indext& index, int radius, int size) {
	int px = index & 0xFF;
	int py = index >> 8;
	int miny = imax(py - radius, 0);
	int maxy = imin(py + radius + 1, (int)map::height);
	for(int scany = miny; scany < maxy; scany++) {
		if(px >= radius) {
			int i = map::getindex(px - radius, scany * 256);
			if(!map::isblock(i, size)) {
				index = i;
				return true;
			}
		}
		if(px + radius < map::width) {
			int i = map::getindex(px + radius, scany * 256);
			if(!map::isblock(i, size)) {
				index = i;
				return true;
			}
		}
	}
	return false;
}

bool map::isblock(indext index, int size) {
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

bool map::islineofsight(indext start, indext goal) {
	return true;
}

indext map::getminimalcost(indext start, int maximum_range, bool need_line_of_sight) {
	if(start == Blocked)
		return Blocked;
	if(!maximum_range) {
		if(getcost(start) == Blocked)
			return Blocked;
		return start;
	}
	auto x1 = getx(start);
	auto y1 = gety(start);
	indext result = start;
	indext result_cost = getcost(start);
	if(result_cost == Blocked) {
		for(auto r = maximum_range; r > 0; r--) {
			auto x2 = x1 + r;
			for(auto x = x1 - r; x <= x2; x++) {
				if(x < 0 || x >= map::width)
					continue;
				auto y2 = y1 + r;
				for(auto y = y1 - r; y <= y2; y++) {
					if(y < 0 || y >= map::height)
						continue;
					auto i = getindex(x, y);
					auto c = getcost(i);
					if(c >= result_cost)
						continue;
					if(need_line_of_sight) {
						if(!islineofsight(i, start))
							continue;
					}
					result = i;
					result_cost = c;
				}
			}
		}
	}
	return result;
}

int map::getfree(indext index, int radius, int size) {
	int maxr = map::width / 2;
	if(maxr > map::height)
		maxr = map::height;
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

void map::initialize() {
	for(auto& e : path_node)
		e.clear();
}