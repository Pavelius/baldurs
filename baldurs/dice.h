#pragma once

struct dice {
	unsigned char		c;
	unsigned char		d;
	char				b;
	char				m;
	constexpr operator bool() const { return c != 0; }
	void				clear() { c = d = b = m = 0; }
	constexpr int		maximal() const { return c * d + b; }
	int					roll() const;
	static int			roll(int c, int d);
};