#include "main.h"

using namespace draw;

static unsigned		timestamp;
static int			current_command;
static void(*current_execute)();
static bool			keep_hot;
static hotinfo		keep_hot_value;
static res::tokens	cursor_sprite = res::CURSORS;
static int			cursor_frame = 0;
static int			cursor_single = false;
static bool			break_modal;
static int			break_result;

void draw::initialize() {
	// Initilaize all plugins
	pallette.read("data/core/MPALETTE.bmp", 0, 32);
	draw::create(-1, -1, 800, 600, 0, 32);
	draw::setcaption("������� ����");
	draw::font = metrics::font;
	colors::text = colors::white;
	colors::h1 = colors::text.mix(colors::yellow, 200);
	colors::h2 = colors::h1;
	colors::h3 = colors::h1;
	colors::special = colors::text.mix(colors::blue, 200);
	draw::fore = colors::text;
	draw::syscursor(false);
	draw::settimer(1000 / 30);
}

unsigned draw::getframe() {
	return timestamp;
}

unsigned draw::getframe(int fps) {
	return timestamp / (1000 / fps);
}

cursorset::cursorset(res::tokens rid, int frame, bool single) : rid(cursor_sprite), frame(cursor_frame), single(cursor_single) {
	set(rid, frame, single);
}

cursorset::~cursorset() {
	set(rid, frame, single);
}

void cursorset::set(res::tokens rid, int frame, bool single) {
	cursor_sprite = rid;
	cursor_frame = frame;
	cursor_single = single;
}

res::tokens	cursorset::getres() {
	return cursor_sprite;
}

void draw::execute(void(*proc)(), int param) {
	current_execute = proc;
	hot.param = param;
	hot.key = InputExecute;
}

void draw::execute(const hotinfo& value) {
	keep_hot = true;
	keep_hot_value = value;
}

int draw::getresult() {
	return break_result;
}

void draw::breakmodal(int result) {
	break_modal = true;
	break_result = result;
}

void draw::buttoncancel() {
	breakmodal(0);
}

void draw::buttonok() {
	breakmodal(1);
}

void update_timestamp() {
	auto previous = timestamp;
	timestamp = clock();
	for(auto& e : players)
		e.update();
	for(auto& e : creature_data)
		e.update();
}

static void before_render() {
	current_command = 0;
	current_execute = 0;
}

static void after_render() {
	auto press_frame = cursor_frame + 1;
	if(cursor_single)
		press_frame = cursor_frame;
	draw::image(hot.mouse.x, hot.mouse.y, gres(cursor_sprite), hot.pressed ? press_frame : cursor_frame, 0);
}

bool draw::ismodal() {
	before_render();
	if(!break_modal)
		return true;
	break_modal = false;
	return false;
}

void draw::domodal() {
	update_timestamp();
	if(current_execute) {
		auto proc = current_execute;
		before_render();
		proc();
		before_render();
		hot.key = InputUpdate;
		return;
	}
	if(hot.key == InputUpdate && keep_hot) {
		keep_hot = false;
		hot = keep_hot_value;
	} else {
		after_render();
		hot.key = draw::rawinput();
	}
	update_timestamp();
	if(!hot.key)
		exit(0);
}