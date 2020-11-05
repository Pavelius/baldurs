#include "main.h"

using namespace draw;

static ccontrol* current_focus;

static void psetfocus() {
	((ccontrol*)hot.object)->setfocus(true);
}

ccontrol* ccontrol::getfocus() {
	return current_focus;
}

void ccontrol::input() {
	auto p = getfocus();
	if(p)
		p->translate();
}

bool ccontrol::isfocused() const {
	return current_focus == this;
}

void ccontrol::setfocus(bool instant) const {
	if(instant)
		current_focus = const_cast<ccontrol*>(this);
	else
		execute(psetfocus, 0, const_cast<ccontrol*>(this));
}