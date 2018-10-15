#include "main.h"

using namespace draw;

const char* root = "data/pma";
const char* chra = "data/pma";

res::element res::elements[] = {{"NONE"}, {"CURSORS", root}, {"CURSARW", root}, {"COLGRAD", root}, {"COLOR", root},
{"GACTN", root},
{"GBTNBFRM", root}, {"GBTNLRG", root}, {"GBTNMED", root}, {"GBTNMED2", root}, {"GBTNMINS", root}, {"GBTNPLUS", root}, {"GBTNOPT1", root}, {"GBTNOPT2", root},
{"GBTNSPB1", root}, {"GBTNSPB2", root}, {"GBTNSPB3", root},
{"GBTNPOR", root}, {"GUIMAP", root}, {"GUIMAPWC", root}, {"GUIREC", root}, {"GBTNRECB", root}, {"GBTNSCRL", root}, {"GBTNSTD", root},
{"GCOMM", root}, {"GCOMMBTN", root},
{"INVBUT2", root}, {"INVBUT3", root}, {"ITEMS", root}, {"SCROLLS", root}, {"FORM", root},
{"CDMB1", chra}, {"CDMB2", chra}, {"CDMB3", chra}, {"CDMC4", chra}, {"CDMF4", chra}, {"CDMT1", chra}, {"CDMW1", chra}, {"CDMW2", chra}, {"CDMW3", chra}, {"CDMW4", chra},
{"CEFB1", chra}, {"CEFB2", chra}, {"CEFB3", chra}, {"CEFC4", chra}, {"CEFF4", chra}, {"CEFT1", chra}, {"CEFW1", chra}, {"CEFW2", chra}, {"CEFW3", chra}, {"CEFW4", chra},
{"CEMB1", chra}, {"CEMB2", chra}, {"CEMB3", chra}, {"CEMC4", chra}, {"CEMF4", chra}, {"CEMT1", chra}, {"CEMW1", chra}, {"CEMW2", chra}, {"CEMW3", chra}, {"CEMW4", chra},
{"CHFB1", chra}, {"CHFB2", chra}, {"CHFB3", chra}, {"CHFC4", chra}, {"CHFF4", chra}, {"CHFT1", chra}, {"CHFW1", chra}, {"CHFW2", chra}, {"CHFW3", chra}, {"CHFW4", chra}, {"CHFM1", chra},
{"CHMB1", chra}, {"CHMB2", chra}, {"CHMB3", chra}, {"CHMC4", chra}, {"CHMF4", chra}, {"CHMM1", chra}, {"CHMW1", chra}, {"CHMW2", chra}, {"CHMW3", chra}, {"CHMW4", chra}, {"CHMT2", chra},
{"CIFB1", chra}, {"CIFB2", chra}, {"CIFB3", chra}, {"CIFC4", chra}, {"CIFF4", chra}, {"CIFT1", chra},
{"CIMB1", chra}, {"CIMB2", chra}, {"CIMB3", chra}, {"CIMC4", chra}, {"CIMF4", chra}, {"CIMT1", chra},
{"GUIA", root}, {"GUIBTACT", root}, {"GUIBTBUT", root},
{"GUICGB", root}, {"GUICHP", root}, {"GUIHELP", root}, {"GUIFEAT", root}, {"GUIINV", root}, {"GUIHSB", root}, {"GUICARBB", root},
{"GUIPFC", root}, {"GUIRSPOR", root}, {"GUISEX", root}, {"GUISPL", root}, {"GUICNAME", root}, {"GUIERR", root},
{"GUIRLVL", root}, {"GMPMCHRB", root}, {"GCGPARTY", root}, {"GBTNLRG2", root},
{"GUIJRLN", root}, {"GOPT", root}, {"GUIMOVB", root}, {"STONEOPT", root}, {"GUISRSVB", root}, {"GUISRRQB", root},
{"CGEAR", root},
{"LOAD", root}, {"LOCATER", root},
{"PORTL", root}, {"PORTS", root},
{"SPELLS", root}, {"SPLBUT", root}, {"START", root}, {"STON", root}, {"STONSLOT", root}, {"WMAP", root},
{"WQSAX", chra}, {"WQNAX", chra}, {"WQMAX", chra}, {"WQLAX", chra},
{"WQSBW", chra}, {"WQNBW", chra}, {"WQMBW", chra}, {"WQLBW", chra},
{"WQSCB", chra}, {"WQNCB", chra}, {"WQMCB", chra}, {"WQLCB", chra},
{"WQSD1", chra}, {"WQND1", chra}, {"WQMD1", chra}, {"WQLD1", chra},
{"WQSD2", chra}, {"WQND2", chra}, {"WQMD2", chra}, {"WQLD2", chra},
{"WQSD3", chra}, {"WQND3", chra}, {"WQMD3", chra}, {"WQLD3", chra},
{"WQSD4", chra}, {"WQND4", chra}, {"WQMD4", chra}, {"WQLD4", chra},
{"WQSDD", chra}, {"WQNDD", chra}, {"WQMDD", chra}, {"WQLDD", chra},
{"WQSFL", chra}, {"WQNFL", chra}, {"WQMFL", chra}, {"WQLFL", chra},
{"WQSH0", chra}, {"WQNH0", chra}, {"WQMH0", chra}, {"WQLH0", chra},
{"WQSH1", chra}, {"WQNH1", chra}, {"WQMH1", chra}, {"WQLH1", chra},
{"WQSH2", chra}, {"WQNH2", chra}, {"WQMH2", chra}, {"WQLH2", chra},
{"WQSH3", chra}, {"WQNH3", chra}, {"WQMH3", chra}, {"WQLH3", chra},
{"WQSH4", chra}, {"WQNH4", chra}, {"WQMH4", chra}, {"WQLH4", chra},
{"WQSH5", chra}, {"WQNH5", chra}, {"WQMH5", chra}, {"WQLH5", chra},
{"WQSHB", chra}, {"WQNHB", chra}, {"WQMHB", chra}, {"WQLHB", chra},
{"WQSMC", chra}, {"WQNMC", chra}, {"WQMMC", chra}, {"WQLMC", chra},
{"WQSQS", chra}, {"WQNQS", chra}, {"WQMQS", chra}, {"WQLQS", chra},
{"WQSS1", chra}, {"WQNS1", chra}, {"WQMS1", chra}, {"WQLS1", chra},
{"WQSS2", chra}, {"WQNS2", chra}, {"WQMS2", chra}, {"WQLS2", chra},
{"WQSS3", chra}, {"WQNS3", chra}, {"WQMS3", chra}, {"WQLS3", chra},
{"WQSSC", chra}, {"WQNSC", chra}, {"WQMSC", chra}, {"WQLSC", chra},
{"WQSSL", chra}, {"WQNSL", chra}, {"WQMSL", chra}, {"WQLSL", chra},
{"WQSSP", chra}, {"WQNSP", chra}, {"WQMSP", chra}, {"WQLSP", chra},
{"WQSSS", chra}, {"WQNSS", chra}, {"WQMSS", chra}, {"WQLSS", chra},
{"WQSWH", chra}, {"WQNWH", chra}, {"WQMWH", chra}, {"WQLWH", chra},
{0}
};
static_assert((sizeof(res::elements) / sizeof(res::elements[0])) == (res::Count + 1), "Need resource update");

static unsigned		timestamp;
draw::surface		draw::pallette;
static int			current_command;
static void(*current_execute)();
static bool			keep_hot;
static hotinfo		keep_hot_value;
static res::tokens	cursor_sprite = res::CURSORS;
static int			cursor_frame = 0;

void draw::initialize() {
	// Initilaize all plugins
	for(auto p = plugin::first; p; p = p->next)
		p->initialize();
	pallette.read("data/core/MPALETTE.bmp", 0, 32);
	draw::create(-1, -1, 800, 600, 0, 32);
	draw::setcaption("Балдурс Гейт");
	draw::font = metrics::font;
	colors::text = colors::white;
	colors::h1 = colors::text.mix(colors::yellow, 200);
	colors::h2 = colors::h1;
	colors::h3 = colors::h1;
	colors::special = colors::text.mix(colors::blue, 200);
	draw::fore = colors::text;
	draw::syscursor(false);
	draw::settimer(1000/30);
}

bool draw::inside(point t, point* points, int count) {
	register int j, yflag0, yflag1, xflag0, index;
	bool inside_flag = false;
	const point* vtx0, *vtx1;

	if(count < 3)
		return false;

	index = 0;
	vtx0 = points + count - 1;
	yflag0 = (vtx0->y >= t.y);
	vtx1 = &points[index];

	for(j = count + 1; --j;) {
		yflag1 = (vtx1->y >= t.y);
		if(yflag0 != yflag1) {
			xflag0 = (vtx0->x >= t.x);
			if(xflag0 == (vtx1->x >= t.x)) {
				if(xflag0)
					inside_flag = !inside_flag;
			} else {
				if((vtx1->x -
					(vtx1->y - t.y) * (vtx0->x - vtx1->x) /
					(vtx0->y - vtx1->y)) >= t.x) {
					inside_flag = !inside_flag;
				}
			}
		}
		yflag0 = yflag1;
		vtx0 = vtx1;
		vtx1 = &points[++index];
	}
	return inside_flag;
}

void draw::set(color * dest, unsigned char index, int start, int count) {
	color* source = (color*)pallette.ptr(0, index);
	if(source)
		memcpy(dest + start, source, count * sizeof(color));
}

unsigned draw::getframe() {
	return timestamp;
}

unsigned draw::getframe(int fps) {
	return timestamp / (1000 / fps);
}

void draw::ellipse(const rect& rc, color c) {
	int w = rc.width();
	int h = rc.height();
	circle(rc.x1 + h/2, rc.y1 + w/2, imin(w, h)/2, c);
}

static void apply_header(int header, int col) {
	switch(header) {
	case 1: draw::font = metrics::h1; break;
	case 2: draw::font = metrics::h2; break;
	case 3: draw::font = metrics::h3; break;
	}
	switch(col) {
	case 0:
		break;
	case Disabled:
		draw::fore = colors::gray;
		break;
	default:
		draw::fore = *((color*)pallette.ptr(3, col));
		break;
	}
}

int labelb(int x, int y, int width, int height, const char * name, int header, int color) {
	draw::state push;
	apply_header(header, color);
	return draw::text({x, y, x + width, y + height}, name);
}

int draw::label(int x, int y, int width, int height, const char * name, int header, int color, bool border) {
	draw::state push;
	apply_header(header, color);
	if(border)
		rectb({x, y, x + width, y + height}, colors::red);
	return draw::text({x, y, x + width, y + height}, name, AlignCenterCenter);
}

int draw::labell(int x, int y, int width, int height, const char * name, int header, int color) {
	draw::state push;
	apply_header(header, color);
	return draw::textc(x, y + (height - draw::texth()) / 2, width, name);
}

int draw::labelr(int x, int y, int width, int height, const char * name, int header, int color) {
	draw::state push;
	apply_header(header, color);
	draw::text(x + width - draw::textw(name), y + (height - draw::texth()) / 2, name);
	return 0;
}

static void scroll_reaction() {}

void draw::view(rect rc, scrollbutton& e) {
	if(!e.increment)
		return;
	sprite* p = gres(e.bar);
	int numb = rc.width() / e.increment;
	for(int i = 0; i < numb; i++) {
		unsigned flags = 0;
		button(rc.x1 + i * e.increment, rc.y1, cmpr(scroll_reaction), flags, e.bar, 0);
	}
}

cursorset::cursorset(res::tokens r, int f) : r(cursor_sprite), f(cursor_frame) {
	set(r, f);
}

cursorset::~cursorset() {
	set(r, f);
}

void cursorset::set(res::tokens r, int f) {
	cursor_sprite = r;
	cursor_frame = f;
}

static struct input_plugin : draw::plugin {

	void before() override {
		current_command = 0;
		current_execute = 0;
	}

	void after() override {
		draw::image(hot.mouse.x, hot.mouse.y, gres(cursor_sprite),
			cursor_frame + (hot.pressed ? 1 : 0), 0);
	}

	bool translate(int id) override {
		switch(id) {
		case 0:
			exit(0);
			break;
		default:
			return false;
		}
		return true;
	}

} plugin_instance;

void draw::execute(void(*proc)(), int param) {
	current_execute = proc;
	hot.param = param;
	hot.key = InputExecute;
}

void draw::execute(const hotinfo& value) {
	keep_hot = true;
	keep_hot_value = value;
}

static void update_timestamp() {
	auto previous = timestamp;
	timestamp = clock();
}

void draw::domodal() {
	update_timestamp();
	if(current_execute) {
		auto proc = current_execute;
		for(auto p = plugin::first; p; p = p->next)
			p->before();
		proc();
		for(auto p = plugin::first; p; p = p->next)
			p->before();
		hot.key = InputUpdate;
		return;
	}
	if(hot.key == InputUpdate && keep_hot) {
		keep_hot = false;
		hot = keep_hot_value;
	} else {
		for(auto p = plugin::first; p; p = p->next)
			p->after();
		hot.key = draw::rawinput();
	}
	update_timestamp();
	for(auto p = plugin::first; p; p = p->next) {
		if(p->translate(hot.key))
			break;
	}
}