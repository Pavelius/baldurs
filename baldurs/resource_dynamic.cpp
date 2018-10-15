#include "collection.h"
#include "crt.h"
#include "draw.h"
#include "io.h"
#include "resources.h"

using namespace draw;

static arem<res::element> source;

static res::element* find(const char* name, const char* folder) {
	for(auto& e : source) {
		if(strcmp(e.name, name)==0 && strcmp(e.folder, folder)==0)
			return &e;
	}
	return 0;
}

const sprite* draw::gres(const char* name, const char* folder) {
	auto p = find(name, folder);
	if(!p) {
		p = source.add();
		p->name = szdup(name);
		p->folder = szdup(folder);
		p->data = 0;
		p->notfound = false;
	}
	if(!p || p->notfound)
		return 0;
	if(!p->data) {
		char temp[260];
		p->data = (sprite*)loadb(szurl(temp, p->folder, p->name, "pma"));
	}
	if(!p->data)
		p->notfound = true;
	return p->data;
}