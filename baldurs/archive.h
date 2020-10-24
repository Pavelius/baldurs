#include "crt.h"
#include "io.h"

#pragma once

// Fast and simple driver for streaming binary data
struct archive {
	typedef aref<array*> pointera;
	io::stream&		source;
	bool			writemode;
	pointera		pointers;
	archive(io::stream& source, bool writemode) : source(source), writemode(writemode), pointers() {}
	archive(io::stream& source, bool writemode, const pointera& pointers) : source(source), writemode(writemode), pointers(pointers) {}
	void			set(void* value, unsigned size);
	void			setpointer(void** value);
	bool			signature(const char* id);
	bool			version(short major, short minor);

	// Any pointer class
	template<class T> void set(T*& value) {
		setpointer((void**)&value);
	}
	// Full specialization for strings
	template<> void set<const char>(const char*& e) {
		if(writemode) {
			unsigned len = zlen(e);
			source.write(&len, sizeof(len));
			if(len)
				source.write(e, len);
		} else {
			unsigned len;
			char temp[128 * 128];
			source.read(&len, sizeof(len));
			e = 0;
			if(len) {
				source.read(temp, len);
				temp[len] = 0;
				e = szdup(temp);
			}
		}
	}
	// Array with fixed count
	template<typename T, unsigned N> void set(T(&value)[N]) {
		for(int i = 0; i < N; i++)
			set(value[i]);
	};
	// Fixed data collection
	template<typename T, unsigned N> void set(adat<T, N>& value) {
		set(value.count);
		for(auto& e : value)
			set(e);
	}
	// Custom aref collection
	template<typename T> void set(aref<T>& value) {
		set(value.count);
		set(value.data);
	}
	// All simple types and requisites
	template<class T> void set(T& value) {
		set(&value, sizeof(value));
	}
};
