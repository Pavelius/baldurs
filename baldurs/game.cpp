#include "main.h"

gamei game;

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
}