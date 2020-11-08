#include "main.h"

static char				textdata[256 * 256 * 4];
static ctext			scroll({12, 500, 542, 592}, {554, 497, 565, 592}, textdata);
static stringbuilder	sb(textdata);

static void update_log() {
	scroll.update();
	if(scroll.getorigin() + scroll.client.height() >= scroll.getorigin() - 4)
		scroll.setorigin(scroll.getmaximum());
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
	scroll.viewc();
}

void nameable::say(const char* format, ...) const {
	sb.addn("[%1]: ", getname());
	sb.addv(format, xva_start(format));
	update_log();
}

void nameable::act(const char* format, ...) {
	sb.addn("%1 ", getname());
	sb.addv(format, xva_start(format));
	sb.add(".");
	update_log();
}