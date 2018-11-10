#include "crt.h"
#include "io.h"

struct filepack {
	struct element {
		char			name[32]; // File name and extension
		unsigned		offset;
		unsigned		size;
	};
	char				signature[4];
	char				version[4];
	unsigned			count;
};

static void remove_files(const char* folder) {
	char temp[512];
	for(io::file::find e(folder); e; e.next()) {
		auto fn = e.name();
		if(fn[0] == '.')
			continue;
		io::file::remove(e.fullname(temp));
	}
}

void archive_pack(const char* arc_name, const char* folder) {
	char temp[512];
	filepack pack = {"PAK", "1.0"};
	filepack::element elements[512] = {0};
	unsigned offset = sizeof(pack);
	for(io::file::find e(folder); e; e.next()) {
		auto fn = e.name();
		if(fn[0] == '.')
			continue;
		auto p = elements + (pack.count++);
		zcpy(p->name, fn, sizeof(p->name));
		p->size = io::file::getsize(e.fullname(temp));
		p->offset = offset;
		offset += p->size;
	}
	io::file file(arc_name, StreamWrite);
	file.write(&pack, sizeof(pack));
	file.write(elements, pack.count * sizeof(filepack::element));
	for(unsigned i = 0; i < pack.count; i++) {
		io::file e(szurl(temp, folder, elements[i].name), StreamRead);
		auto p = elements + i;
		unsigned s = 0;
		while(s < p->size) {
			unsigned sz = sizeof(temp);
			if(s + sz > p->size)
				sz = p->size - s;
			e.read(temp, sz);
			file.write(temp, sz);
			s += sz;
		}
	}
}

void archive_unpack(const char* arc_name, const char* folder) {
	char temp[512];
	filepack pack = {};
	filepack::element elements[512] = {};
	io::file file(arc_name, StreamRead);
	file.read(&pack, sizeof(pack));
	file.read(elements, pack.count * sizeof(filepack::element));
	for(unsigned i = 0; i < pack.count; i++) {
		io::file e(szurl(temp, folder, elements[i].name), StreamWrite);
		auto p = elements + i;
		unsigned s = 0;
		while(s < p->size) {
			unsigned sz = sizeof(temp);
			if(s + sz > p->size)
				sz = p->size - s;
			file.read(temp, sz);
			e.write(temp, sz);
			s += sz;
		}
	}
}