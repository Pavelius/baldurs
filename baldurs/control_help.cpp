#include "main.h"

using namespace draw;

struct listbox : clist {
	varianta&		elements;
	int				current;
	static listbox*	current_focus;
	listbox(varianta& elements) : elements(elements), current(0) {}
	static void select_item() {
		current_focus->current = hot.param;
	}
	variant get() const {
		return elements.data[current];
	}
	int getmaximum() const override {
		return elements.getcount();
	}
	void prerender() override {
		if(current >= getmaximum())
			current = getmaximum() - 1;
		if(current < 0)
			current = 0;
	}
	void row(rect rc, int n) override {
		auto push_color = fore;
		unsigned flags = 0;
		if(current == n) {
			//rectf({rc.x1-2, rc.y1 + 1, rc.x2+2, rc.y2 + 2}, colors::edit, 0x80);
			fore = fore.mix(colors::yellow);
			flags = TextBold;
		}
		text(rc.x1, rc.y1 + 1, getstr(elements[n]), -1, flags);
		switch(hot.key) {
		case MouseLeft:
			if(hot.pressed && areb(rc)) {
				current_focus = this;
				draw::execute(select_item, n);
			}
			break;
		}
		fore = push_color;
	}
	void ensurevisible(int index) {
	}
	void translate() {
		switch(hot.key) {
		case KeyUp:
			current--;
			ensurevisible(current);
			break;
		case KeyDown:
			current++;
			ensurevisible(current);
			break;
		}
	}
};

listbox*			listbox::current_focus;
static ctext		description_control;

void creature::help() {
	cursorset cur;
	varianta topics; listbox topicv(topics);
	for(auto& e : bsdata<varianti>()) {
		if(!e.manual[0])
			continue;
		auto id = (variant_s)(&e - bsdata<varianti>::elements);
		topics.add(id);
	}
	topics.sort();
	varianta elements; listbox elementv(elements);
	char temp[8192]; stringbuilder sb(temp);
	auto list_height = texth() + 2;
	variant last_variant;
	while(ismodal()) {
		background(res::GUIHELP);
		label(300, 26, 200, 24, "Справка", 2);
		view({72, 70, 72 + 98, 70 + 276}, {0}, list_height, topicv);
		auto& ei = bsdata<varianti>::elements[topicv.get().value];
		elements.select(ei.manual[0], ei.manual[1]);
		elements.sort();
		view({193, 70, 193 + 200, 70 + 276}, {404, 70, 404 + 12, 70 + 291}, list_height, elementv);
		sb.clear(); elementv.get().addinfo(sb);
		if(last_variant != elementv.get())
			description_control.reset();
		view({434, 70, 706, 70 + 288}, {720, 70, 720 + 12, 70 + 291}, temp, description_control);
		menumodal();
		if(listbox::current_focus)
			listbox::current_focus->translate();
	}
}