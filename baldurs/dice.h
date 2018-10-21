#pragma once

struct dice {
	unsigned char		c;
	unsigned char		d;
	char				b;
	char				m;
	operator bool() const { return c != 0; }
	void				clear() { c = d = b = m = 0; }
	int					maximal() const { return c * d + b; }
	char*				print(char* result, const char* result_maximum) const;
	char*				range(char* result, const char* result_maximum) const;
	int					roll() const;
	static int			roll(int c, int d);
};