#include "main.h"

gamei game;

void gamei::checklocalization() {
#ifdef _DEBUG
	for(auto& e : bsdata<varianti>()) {
		if(!e.source || !e.locale[0])
			continue;
		for(unsigned i = 0; i < e.source->getcount(); i++) {
			auto p = (char*)e.source->ptr(i);
			auto id = *((const char**)(p));
			auto p1 = *((const char**)(p + e.locale[0]));
			if(!p1 || !p1[0])
				draw::mslog("Empthy name for [%1] type in element [-%2]", e.id, id);
		}
	}
#endif
}

void gamei::localization(const char* locale_id, bool writemode) {
	const auto fields_count = sizeof(varianti::locale) / sizeof(varianti::locale[0]);
	for(auto& e : bsdata<varianti>()) {
		if(!e.source || !e.locale[0])
			continue;
		char temp[260]; stringbuilder sb(temp);
		sb.add("locale/%1/%2.txt", locale_id, e.id);
		auto special_count = zlen(e.special);
		if(writemode)
			savel(temp, *e.source, e.locale, fields_count, e.special, special_count);
		else
			readl(temp, *e.source, e.locale, fields_count, e.special, special_count);
	}
	if(!writemode)
		checklocalization();
}