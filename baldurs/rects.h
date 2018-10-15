#pragma once

struct rects {
	short		x1, y1;
	short		x2, y2;
	//
	explicit operator bool() const { return x1 || x2 || y1 || y2; }
	bool operator!=(const rects& rc) const { return rc.x1 != x1 || rc.x2 != x2 || rc.y1 != y1 || rc.y2 != y2; }
	bool operator==(const rects& rc) const { return rc.x1 == x1 && rc.x2 == x2 && rc.y1 == y1 && rc.y2 == y2; }
	void clear() { x1 = x2 = y1 = y2 = 0; }
	int	height() const { return y2 - y1; }
	bool intersect(const rects& rc) const { return x1<rc.x2 && x2>rc.x1 && y1<rc.y2 && y2>rc.y1; }
	void move(int x, int y) { x1 += x; x2 += x; y1 += y; y2 += y; }
	void offset(int x, int y) { x1 += x; x2 -= x; y1 += y; y2 -= y; }
	void offset(int n) { offset(n, n); }
	void set(int px1, int py1, int px2, int py2) { x1 = px1; x2 = px2; y1 = py1; y2 = py2; }
	int width() const { return x2 - x1; }
};