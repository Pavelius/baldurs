#include "draw.h"

using namespace draw;

struct layouti {
	int					id;
	layouti*			previous;
	fnevent				proc;
	fnevent				def_proc;
	static layouti*	current;
	layouti(void(*proc)()) : proc(proc), def_proc(proc) {
		previous = current;
		current = this;
	}
	~layouti() {
		current = previous;
	}
};
layouti* layouti::current;

fnevent draw::getlayout() {
	return layouti::current ? layouti::current->def_proc : 0;
}

void draw::setlayout(fnevent proc) {
	layouti e(proc);
	while(e.proc)
		e.proc();
}

void draw::setpage(fnevent proc) {
	if(!layouti::current)
		return;
	layouti::current->proc = proc;
	breakmodal(1);
}

void draw::setpagedef(fnevent proc) {
	if(!layouti::current)
		return;
	layouti::current->def_proc = proc;
}

void draw::setpage() {
	if(!layouti::current)
		return;
	layouti::current->proc = layouti::current->def_proc;
	breakmodal(1);
}

bool draw::isnext(fnevent proc) {
	if(!layouti::current)
		return false;
	return layouti::current->proc == proc;
}