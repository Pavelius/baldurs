#include "main.h"

using namespace draw;

struct listbox : cbox {
	varianta&		elements;
	listbox(const rect& client, const rect& scroll, varianta& elements) : cbox(client, scroll), elements(elements) {
	}
	int getmaximum() const {
		return elements.getcount();
	}
	const char* getname(stringbuilder& sb, int n) const {
		return getstr(elements[n]);
	}
	variant get() const {
		return elements.data[current];
	}
};

void creature::help() {
	cursorset cur;
	varianta topics;
	listbox topicv({72, 70, 72 + 98, 70 + 276}, {0}, topics);
	for(auto& e : bsdata<varianti>()) {
		if(!e.manual[0])
			continue;
		auto id = (variant_s)(&e - bsdata<varianti>::elements);
		topics.add(id);
	}
	topics.sort();
	varianta elements;
	listbox elementv({193, 70, 193 + 200, 70 + 276}, {404, 70, 404 + 12, 70 + 291}, elements);
	char temp[8192]; stringbuilder sb(temp);
	auto list_height = texth() + 2;
	variant last_variant;
	ctext textv({434, 70, 706, 70 + 288}, {720, 70, 720 + 12, 70 + 291}, temp);
	while(ismodal()) {
		background(res::GUIHELP);
		label(300, 26, 200, 24, "Справка", 2);
		topicv.view();
		auto& ei = bsdata<varianti>::elements[topicv.get().value];
		elements.select(ei.manual[0], ei.manual[1]);
		elements.sort();
		elementv.view();
		sb.clear(); elementv.get().addinfo(sb);
		if(last_variant != elementv.get())
			textv.reset();
		textv.viewc();
		menumodal();
		ccontrol::input();
	}
}