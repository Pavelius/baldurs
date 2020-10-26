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

bool readloc(const char* url, array& source, unsigned* fields, int fields_count) {
	auto p_alloc = (const char*)loadt(url);
	if(!p_alloc)
		return false;
	auto p = p_alloc;
	char name[128], value[8192];
	auto records_read = 0;
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
			if(pt[0] == '.' && fields_count > 1) {
				// ����� ����� ����� �������� ��������, ������ �� �� �� ����.
				// ��������, �� �������, ��� ���� �������� name, nameof, text.
				// �� ����� ������: Red|Red is bright color. Use it to hilite errors.
				// �� ������ �����: name='Red', nameof=0, text='Red is bright color. Use it to hilite errors'
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
		records_read++;
	}
	delete p_alloc;
	return true;
}

bool saveloc(const char* url, array& source, unsigned* fields, int fields_count) {
	io::file file(url, StreamText | StreamWrite);
	if(!file)
		return false;
	int count = source.getcount();
	for(auto i = 0; i < count; i++) {
		auto p = source.ptr(i);
		auto id = *((const char**)p);
		if(!id ||id[0]==0)
			continue;
		file << id << ": ";
		auto out_count = 0;
		for(auto j = 0; j < fields_count; j++) {
			if(!fields[j])
				continue;
			auto pr = *((const char**)((char*)p + fields[j]));
			if(j == fields_count - 1)
				file << ". ";
			else if(out_count!=0)
				file << "|";
			if(pr)
				file << pr;
			out_count++;
		}
		file << "\r\n";
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
		if(writemode)
			saveloc(temp, *e.source, e.locale, fields_count);
		else
			readloc(temp, *e.source, e.locale, fields_count);
	}
}