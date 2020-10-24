#include "main.h"

static char			textdata[256 * 256 * 4];
static int			count;
static scrolltext	scroll;
static rect			last_rect;

void msclear() {
	textdata[0] = 0;
	count = 0;
}

void draw::mslogv(const char* temp) {
	int n = zlen(temp);
	if(count + n > int(sizeof(textdata)) - 8)
		msclear();
	if(count) {
		zcpy(textdata + count, "\n");
		count++;
	}
	zcpy(textdata + count, temp);
	count += n;
	scroll.reset();
	if(scroll.origin + last_rect.height() >= scroll.maximum - 4)
		scroll.origin = scroll.maximum;
}

void draw::mslog(const char* format, ...) {
	char temp[4096]; stringbuilder sb(temp);
	sb.addv(format, xva_start(format));
	mslogv(temp);
}

void draw::mspaint(const rect& rc, const rect& rcs) {
	last_rect = rc;
	view(rc, rcs, textdata, scroll);
}