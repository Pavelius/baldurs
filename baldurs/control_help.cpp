#include "main.h"

using namespace draw;

struct listbox : scrolllist {
	variant*		elements;
	int				current;
	static listbox*	current_focus;
	listbox() : elements(0), current(0) {}
	listbox(const aref<variant>& source) {
		set(source);
	}
	static void select_item() {
		current_focus->current = hot.param;
	}
	variant get() const {
		return elements[current];
	}
	void set(const aref<variant>& source) {
		elements = source.data;
		maximum = source.count;
		qsort(elements, maximum, sizeof(elements[0]), compare_variant);
	}
	void prerender() override {
		if(current >= maximum)
			current = maximum - 1;
		if(current < 0)
			current = 0;
	}
	void row(rect rc, int n) override {
		auto push_color = fore;
		unsigned flags = 0;
		if(current == n) {
			rectf({rc.x1-2, rc.y1 + 1, rc.x2+2, rc.y2 + 2}, colors::edit, 0x80);
			fore = fore.mix(colors::yellow);
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
};

listbox* listbox::current_focus;

static variant		topics[] = {Skill, Spell, Feat, Alignment, Race, Class, Ability};
static listbox		topic_list(topics);
static scrolltext	description_control;

static aref<variant> select_data(const aref<variant>& result, const variant v1, const variant v2) {
	auto pb = result.data;
	auto pe = pb + result.count;
	for(auto e = v1; e.number <= v2.number; e.number++) {
		if(pb < pe)
			*pb++ = e;
	}
	auto rs = result;
	rs.count = pb - result.data;
	return rs;
}

void creature::help() {
	cursorset cur;
	variant elements[256];
	listbox elements_list;
	char temp[8192];
	stringcreator sc;
	stringbuilder sb(sc, temp);
	auto list_height = texth() + 2;
	variant last_variant;
	while(ismodal()) {
		background(res::GUIHELP);
		label(300, 26, 200, 24, "Справка", 2);
		view({72, 70, 72 + 98, 70 + 276}, {0}, list_height, topic_list);
		switch(topic_list.get().var) {
		case Feat: elements_list.set(select_data(elements, FirstFeat, LastFeat)); break;
		case Skill: elements_list.set(select_data(elements, FirstSkill, LastSkill)); break;
		case Spell: elements_list.set(select_data(elements, FirstSpell, LastSpell)); break;
		case Alignment: elements_list.set(select_data(elements, LawfulGood, ChaoticEvil)); break;
		case Race: elements_list.set(select_data(elements, FirstRace, LastRace)); break;
		case Class: elements_list.set(select_data(elements, FirstClass, LastClass)); break;
		case Ability: elements_list.set(select_data(elements, Strenght, Charisma)); break;
		default: break;
		}
		view({193, 70, 193 + 200, 70 + 276}, {404, 70, 404 + 12, 70 + 291}, list_height, elements_list);
		auto description = getdescription(elements_list.get());
		if(last_variant != elements_list.get())
			description_control.reset();
		view({434, 70, 706, 70 + 288}, {720, 70, 720 + 12, 70 + 291}, description, description_control);
		menumodal();
		switch(hot.key) {
		case KeyUp:
			if(listbox::current_focus)
				listbox::current_focus->current--;
			break;
		case KeyDown:
			if(listbox::current_focus)
				listbox::current_focus->current++;
			break;
		}
	}
}