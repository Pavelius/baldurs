#include "main.h"

const int maximum_strings = 32;

static const char* read_string_v1(const char* p, char* ps, const char* pe) {
	while(*p && *p != '\n' && *p != '\r') {
		if(ps < pe)
			*ps++ = *p;
		p++;
	}
	*ps = 0;
	while(*p == '\n' || *p == '\r') {
		p = skipcr(p);
		p = skipsp(p);
	}
	return p;
}

static const char* read_identifier(const char* p, char* ps, const char* pe) {
	while(*p && (ischa(*p) || isnum(*p) || *p == '_' || *p == ' ')) {
		if(ps < pe)
			*ps++ = *p++;
		else
			break;
	}
	*ps = 0;
	return p;
}

static const char* read_special_index(const char* p, int& result) {
	result = -1;
	if(p[0] != '.')
		return p;
	return stringbuilder::readnum(p + 1, result);
}

static void apply_value(array& source, unsigned* fields, const char* id_value, const char** strings, int count) {
	auto pf = source.find(id_value, 0);
	if(pf == -1)
		return;
	auto p = source.ptr(pf);
	for(auto i = 0; i < count; i++) {
		if(!strings[i])
			continue;
		if(!fields[i])
			continue;
		auto pp = (const char**)((char*)p + fields[i]);
		*pp = szdup(strings[i]);
	}
}

bool readloc(const char* url, array& source, unsigned* fields, int fields_count, unsigned* special, int special_count) {
	auto p_alloc = (const char*)loadt(url);
	if(!p_alloc)
		return false;
	auto p = p_alloc;
	char name[128], value[8192];
	auto records_read = 0;
	int special_index;
	while(*p) {
		p = read_identifier(p, name, name + sizeof(name) - 1);
		p = read_special_index(p, special_index);
		if(p[0] != ':')
			break;
		p = skipsp(p + 1);
		p = read_string_v1(p, value, value + sizeof(value) - 1);
		const char* strings[maximum_strings] = {};
		auto pt = value;
		strings[0] = pt;
		if(special_index == -1) {
			auto count = 0;
			while(pt[0]) {
				if(pt[0] == '.' && fields_count > 1) {
					// Через точку пишут поседний реквизит, сколко бы их не было.
					// Например, мы указали, что надо заполнит name, nameof, text.
					// На входе строка: Red|Red is bright color. Use it to hilite errors.
					// На выходе будет: name='Red', nameof=0, text='Red is bright color. Use it to hilite errors'
					pt[0] = 0;
					pt = (char*)skipsp(pt + 1);
					strings[fields_count - 1] = pt;
					count = fields_count;
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
			if(count > fields_count)
				count = fields_count;
			apply_value(source, fields, name, strings, count);
		} else {
			if(special && special_index < special_count)
				apply_value(source, special + special_index, name, strings, 1);
		}
		records_read++;
	}
	delete p_alloc;
	return true;
}

static int sort_by_id(const void* v1, const void* v2, void* param) {
	auto p1 = *(*((const char***)v1));
	auto p2 = *(*((const char***)v2));
	return strcmp(p1, p2);
}

bool saveloc(const char* url, array& source, unsigned* fields, int fields_count, unsigned* special, int special_count) {
	io::file file(url, StreamText | StreamWrite);
	if(!file)
		return false;
	int count = source.getcount();
	vector<void*> sorted;
	for(auto i = 0; i < count; i++)
		sorted.add(source.ptr(i));
	sorted.sort(sort_by_id);
	for(auto i = 0; i < count; i++) {
		auto p = sorted[i];
		auto id = *((const char**)p);
		if(!id || id[0] == 0)
			continue;
		file << id << ": ";
		auto out_count = 0;
		for(auto j = 0; j < fields_count; j++) {
			if(!fields[j])
				continue;
			auto pr = *((const char**)((char*)p + fields[j]));
			if(j == fields_count - 1)
				file << ". ";
			else if(out_count != 0)
				file << "|";
			if(pr)
				file << pr;
			out_count++;
		}
		file << "\r\n";
		for(auto j = 0; j < special_count; j++) {
			if(!special[j])
				continue;
			auto pr = *((const char**)((char*)p + special[j]));
			if(!pr || !pr[0])
				continue;
			file << id << "." << j << ": " << pr;
			file << "\r\n";
		}
	}
	return true;
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
			saveloc(temp, *e.source, e.locale, fields_count, e.special, special_count);
		else
			readloc(temp, *e.source, e.locale, fields_count, e.special, special_count);
	}
}