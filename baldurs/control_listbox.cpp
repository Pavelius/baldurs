#include "main.h"

using namespace draw;

static int standart_pixels_per_line;

static void porigin() {
	auto p = (cscroll*)hot.object;
	p->setorigin(hot.param);
}

void cscroll::initialize() {
	standart_pixels_per_line = texth() + 2;
}

cscroll::cscroll(const rect& client, const rect& scroll) :
	bar(res::GBTNSCRL), scroll_frame(4),
	client(client), scroll(scroll), origin(0) {
}

int cscroll::getpixelsperline() const {
	return standart_pixels_per_line;
}

void cscroll::viewscroll(rect rc, sprite* pb, int i, int value) const {
	bool pressed = draw::area(rc) == AreaHilitedPressed;
	draw::image(rc.x1, rc.y1, pb, i + (pressed ? 1 : 0), 0);
	if(pressed) {
		if(hot.key == MouseLeft && hot.pressed)
			execute(porigin, value, this);
	}
}

void cscroll::correct() {
	auto lpp = getlinesperpage();
	auto m = getmaximum();
	if(origin + lpp >= m)
		origin = m - lpp;
	if(origin < 0)
		origin = 0;
}

void cscroll::setorigin(int index) {
	origin = index;
	correct();
}

int cscroll::getlinesperpage() const {
	auto ppl = getpixelsperline();
	if(!ppl)
		return 0;
	return (client.height() + ppl - 1) / ppl;
}

void cscroll::view() const {
	if(draw::areb(client) || draw::areb(scroll)) {
		switch(hot.key) {
		case MouseWheelDown: execute(porigin, origin + getscrollstep(), this); break;
		case MouseWheelUp: execute(porigin, origin - getscrollstep(), this); break;
		}
	}
	auto lpp = getlinesperpage();
	auto h1 = getmaximum() - lpp;
	sprite* pb = gres(bar);
	if(pb && scroll) {
		viewscroll({scroll.x1, scroll.y1, scroll.x2, scroll.y1 + scroll.width()}, pb, 0, origin - getscrollstep());
		viewscroll({scroll.x1, scroll.y2 - scroll.width(), scroll.x2, scroll.y2}, pb, 2, origin + getscrollstep());
		int h = scroll.height() - scroll.width() * 2 - pb->get(scroll_frame).sy - 4;
		if(h1 > 0) {
			auto my = (origin * h) / h1;
			draw::image(scroll.x1, scroll.y1 + scroll.width() + my + 2, pb, scroll_frame, ImageNoOffset);
		}
		//rectb(scroll, colors::red);
	}
	//rectb(client, colors::green);
}

static void pselect() {
	auto p = (cbox*)hot.object;
	p->setcurrent(hot.param);
}

cbox::cbox(const rect& client, const rect& scroll) : cscroll(client, scroll), current(0) {
}

void cbox::setcurrent(int index) {
	setfocus(true);
	current = index;
	auto m = getmaximum();
	if(current >= m)
		current = m - 1;
	if(current < 0)
		current = 0;
}

void cbox::row(rect rc, int n) const {
	auto push_color = fore;
	unsigned flags = 0;
	if(current == n) {
		fore = fore.mix(colors::yellow);
		if(isfocused())
			flags = TextBold;
	}
	char temp[512]; stringbuilder sb(temp);
	text(rc.x1, rc.y1 + 1, getname(sb, n), -1, flags);
	if(areb(rc) && hot.pressed) {
		if(hot.key == MouseLeft)
			draw::execute(pselect, n, const_cast<cbox*>(this));
	}
	fore = push_color;
}

void cbox::translate() {
	int n;
	switch(hot.key) {
	case KeyUp:
		setcurrent(current - 1);
		ensurevisible(current);
		break;
	case KeyDown:
		setcurrent(current + 1);
		ensurevisible(current);
		break;
	case KeyHome:
		setcurrent(0);
		ensurevisible(current);
		break;
	case KeyEnd:
		setcurrent(getmaximum() - 1);
		ensurevisible(current);
		break;
	case KeyPageDown:
		n = getlinesperpage() - 1;
		if((getorigin() + n) != current)
			setcurrent(getorigin() + n);
		else
			setcurrent(current + n);
		ensurevisible(current);
		break;
	case KeyPageUp:
		n = getlinesperpage() - 1;
		if(getorigin() != current)
			setcurrent(getorigin());
		else
			setcurrent(current - n);
		ensurevisible(current);
		break;
	}
}

void cbox::ensurevisible(int index) {
	auto lpp = getlinesperpage();
	auto origin = getorigin();
	if(index >= origin && index <= origin + lpp - 1)
		return;
	if(index < origin)
		setorigin(index);
	else
		setorigin(index - lpp + 1);
}

void cbox::view() const {
	cscroll::view();
	auto m = getmaximum();
	auto w = client.width();
	auto i = origin;
	auto y1 = client.y1;
	auto ppl = getpixelsperline();
	for(auto i = origin; i < m; i++) {
		if(y1 >= client.y2)
			break;
		row({client.x1, y1, client.x1 + w, y1 + ppl}, i);
		y1 += ppl;
	}
}