#include "main.h"

using namespace draw;

static int standart_pixels_per_line;

static void porigin() {
	auto p = (cbox*)hot.object;
	p->setorigin(hot.param);
}

static void pselect() {
	auto p = (cbox*)hot.object;
	p->setcurrent(hot.param);
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
		origin = m - lpp - 1;
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
	return client.height() / ppl;
}

void cscroll::view() const {
	if(draw::areb(client) || draw::areb(scroll)) {
		switch(hot.key) {
		case MouseWheelDown: execute(porigin, origin + getscrollstep(), this); break;
		case MouseWheelUp: execute(porigin, origin - getscrollstep(), this); break;
		}
	}
	auto lpp = getlinesperpage();
	auto valid_maximum = getmaximum() - lpp  - 1;
	sprite* pb = gres(bar);
	if(pb && scroll) {
		viewscroll({scroll.x1, scroll.y1, scroll.x2, scroll.y1 + scroll.width()}, pb, 0, origin - getscrollstep());
		viewscroll({scroll.x1, scroll.y2 - scroll.width(), scroll.x2, scroll.y2}, pb, 2, origin + getscrollstep());
		int h = scroll.height() - pb->get(0).sy - pb->get(2).sy - pb->get(scroll_frame).sy - 2;
		int h1 = valid_maximum;
		if(h1 > 0) {
			int my = origin*h / h1;
			draw::image(scroll.x1, scroll.y1 + pb->get(0).sy + my, pb, scroll_frame, 0);
		}
		//rectb(scroll, colors::red);
	}
	//rectb(client, colors::green);
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
		setcurrent(getmaximum()-1);
		ensurevisible(current);
		break;
	case KeyPageDown:
		n = getlinesperpage();
		if((getorigin() + n) != current)
			setcurrent(getorigin() + n);
		else
			setcurrent(current + n);
		ensurevisible(current);
		break;
	case KeyPageUp:
		n = getlinesperpage();
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
	if(index >= origin && index <= origin + lpp)
		return;
	if(index < origin)
		setorigin(index);
	else
		setorigin(index - lpp);
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