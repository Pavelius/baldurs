#include "main.h"

const int maximum_strings = 32;

static const char* read_string(const char* p, char* ps, const char* pe) {
	while(p[0] && p[0] != '\n' && p[0] != '\r') {
		if(ps < pe)
			*ps++ = *p;
		p++;
	}
	ps[0] = 0;
	while(p[0] == '\n' || p[0] == '\r') {
		p = skipcr(p);
		p = skipsp(p);
	}
	return p;
}

static void apply_value(const char* id_value, const char** strings, int count) {
	for(auto& e : bsdata<varianti>()) {
		if(!e.source)
			continue;
		if(!e.locale[0] || !e.locale[0])
			continue;
		auto pf = e.source->find(id_value, 0);
		if(pf==-1)
			continue;
		auto p = e.source->ptr(pf);
		for(auto i = 0; i < count; i++) {
			if(!strings[i])
				continue;
			if(!e.locale[i])
				continue;
			auto pp = (const char**)((char*)p + e.locale[i]);
			*pp = szdup(strings[i]);
		}
	}
}

static bool read_locale(const char* url) {
	auto p_alloc = (const char*)loadt(url);
	if(!p_alloc)
		return false;
	auto p = p_alloc;
	auto requisits_count = 2;
	char name[128], value[8192];
	while(*p) {
		p = stringbuilder::readidn(p, name, name + sizeof(name) - 1);
		if(p[0] != ':')
			break;
		p = skipsp(p + 1);
		p = read_string(p, value, value + sizeof(value) - 1);
		const char* strings[maximum_strings] = {};
		auto count = 0;
		auto pt = value;
		strings[0] = pt;
		while(pt[0]) {
			if(pt[0] == '.' && requisits_count > 1) {
				// Через точку пишут поседний реквизит, сколко бы их не было.
				// Например, мы указали, что надо заполнит name, nameof, text.
				// На входе строка: Red|Red is bright color. Use it to hilite errors.
				// На выходе будет: name='Red', nameof=0, text='Red is bright color. Use it to hilite errors'
				pt[0] = 0;
				pt = (char*)skipsp(pt + 1);
				strings[requisits_count - 1] = pt;
				break;
			} else if(pt[0] == '|') {
				pt[0] = 0;
				pt = (char*)skipsp(pt + 1);
				if(count < maximum_strings)
					count++;
				strings[count] = pt;
				continue;
			}
			pt++;
		}
		apply_value(name, strings, 2);
	}
	delete p_alloc;
	return true;
}

bool gamei::readl() {
	read_locale("locale/items_ru.txt");
	auto& ei = bsdata<itemi>::elements[BattleAxe];
	if(!ei.text)
		return false;
	return true;
}