#include "main.h"

static char				textdata[256 * 256 * 4];
static stringbuilder	sb(textdata);
static ctext			scroll;
static rect				last_rect;

static void update_log() {
	scroll.reset();
	if(scroll.origin + last_rect.height() >= scroll.maximum - 4)
		scroll.origin = scroll.maximum;
}

void msclear() {
	sb.clear();
	update_log();
}

void draw::mslog(const char* format, ...) {
	sb.addsep('\n');
	sb.addv(format, xva_start(format));
	update_log();
}

void draw::mspaint(const rect& rc, const rect& rcs) {
	last_rect = rc;
	view(rc, rcs, textdata, scroll);
}

void actor::say(const char* format, ...) const {
	sb.addn("[%1]: ", getname());
	sb.addv(format, xva_start(format));
	update_log();
}

void actor::act(int player, const char* format, ...) {
	sb.addn("%1 ", getname());
	sb.addv(format, xva_start(format));
	sb.add(".");
	update_log();
}