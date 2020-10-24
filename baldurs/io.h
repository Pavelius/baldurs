#pragma once

enum seek_flags { SeekSet, SeekCur, SeekEnd };
enum stream_flags {
	StreamRead = 1,
	StreamWrite = 2,
	StreamText = 4,
};

namespace io {
// Network protocols
enum protocols { NoProtocol, TCP, UDP };
// Abstract bi-stream interface
struct stream {
	stream&				operator<<(const char* t); // Post text string into stream data in correct coding.
	stream&				operator<<(const int n); // Post number as string into stream data in correct coding.
	unsigned char		get();
	unsigned short		getLE16();
	unsigned			getLE32();
	unsigned			getsize();
	void				gettext(char* result, int max_buffer);
	virtual int			read(void* result, int count) = 0;
	void				read(bool& value) { read(&value, sizeof(value)); }
	void				read(int& value) { read(&value, sizeof(value)); }
	void				read(unsigned& value) { read(&value, sizeof(value)); }
	void				read(short& value) { read(&value, sizeof(value)); }
	void				read(unsigned short& value) { read(&value, sizeof(value)); }
	void				read(unsigned char& value) { read(&value, sizeof(value)); }
	template<class T> void read(T& object);
	int					readsig(char* signature) { return read(signature, 4); }
	void				puttext(const char* text);
	virtual int			seek(int count, int rel = SeekCur) { return 0; };
	virtual int			write(const void* result, int count) = 0;
	void				write(bool value) { write(&value, sizeof(value)); }
	void				write(int value) { write(&value, sizeof(value)); }
	void				write(unsigned value) { write(&value, sizeof(value)); }
	void				write(short value) { write(&value, sizeof(value)); }
	void				write(unsigned short value) { write(&value, sizeof(value)); }
	void				write(unsigned char value) { write(&value, sizeof(value)); }
	template<class T> void write(const T& e); // For overloaded stream output
	void				writescan(void* p, int width, int heght, int scan_line, int element_size);
	int					writesig(const char* signature) { return write(signature, 4); }
};
struct sequence : public stream {
	int					read(void* result, int count) override;
	int					write(const void* result, int count) override;
	int					seek(int count, int rel = SeekCur) override;
	bool				match(const char* value);
	sequence(io::stream& parent);
private:
	char				cashed[64];
	int					cashed_count;
	io::stream&			parent;
	//
	void				makecashe(int count);
};
struct file : stream {
	struct find {
		find(const char* url);
		~find();
		const char*		name();
		const char*		fullname(char* result);
		void			next();
		operator bool() const { return handle != 0; }
	private:
		char			path[261];
		char			reserved[512];
		void*			handle;
	};
	file();
	file(const char* url, unsigned flags = StreamRead);
	~file();
	operator bool() const { return handle != 0; }
	void				close();
	bool				create(const char* url, unsigned flags);
	static bool			exist(const char* url);
	static char*		getdir(char* url, int size);
	static char*		getmodule(char* url, int size);
	static unsigned		getsize(const char* url);
	static bool			makedir(const char* url);
	int					read(void* result, int count) override;
	static bool			remove(const char* url);
	int					seek(int count, int rel) override;
	static bool			setdir(const char* url);
	int					write(const void* result, int count) override;
private:
	int					handle;
};
struct memory : public stream {
	memory(void* data, int size);
	int					read(void* result, int count) override;
	int					seek(int count, int rel) override;
	int					write(const void* result, int count) override;
private:
	unsigned char*		data;
	int					pos;
	int					size;
};
struct node {
	node*				parent;
	const char*			name;
	int					type, index;
	bool				skip; // set this if you want skip block
	int					params[12];
	//
	node(int type = 0);
	node(node& parent, const char* name = "", int type = 0);
	bool				operator==(const char* name) const;
	//
	int					getlevel() const;
};
}