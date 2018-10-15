#include "main.h"
#include "rects.h"

#define Type2da			0x03f4
#define TypeBam			0x03e8
#define TypeMos			0x03ec
#define TypeChu			0x03ea
#define TypeBmp			0x0001
#define TypeSpl			0x03ee
#define TypeWed			0x03e9
#define TypeAre			0x03f2
#define TypeTis			0x03eb
#define TypeItm			0x03ed
#define TypeCre			0x03f1

static const char*		path_to_source = "d:/games/IWD2";
static const char*		path_to_dest = "data";
static const char*		path_to_dest_original = "origin";
static bool				use_cleanup = false;
extern const char*		util_spells_index[];

static unsigned char* xor_2da(unsigned char* data, int size) {
	static unsigned char key[64] = {
		0x88, 0xa8, 0x8f, 0xba, 0x8a, 0xd3, 0xb9, 0xf5, 0xed, 0xb1, 0xcf, 0xea, 0xaa, 0xe4, 0xb5, 0xfb,
		0xeb, 0x82, 0xf9, 0x90, 0xca, 0xc9, 0xb5, 0xe7, 0xdc, 0x8e, 0xb7, 0xac, 0xee, 0xf7, 0xe0, 0xca,
		0x8e, 0xea, 0xca, 0x80, 0xce, 0xc5, 0xad, 0xb7, 0xc4, 0xd0, 0x84, 0x93, 0xd5, 0xf0, 0xeb, 0xc8,
		0xb4, 0x9d, 0xcc, 0xaf, 0xa5, 0x95, 0xba, 0x99, 0x87, 0xd2, 0x9d, 0xe3, 0x91, 0xba, 0x90, 0xca,
	};
	for(int i = 0; i < size; i++)
		data[i] = key[i % sizeof(key)] ^ data[i];
	return data;
}

static const char* rsurl(char* result, const char* name, const char* ext, const char* fext = 0) {
	if(!fext)
		fext = ext;
	zcpy(result, path_to_dest);
	zcat(result, "/");
	zcat(result, fext);
	zcat(result, "/");
	zcat(result, name);
	zcat(result, ".");
	zcat(result, ext);
	return result;
}

static const char* rsurlo(char* result, const char* name, const char* ext = "bam", const char* postfix = 0) {
	char fnwe[64];
	zcpy(result, path_to_dest_original);
	zcat(result, "/");
	zcat(result, ext);
	zcat(result, "/");
	zcat(result, szfnamewe(fnwe, name));
	if(postfix)
		zcat(result, postfix);
	zcat(result, ".");
	zcat(result, ext);
	return result;
}

static const char* orurl(char* result, const char* name, const char* ext, const char* fext = 0) {
	if(!fext)
		fext = ext;
	zcpy(result, path_to_source);
	zcat(result, "/");
	zcat(result, fext);
	zcat(result, "/");
	zcat(result, name);
	zcat(result, ".");
	zcat(result, ext);
	return result;
}

static void gwrite(const char* url, const void* p, unsigned size, int type) {
	if(!url)
		return;
	io::file file(url, StreamWrite);
	if(!file)
		return;
	if(type == Type2da) {
		char* p1 = new char[size];
		memcpy(p1, p, size);
		xor_2da((unsigned char*)p1, size);
		file.write(p, size);
	} else
		file.write(p, size);
}

static const char* gext(int type) {
	switch(type) {
	case TypeMos: return "mos";
	case TypeBam: return "bam";
	case Type2da: return "2da";
	case TypeChu: return "chu";
	case TypeBmp: return "bmp";
	case TypeSpl: return "spl";
	case TypeWed: return "wed";
	case TypeAre: return "are";
	case TypeItm: return "itm";
	case TypeCre: return "cre";
	default: return "unk";
	}
}

static int find_index(const char* text, const char** source) {
	for(const char** p = source; *p; p++) {
		if(strcmp(text, *p) == 0)
			return p - source;
	}
	return -1;
}

static int find_index_spell(const char* text, const char** source) {
	char temp[12];
	zcpy(temp, text);
	zend(temp)[-1] = 'B';
	return find_index(temp, source);
}

static const char* grsname(char* name) {
	static char temp[9];
	zcpy(temp, name, 8);
	return temp;
}

static sprite* create_sprite(int count, int pallette = 0, int size = 1000000 * 8, int cicles = 0, int cicle_indexes = 0) {
	char* p = new char[size];
	memset(p, 0, size);
	sprite* p1 = (sprite*)p;
	p1->setup(count, pallette, cicles, cicle_indexes);
	return p1;
}

static void delete_sprite(sprite* p, const char* name, const char* efolder = "pma") {
	char result[260];
	zcpy(result, path_to_dest);
	zcat(result, "/");
	zcat(result, efolder);
	zcat(result, "/");
	zcat(result, name);
	zcat(result, ".pma");
	p->write(result);
	delete p;
}

static bool has(unsigned char* p, unsigned char t) {
	while(*p != 0xFF) {
		if(*p++ == t)
			return true;
	}
	return false;
}

//static unsigned char* get_colors_to_choose(unsigned char* result, int type) {
//	unsigned char* p = result;
//	int i1 = 0;
//	int i2 = 100;
//	unsigned char* p1 = p + 63;
//	*p = 0xFF;
//	while(i1 < i2) {
//		unsigned char m = bsget(i1, type);
//		if(!has(result, m)) {
//			if(p < p1) {
//				*p++ = m;
//				*p = 0xFF;
//			}
//		}
//		i1++;
//	}
//	return result;
//}

static const char* get_header(const char* header) {
	return szline(header, 2);
}

static int get_color(const color* pal, color v) {
	for(int i = 0; i < 256; i++) {
		if(pal[i].r == v.r
			&& pal[i].g == v.g
			&& pal[i].b == v.b)
			return i;
	}
	return -1;
}

static int get_color_count(const color* pal, color v) {
	int result = 0;
	for(int i = 0; i < 256; i++) {
		if(pal[i].r == v.r
			&& pal[i].g == v.g
			&& pal[i].b == v.b)
			result++;
	}
	return result;
}

static const char* get_value(const char* p, int index) {
	static char temp[512];
	while(true) {
		if(*p == 0 || *p == 13 || *p == 10)
			return "";
		while(*p == 0x20)
			p++;
		const char* pe = zchr(p, '\t');
		if(!pe)
			pe = zchr(p, '\n');
		if(!pe)
			pe = zchr(p, '\r');
		if(!pe)
			pe = zend(p);
		int m = pe - p;
		m = imin(m, (int)sizeof(temp));
		if(!index) {
			zcpy(temp, p, m);
			return temp;
		}
		if(pe[0] != '\t')
			return "";
		p = pe + 1;
		index--;
	}
}

namespace {
#pragma pack(push)
#pragma pack(1)
struct signature {
	char				name[4];
	char				version[4];
	unsigned char*		offs(unsigned o) { return (unsigned char*)this + o; }
};
struct zip : public signature {
	unsigned			lenght;
};
struct mos : public signature {
	short unsigned		width;
	short unsigned		height;
	short unsigned		columns;
	short unsigned		rows;
	unsigned			size;
	unsigned			pallettes;
};
struct bam : public signature {
	short unsigned		frames;
	unsigned char		cicles;
	unsigned char		compressed_index;
	unsigned			frames_offset;
	unsigned			pallette_offset;
	unsigned			frames_lookup_offset;
	struct frame {
		short unsigned	width;
		short unsigned	height;
		short int		x;
		short int		y;
		unsigned		offset; // if bit 31 is set, then there is no compression
	};
	struct cicle {
		short unsigned	count;
		short unsigned	start;
	};
	frame*				gframe(int index) { return (frame*)offs(frames_offset) + index; }
	cicle*				gcicle(int index) { return (cicle*)offs(frames_offset + frames * sizeof(frame)) + index; }
	unsigned char*		gpal() { return offs(pallette_offset); }
};
struct chu : public signature {
	int					windows;
	unsigned			controls_offset;
	unsigned			windows_offset;
	struct window {
		short unsigned	id;
		short unsigned	unknown;
		short			x;
		short			y;
		short			width;
		short			height;
		short			has_background;
		short			controls_count;
		char			rname[8];
		short			controls_start;
		short			unknown2;
	};
	struct table {
		unsigned		offset;
		unsigned		lenght;
	};
	struct control {
		unsigned		id;
		short			x;
		short			y;
		short unsigned	width;
		short unsigned	height;
		unsigned char	type;
		unsigned char	unknown;
	};
	struct button : public control {
		char			rname[8];
		unsigned char	cicle;
		unsigned char	justify;
		unsigned char	unpressed;
		unsigned char	anchorX1;
		unsigned char	pressed;
		unsigned char	anchorX2;
		unsigned char	selected;
		unsigned char	anchorY1;
		unsigned char	disabled;
		unsigned char	anchorY2;
	};
	struct textarea : public control {
		char			font_initials[8];
		char			font[8];
		color			c[3];
		int				scroll_id;
	};
	struct label : public control {
		int				rstr;
		char			font[8];
		color			c[2];
		short unsigned	flags;
	};
};
struct bif : public signature {
	unsigned			file_count;
	int					tile_count;
	unsigned			file_offset;
	//
	struct file {
		unsigned		locator;
		unsigned		offset;
		unsigned		size;
		short unsigned	type;
		short unsigned	unknown;
	};
	struct tile {
		unsigned		locator;
		unsigned		offset;
		int				count;
		unsigned		element_size;
		short unsigned	type;
		short unsigned	unknown;
	};
	struct cblock {
		unsigned		decompressed_size;
		unsigned		size;
	};
	//
	static bif*			cashe[256];
	//
	unsigned char*		offs(unsigned o) { return (unsigned char*)this + o; }
	file&				gf(int index) { return ((file*)offs(file_offset))[index]; }
	void				write(int index, const char* rname);
};
struct bifc : public signature {
	unsigned char		filename_lenght;
	char*				filename;
	struct block {
		unsigned		decompressed_size;
		unsigned		size;
	};
};
struct tlk : public signature {
	enum entry_types { None, Text, Sound, Standart, Message };
	struct entry {
		short unsigned	type;
		char			rname[8];
		unsigned		volume_variance;
		unsigned		pitch_variance;
		unsigned		offset;
		unsigned		size;
	};
	short unsigned		lang;
	unsigned			count;
	unsigned			string_data;
	entry				entries[1];
	char*				str(unsigned o) { return (char*)this + string_data + o; }
};
struct spl : public signature {
	unsigned			name_unidentified;
	unsigned			name_identified;
	char				sound[8];
	unsigned			flags;
	unsigned short		type;
	unsigned			exclusion_flags;
	unsigned short		casting_graphics;
	unsigned char		min_level;
	unsigned char		spell_school;
	unsigned char		min_strenght;
	unsigned char		spell_school_ext;
	unsigned char		min_str_bonus;
	unsigned char		usability1;
	unsigned char		min_int_bonus;
	unsigned char		usability2;
	unsigned char		min_dex_bonus;
	unsigned char		usability3;
	unsigned char		min_wis_bonus;
	unsigned char		usability4;
	unsigned short		min_con;
	unsigned short		min_cha;
	int					level;
	unsigned short		stack_amount;
	char				bam_spellbook[8];
	unsigned short		lore_identification;
	unsigned			ground_icon;
	unsigned			weight;
	unsigned			str_description_unidentified;
	unsigned			str_description_identified;
	char				bam_description[8];
	unsigned			enchantment;
	unsigned			ext_header_offset;
	unsigned short		ext_header_count;
	unsigned			feature_table_offset;
	struct ext {
		enum form_types { Standart, Projectile };
		enum target_types { Invalid, Creature, Crash, CharacterPortrait, Area, Self, Unknown, None };
		unsigned char	form;
		unsigned char	unknown;
		unsigned short	location;
		char			bam_memoried[8];
		unsigned char	target;
		unsigned char	target_number;
		short unsigned	range;
		short unsigned	level_required;
		short unsigned	casting_time;
		short unsigned	times_per_day;
		short unsigned	dice_sides;
		short unsigned	dice_thrown;
		short unsigned	enchanted;
		short unsigned	damage_type;
		short unsigned	feature_blocks_count;
		short unsigned	feature_blocks_offset;
		short unsigned	charges;
		short unsigned	charge_depletion_behaviour;
		short unsigned	projectile;
	};
};
struct wed : public signature {
	int					overlays_count;
	int					doors_count;
	unsigned			overlays_offset;
	unsigned			secondary_header_offset;
	unsigned			doors_offset;
	unsigned			doors_tile_cells_indices_offset;
	struct sheader {
		int				poligons_count;
		unsigned		poligons_offset;
		unsigned		verticles_offset;
		unsigned		wallgroup_offset;
		unsigned		poligon_indices_offset;
	};
	struct overlay {
		short			width;
		short			height;
		char			tileset[8];
		short			unique_tile_count;
		short unsigned	movement_type;
		unsigned		tilemap_offset;
		unsigned		tile_index_lookup_offset;
	};
	struct tilemap {
		short unsigned	start;
		short unsigned	count;
		short unsigned	start_alternate;
		unsigned		flags;
	};
	struct door {
		char			name[8];
		short unsigned	closed;
		short unsigned	start;
		short unsigned	count;
		short unsigned	polygons_open_count;
		short unsigned	polygons_close_count;
		unsigned		polygons_open_offset;
		unsigned		polygons_close_offset;
	};
	struct polygon {
		int				verticles_start;
		int				verticles_count;
		unsigned char	flags;
		unsigned char	height;
		short			min_x;
		short			max_x;
		short			min_y;
		short			max_y;
	};
};
struct are : public signature {
	struct direction {
		char			name_area[8];
		unsigned		flags;
	};
	char				name_wed[8];
	unsigned			last_saved;
	unsigned			flags;
	direction			world[4];
	unsigned short		type;
	unsigned short		rain_probably;
	unsigned short		snow_probably;
	unsigned short		fog_probably;
	unsigned short		lighting_probably;
	unsigned short		wind_speed;
	unsigned char		dificult_level_2;
	unsigned char		dificult_level_3;
	unsigned char		unknown[2];
	unsigned char		unknown1[12];
	unsigned			actors_offset;
	short				actors_count;
	short				regions_count;
	unsigned			regions_offset;
	unsigned			spawn_points_offset;
	int					spawn_points_count;
	unsigned			entrances_offset;
	int					entrances_count;
	int					containers_offset;
	short				containers_count;
	short				items_count;
	int					items_offset;
	int					verticles_offset;
	short				verticles_count;
	short				ambients_count;
	int					ambients_offset;
	int					variables_offset;
	int					variables_count;
	short				tiled_object_flags_offset;
	short				tiled_object_flags_count;
	char				name_script[8];
	int					explored_size;
	int					explored_offset;
	int					doors_count;
	int					doors_offset;
	int					animations_count;
	int					animations_offset;
	int					tiled_object_count;
	int					tiled_object_offset;
	int					song_offset;
	int					rest_interruptions_offset;
	int					automap_offset;
	int					automap_count;
	char				unknown3[80];
	struct door {
		char			name[32];
		char			name_wed[8];
		unsigned		flags;
		int				open_verticles_start;
		short			open_verticles_count;
		short			close_verticles_count;
		int				close_verticles_start;
		rects			open_rect;
		rects			close_rect;
		int				open_search_indices_start;
		short			open_search_indices_count;
		short			close_search_indices_count;
		int				close_search_indices_start;
		short			hp;
		short			ac;
		char			open_sound[8];
		char			close_sound[8];
		int				cursor_index;
		short			trap_detection_difficult;
		short			trap_remove_difficult;
		short			trapped;
		short			detected;
		point			launch;
		char			key_name[8];
		char			script_name[8];
		int				detection_difficult;
		int				lock_difficult;
		point			points[2];
		int				lockpick_strref;
		char			travel_name[24];
		int				speaker_strref;
		char			dialog_name[8];
		char			unknown[8];
	};
	struct conainer {
		char			name[32];
		point			pos;
		short			type;
		short			lock_difficulty;
		unsigned		flags;
		short			trap_detection_difficulty;
		short			trap_removal_difficulty;
		short			is_trapped;
		short			trap_detected;
		point			launch;
		rects			box;
		int				items_start;
		int				items_count;
		char			trap_script[8];
		int				verticles_start;
		short			verticles_count;
		short			trigger_range;
		char			owner[32];
		char			key_item[8];
		int				break_difficulty;
		int				lockpick_strref;
		char			unknown[56];
	};
	enum region_types { RegionTrigger, RegionInfo, RegionTravel };
	struct region {
		char			name[32];
		short			type;
		rects			box;
		short			verticles_count;
		int				verticles_start;
		int				trigger_value;
		int				cursor_index;
		char			destination_name[8];
		char			entrance_name[32];
		unsigned		flags;
		int				info_strref;
		short			trap_detection_difficulty;
		short			trap_removal_difficulty;
		short			is_traped;
		short			trap_detected;
		point			launch;
		char			key_name[8];
		char			script_name[8];
		point			use_alternative;
		int				unknown;
		char			unknown2[56];
	};
	struct entrance {
		char			name[32];
		point			pos;
		short			orientation;
		char			unused[66];
	};
	struct tile {
		char			name[32];
		char			rsname[8];
		unsigned		flags; // bit 0: Currently in secondary state bit 1 : Can be seen through
		unsigned		open_offset;
		short unsigned	open_count;
		short unsigned	closed_count;
		unsigned		closed_offset;
		char			unused[48];
	};
	struct animation {
		char			name[32];
		point			position;
		unsigned		schedule;
		char			rsname[8];
		unsigned short	circle;
		unsigned short	frame;
		unsigned		flags;
		unsigned short	height;
		unsigned short	transparency;
		unsigned short	start_frame;
		unsigned char	chance_loop;
		unsigned char	skip_cycles;
		char			rsname_pallette[8];
		unsigned		unused;
	};
};
struct itm : public signature {
	struct info {
		const char*		name;
		char			resref[9];
		int				magic;
		int				price;
		int				weight;
		unsigned char	minor;
		unsigned char	major;
		const char*		descrition;
		static int compare(const void* p1, const void* p2) {
			return szcmpi(((info*)p1)->name, ((info*)p2)->name);
		}
	};
	int					name_generic;
	int					name_identified;
	char				resrepl[8];
	unsigned			flags;
	short				type;
	char				usability_bitmask[4];
	char				weapon_animation[2];
	short				min_level;
	short				min_str;
	char				min_str_bonus;
	char				usability_1;
	char				min_int;
	char				usability_2;
	char				min_dex;
	char				usability_3;
	char				min_wis;
	char				usability_4;
	char				min_con;
	char				weapon_proficience;
	short				min_cha;
	int					price;
	short				stack;
	char				resicon[8];
	short				lore_to_id;
	char				ground[8];
	int					weight;
	int					description;
	int					description_identified;
	char				resdesc[8];
	int					enchantment;
	unsigned			eheader_offset;
	short				eheader_count;
	unsigned			features_offset;
	short				equiping_feature_index;
	short				equiping_feature_count;
	char				unknown[16];
};
struct cre : public signature {
	struct info {
		const char*		name;
		unsigned char	minor;
		unsigned char	major;
		unsigned char	skin;
		unsigned char	hair;
		unsigned char	armor;
		unsigned char	metal;
		unsigned char	leather;
		static int compare(const void* p1, const void* p2) {
			return szcmpi(((info*)p1)->name, ((info*)p2)->name);
		}
	};
	int					name_long;
	int					name_short;
	unsigned			flags;
	int					exp;
	int					power_level;
	int					gold;
	int					status_permanent;
	short				hp;
	short				hp_maximum;
	int					animation;
	unsigned char		color_metal;
	unsigned char		color_minor;
	unsigned char		color_major;
	unsigned char		color_skin;
	unsigned char		color_leather;
	unsigned char		color_armor;
	unsigned char		color_hair;
	char				eff_version;
	char				res_port_small[8];
	char				res_port_large[8];
	char				reputation;
	char				hide_in_shadow;
	short				ac;
	short				ac_crushed;
	short				ac_missile;
	short				ac_piercing;
	short				ac_slashing;
	char				bab;
};
struct pattern {
	const char*			name;
	int					source_cicle_start;
	int					source_cicle_count;
	//
	bam*				source;
	int					frame_start, frame_count;
	int					index_start, index_count;
	int					cicle_start, cicle_count;
	short unsigned		index_source;
};
struct pattern_block {
	const char*			name;
	pattern*			data;
};
struct rkey : public signature {
	struct bif {
		unsigned		lenght;
		unsigned		name;
		short unsigned	name_lenght;
		short unsigned	flags;
	};
	struct res {
		char			name[8];
		short unsigned	type;
		unsigned		locator;
	};
	unsigned			bif_count;
	unsigned			res_count;
	unsigned			bif_offset;
	unsigned			res_offset;
	//
	bif&				gb(int index) { return ((bif*)offs(bif_offset))[index]; }
	res&				gr(int index) { return ((res*)offs(res_offset))[index]; }
	short unsigned		flags(int index) { return gb(index).flags; }
	const char*			name(int index) { return (const char*)offs(gb(index).name); }
	unsigned			lenght(int index) { return gb(index).lenght; }
	unsigned char*		offs(unsigned o) { return (unsigned char*)this + o; }
	static int			rbif(unsigned r) { return (r & 0xFFF00000) >> 20; }
	static int			rfil(unsigned r) { return (r & 0x00000FFF); }
	static int			rtil(unsigned r) { return (r & 0x000FF000) >> 14; }
	const char*			rname(char* result, int index) { zcpy(result, gr(index).name, 8); return result; }
};
struct bmp_header {
	unsigned short		signature; // for bitmap 4D42
	unsigned			size; // size in bytes all file
	unsigned short		reserved[2]; // reserved, must be null
	unsigned			bits; // offset to bitmap bits from top of this structure
};
struct bmp_info {
	unsigned			size; // The number of bytes required by the structure.
	int					width; // The width of the bitmap, in pixels. If biCompression is BI_JPEG or BI_PNG, the biWidth member specifies the width of the decompressed JPEG or PNG image file, respectively.
	int					height; // The height of the bitmap, in pixels. If biHeight is positive, the bitmap is a bottom-up DIB and its origin is the lower-left corner. If biHeight is negative, the bitmap is a top-down DIB and its origin is the upper-left corner.
	unsigned short		planes; // The number of planes for the target device. This value must be set to 1.
	unsigned short		bpp; // Bits per pixels. Can be 8, 16, 24, 32
	unsigned			compression;
	unsigned			size_image; // The size, in bytes, of the image.This may be set to zero for BI_RGB bitmaps.
	int					pels_per_meter_x; // The horizontal resolution, in pixels-per-meter, of the target device for the bitmap. An application can use this value to select a bitmap from a resource group that best matches the characteristics of the current device.
	int					pels_per_meter_y; // The vertical resolution, in pixels-per-meter, of the target device for the bitmap.
	unsigned			color_used; // Used mainly with 8-bit per pixel image format.
	unsigned			color_important;
};
#pragma pack(pop)
}
static struct rkey*		key;
bif*					bif::cashe[256];

void bif::write(int fi, const char* rname) {
	char temp[260];
	file& f = gf(fi);
	const char* ext = gext(f.type);
	zcpy(temp, path_to_dest_original);
	zcat(temp, "/");
	zcat(temp, ext);
	zcat(temp, "/");
	zcat(temp, rname);
	zcat(temp, ".");
	zcat(temp, ext);
	gwrite(temp, offs(f.offset), f.size, f.type);
}

static bif* getbif(const char* name) {
	char temp[260];
	char fnwe[260];
	bif* bi = (bif*)loadb(orurl(temp, szfnamewe(fnwe, name), "bif", "data"));
	if(!bi)
		return 0;
	if(bi->name[3] == 'C') {
		// Decompress
		zip* bc = (zip*)bi;
		unsigned file_size = bc->lenght;
		unsigned char* p0 = new unsigned char[file_size];
		bif::cblock* pb = (bif::cblock*)((char*)bi + sizeof(zip));
		unsigned char* p1 = p0;
		unsigned char* pe = p0 + file_size;
		while(p1 < pe) {
			xdecode("zip",
				p1, pb->decompressed_size,
				(unsigned char*)pb + sizeof(bif::cblock), pb->size);
			p1 += pb->decompressed_size;
			pb = (bif::cblock*)((char*)pb + sizeof(bif::cblock) + pb->size);
		}
		delete bi;
		bi = (bif*)p0;
	}
	return bi;
}

static bif* getbif(int index) {
	if(!bif::cashe[index])
		bif::cashe[index] = getbif(key->name(index));
	return bif::cashe[index];
}

static tlk* load_tlk() {
	char temp[260];
	return (tlk*)loadb(szurl(temp, path_to_source, "dialog.tlk"));
}

static const char* getstr(char* result, unsigned index) {
	static tlk* source;
	result[0] = 0;
	if(!index || index == -1)
		return result;
	if(!source)
		source = load_tlk();
	if(!source)
		return result;
	if(index >= source->count)
		return result;
	tlk::entry e = source->entries[index];
	if(!e.offset || !e.size)
		return result;
	memcpy(result, source->str(e.offset), e.size);
	result[e.size] = 0;
	return result;
}

static void load_key() {
	char temp[260];
	key = (rkey*)loadb(szurl(temp, path_to_source, "CHITIN.key"));
}

static signature* loadr(const char* url, int& file_size, const char* ext = "bam", bool try_use_cleanup = false) {
	char temp[260];
	zcpy(temp, url);
	signature* p = (signature*)loadb(temp, &file_size);
	if(!p) {
		p = (signature*)loadb(rsurlo(temp, url, ext), &file_size);
		if(!p)
			return 0;
	}
	if(p->name[3] == 'C') {
		unsigned char* p1 = new unsigned char[((zip*)p)->lenght];
		xdecode("zip", p1, ((zip*)p)->lenght, (unsigned char*)p + sizeof(zip), file_size - sizeof(zip));
		file_size = ((zip*)p)->lenght;
		delete p;
		p = (signature*)p1;
	}
	if(try_use_cleanup && use_cleanup)
		io::file::remove(temp);
	return p;
}

static void load_pallette(color* result, void* source) {
	unsigned char* s = (unsigned char*)source;
	for(int i = 0; i < 256; i++) {
		result[i].r = s[i * 4 + 2];
		result[i].g = s[i * 4 + 1];
		result[i].b = s[i * 4 + 0];
		if(result[i].r == 0 && result[i].g == 255 && result[i].b == 0)
			result[i].a = 0;
		else
			result[i].a = 255;
	}
}

static void put_mos(draw::surface& dc, const char* su) {
	draw::state push;
	draw::canvas = &dc;
	int file_size;
	mos* p = (mos*)loadr(su, file_size);
	//dc.resize(0, 0, 32, true);
	dc.resize(p->width, p->height, 32, true);
	draw::setclip();
	unsigned char* pallettes = p->offs(p->pallettes);
	unsigned* tiles_offset = (unsigned*)p->offs(p->pallettes + 256 * 4 * (p->columns*p->rows));
	unsigned tiles_base = p->pallettes + 256 * 4 * (p->columns*p->rows) + 4 * (p->columns*p->rows);
	for(int r = 0; r < p->rows; r++) {
		int sy = 64;
		if(r == p->rows - 1)
			sy = (p->height % sy) ? p->height % sy : sy;
		for(int c = 0; c < p->columns; c++) {
			color pallette[256];
			load_pallette(pallette, pallettes + (256 * 4)*(r*p->columns + c));
			int sx = 64;
			int x = 64 * c;
			int y = 64 * r;
			if(c == p->columns - 1)
				sx = (p->width % sx) ? p->width % sx : sx;
			unsigned char* b = p->offs(tiles_offset[r*p->columns + c] + tiles_base);
			for(int py = 0; py < sy; py++) {
				for(int px = 0; px < sx; px++) {
					draw::fore = pallette[b[py*sx + px]];
					draw::pixel(x + px, y + py);
				}
			}
		}
	}
	delete p;
}

static void put_bam(draw::surface& dc, bam* file, int index) {
	bam::frame* f = file->gframe(index);
	if(!f)
		return;
	draw::state push;
	draw::canvas = &dc;
	color pallette[256];
	load_pallette(pallette, file->offs(file->pallette_offset));
	unsigned char* s = file->offs(f->offset & 0x7FFFFFFF);
	unsigned char* d = dc.ptr(0, 0);
	unsigned char* d1 = d + f->width*f->height * 4;
	unsigned char ci = 0;
	dc.resize(f->width, f->height, 32, true);
	if(f->offset & 0x80000000) {
		while(d < d1) {
			color r = pallette[*s++];
			d[2] = r.r;
			d[1] = r.g;
			d[0] = r.b;
			d[3] = r.a;
			d += 4;
		}
	} else {
		while(d < d1) {
			unsigned char c = *s++;
			color r = pallette[c];
			int count = 1;
			if(c == ci)
				count = *s++;
			while(count--) {
				d[2] = r.r;
				d[1] = r.g;
				d[0] = r.b;
				d[3] = r.a;
				d += 4;
			}
		}
	}
}

static unsigned char* create_bitmap(bam* file, int index) {
	bam::frame* f = file->gframe(index);
	if(!f)
		return 0;
	unsigned size = f->width*f->height;
	unsigned char* s = file->offs(f->offset & 0x7FFFFFFF);
	unsigned char* result = new unsigned char[size];
	unsigned char* d = result;
	unsigned char* d1 = d + f->width*f->height;
	if(f->offset & 0x80000000)
		memcpy(d, s, size);
	else {
		unsigned char ci = file->compressed_index;
		while(d < d1) {
			unsigned char c = *s++;
			int count = 1;
			if(c == ci)
				count += *s++;
			while(count--) {
				if(d >= d1)
					break;
				*d++ = c;
			}
		}
	}
	return result;
}

static void convert_mos_bmp(const char* su) {
	draw::surface dc;
	put_mos(dc, su);
	char rs[64];
	char rp[260];
	dc.write(rsurl(rp, szfnamewe(rs, su), "bmp"), 0);
}

static void add_mos(sprite* source, const char* url) {
	draw::surface dc;
	put_mos(dc, url);
	source->store(dc.ptr(0, 0), dc.width * 4, dc.width, dc.height, 0, 0, sprite::Auto, 0, 0, -1, 0);
}

static void create_mos(const char* name, const char* pattern) {
	char temp[261];
	zcpy(temp, path_to_dest_original);
	zcat(temp, "/mos");
	int count = 0;
	for(io::file::find e(temp); e; e.next()) {
		if(!szpmatch(e.name(), pattern))
			continue;
		count++;
	}
	if(!count)
		return;
	sprite* p = create_sprite(count, 0, 1000000 * 20);
	for(io::file::find e(temp); e; e.next()) {
		char source[261];
		if(!szpmatch(e.name(), pattern))
			continue;
		e.fullname(source);
		add_mos(p, source);
		if(use_cleanup)
			io::file::remove(source);
	}
	delete_sprite(p, name);
}

unsigned char find_shadow_index(color* pal) {
	for(int i = 0; i < 256; i++) {
		if(*((int*)(pal + i)) == 0)
			return i;
	}
	return 1;
}

static void convert_bam_pma_all(const char* su, unsigned char shadow_index, const char* folder = "pma") {
	int file_size;
	bam* pb = (bam*)loadr(su, file_size);
	if(!pb)
		return;
	sprite* source = create_sprite(pb->frames, 1);
	color pal[256];
	load_pallette(pal, pb->offs(pb->pallette_offset));
	if(shadow_index == 0xFF)
		shadow_index = find_shadow_index((color*)pb->offs(pb->pallette_offset));
	for(int i = 0; i < pb->frames; i++) {
		unsigned char* bm = create_bitmap(pb, i);
		bam::frame* f = pb->gframe(i);
		source->store(bm, f->width, f->width, f->height, f->x, f->y, sprite::RLE8,
			(f->offset & 0x80000000) ? 0 : shadow_index,
			pal);
		delete bm;
	}
	char rs[64];
	delete_sprite(source, szfnamewe(rs, su), folder);
	delete pb;
}

static void convert_bam_pma(const char* su) {
	convert_bam_pma_all(su, 0xFF);
}

static void convert_bam_pma_no_shadow(const char* su) {
	convert_bam_pma_all(su, 0);
}

static void add_bam(sprite* source, bam* pb, int frame, unsigned char shadow_index = 0xFF) {
	if(shadow_index == 0xFF)
		shadow_index = find_shadow_index((color*)pb->offs(pb->pallette_offset));
	color pal[256];
	load_pallette(pal, pb->offs(pb->pallette_offset));
	unsigned char* bm = create_bitmap(pb, frame);
	bam::frame* f = pb->gframe(frame);
	source->store(bm, f->width, f->width, f->height, f->x, f->y, sprite::RLE8,
		shadow_index,
		pal);
	delete bm;
}

static void add_bam(sprite* source, const char* su, unsigned char shadow_index = 0xFF, int minimal_frames = 0) {
	int file_size;
	bam* pb = (bam*)loadr(su, file_size, "bam", true);
	if(!pb)
		return;
	for(int i = 0; i < pb->frames; i++) {
		add_bam(source, pb, i, shadow_index);
		if(minimal_frames)
			minimal_frames--;
	}
	while(minimal_frames > 0) {
		source->addlast();
		minimal_frames--;
	}
	delete pb;
}

static void add_bam_files(sprite* source, const char* pattern, unsigned char shadow_index = 0xFF, int minimal_frames = 0) {
	char temp[261];
	zcpy(temp, path_to_dest_original);
	zcat(temp, "/bam");
	for(io::file::find e(temp); e; e.next()) {
		const char* fn = e.name();
		if(fn[0] == '.')
			continue;
		if(!szpmatch(fn, pattern))
			continue;
		add_bam(source, fn, shadow_index, minimal_frames);
	}
}

static void add_bmp(sprite* source, const char* su) {
	draw::surface dc;
	dc.read(su, 0, 32);
	if(!dc)
		return;
	source->store(dc.ptr(0, 0), dc.width * 4, dc.width, dc.height, 0, 0, sprite::Auto);
}

static void convert_tlk_txt() {
	char temp[256 * 128];
	io::file e(rsurl(temp, "dialog", "txt"), StreamWrite);
	for(unsigned i = 0; i < 100; i++) {
		getstr(temp, i);
		e << temp << "\n";
	}
}

struct fileop {
	virtual	void	action(const char* name) = 0;
};

static void convert_files(const char* type, const char* pattern, fileop& fo) {
	char temp[261];
	zcpy(temp, path_to_dest_original);
	zcat(temp, "/");
	zcat(temp, type);
	for(io::file::find e(temp); e; e.next()) {
		if(e.name()[0] == '.')
			continue;
		if(!szpmatch(e.name(), pattern))
			continue;
		e.fullname(temp);
		fo.action(temp);
	}
}

static int compare_spell(const void* p1, const void* p2) {
	auto e1 = (spell_info*)p1;
	auto e2 = (spell_info*)p2;
	if(e1->levels[Wizard] != e2->levels[Wizard])
		return e1->levels[Wizard] - e2->levels[Wizard];
	return strcmp(e1->name, e2->name);
}

static void convert_spl_cpp(const char* name, const char* pattern) {
	char		temp[260];
	spell_info	spells[512];
	struct add_spells : public fileop {
		int			count;
		spell_info*	spells;
		add_spells() : count(0) {}
		void action(const char* su) override {
			int file_size;
			char temp[260];
			spl* pb = (spl*)loadr(su, file_size, "spl");
			if(!pb)
				return;
			if(!pb->bam_spellbook[0])
				return;
			if(!getstr(temp, pb->name_unidentified)[0])
				return;
			const char* fn = szfname(su);
			spell_info& e = spells[count++];
			e.name = szdup(getstr(temp, pb->name_unidentified));
			e.school = SchoolAbjuration;
			if(szcmpi(fn, "SPWI", 4) == 0) {
				e.levels[Wizard] = pb->level;
				e.levels[Sorcerer] = pb->level;
			} else if(szcmpi(fn, "SPPR", 4) == 0) {
				e.levels[Druid] = pb->level;
			}
			e.frame = find_index_spell(grsname(pb->bam_spellbook), util_spells_index);
		}
		void antitwins() {
			auto p = spells;
			for(int i = 0; i < count; i++) {
				bool is_twin = false;
				for(int j = 0; j < i; j++) {
					if(spells[j].name == spells[i].name) {
						is_twin = true;
						break;
					}
				}
				if(!is_twin)
					*p++ = spells[i];
			}
			count = p - spells;
		}
	};
	memset(spells, 0, sizeof(spells));
	add_spells as;
	as.spells = spells;
	convert_files("spl", pattern, as);
	as.antitwins();
	qsort(spells, as.count, sizeof(spells[0]), compare_spell);
	io::file file(rsurl(temp, name, "cpp"), StreamWrite | StreamText);
	file << "enum spells_tag {\n";
	for(int lev = 1; lev <= 9; lev++) {
		int count = 0;
		file << "\n// Spells level " << lev << "\n";
		for(int i = 0; i < as.count; i++) {
			spell_info e = spells[i];
			if(e.levels[Wizard] != lev)
				continue;
			zcpy(temp, "Spell");
			zcat(temp, sztag(e.name));
			file << temp << ", ";
			count++;
			if((count % 5) == 0)
				file << "\n";
		}
	}
	file << "};\n\n";
	file << "spell_t spells[] = {\n";
	for(int i = 0; i < as.count; i++) {
		spell_info e = spells[i];
		file << "{";
		file << "\"" << e.name << "\", ";
		file << getstr(e.school) << ", ";
		file << e.frame << ", ";
		file << "{";
		file << "}, ";
		file << "},\n";
	}
	file << "};";
}

static void convert_spl_txt_enum(io::stream& e, signature* psig) {
	char temp[260];
	spl* pb = (spl*)psig;
	if(!pb->bam_spellbook[0])
		return;
	zcpy(temp, "Spell");
	getstr(zend(temp), pb->name_unidentified);
	e << sztag(temp) << ",";
}

const char* strfin(const char* s1, const char* s2) {
	for(auto s = s1; *s; s++) {
		if(*s != *s2)
			continue;
		auto ps = s + 1;
		auto pd = s2 + 1;
		while(*ps == *pd && *pd) {
			ps++; pd++;
		}
		if(*pd == 0)
			return s;
	}
	return 0;
}

static void convert_chu_cpp(const char* su) {
	enum controls_type {
		Button, Empthy, Slider, TextEdit, Empthy2, TextArea, Label, ScrollBar
	};
	int file_size;
	char rs[260];
	char temp[4096];
	if(strfin(su, "WORLD.chu"))
		return;
	auto pb = (chu*)loadr(su, file_size, "chu");
	if(!pb)
		return;
	auto table = (chu::table*)pb->offs(pb->controls_offset);
	io::file e(rsurl(temp, szfnamewe(rs, su), "cpp"), StreamWrite | StreamText);
	for(int i = 0; i < pb->windows; i++) {
		auto w = (chu::window*)pb->offs(pb->windows_offset + sizeof(chu::window)*i);
		e << "static void " << grsname(w->rname) << "() {\n";
		int i2 = w->controls_start + w->controls_count & 0xFF;
		for(int i = w->controls_start; i < i2; i++) {
			auto wc = (chu::control*)pb->offs(table[i].offset);
			chu::button* wb;
			chu::textarea* wta;
			chu::label* wl;
			rect rc;
			rc.x1 = wc->x + w->x;
			rc.y1 = wc->y + w->y;
			rc.x2 = rc.x1 + wc->width;
			rc.y2 = rc.y1 + wc->height;
			e << "\t";
			switch(wc->type) {
			case Button:
				wb = (chu::button*)wc;
				e << "button(" << rc.x1 << ", " << rc.y1
					<< ", " << "cmpr(buttonparam, " << wc->id + 1 << ")"
					<< ", " << (wb->disabled ? "Disabled" : "0")
					<< ", " << "res::" << grsname(wb->rname)
					<< ", " << wb->selected << ", " << wb->unpressed << ", " << wb->pressed << ", " << wb->disabled
					<< ", " << 0
					<< ", " << 0
					<< ", " << 0
					<< ");\n";
				break;
			case TextArea:
				wta = (chu::textarea*)wc;
				e << "label(" << rc.x1 << ", " << rc.y1 << ", " << rc.width() << ", " << rc.height()
					<< ", " << "color(" << wta->c[0].r << ", " << wta->c[0].g << ", " << wta->c[0].b << ")"
					<< ");\n";
				break;
			case Label:
				wl = (chu::label*)wc;
				e << "label(" << rc.x1 << ", " << rc.y1 << ", " << rc.width() << ", " << rc.height()
					<< ", " << "\"" << getstr(temp, wl->rstr) << "\""
					<< "); // " << grsname(wl->font) << "\n";
				break;
			default:
				e << "rectb({" << rc.x1 << ", " << rc.y1 << ", " << rc.x2 << ", " << rc.y2 << "}, colors::white"
					<< ");\n";
				break;
			}
		}
		e << "}\n\n";
	}
	delete pb;
}

static int get_frame_count_by_cicles(bam* source, short unsigned* temp, int cicle_start, int cicle_count) {
	int count = 0;
	short unsigned* pfi = (short unsigned*)source->offs(source->frames_lookup_offset);
	for(int i = cicle_start; i < cicle_start + cicle_count; i++) {
		bam::cicle* p = source->gcicle(i);
		for(int c = 0; c < p->count; c++) {
			int index = -1;
			short unsigned value = pfi[p->start + c];
			for(int j = 0; j < count; j++) {
				if(temp[j] == value) {
					index = j;
					break;
				}
			}
			if(index == -1)
				temp[count++] = value;
		}
	}
	return count;
}

static int get_cicle_indexes_count(bam* source, int cicle_start, int cicle_count, unsigned short& index_source) {
	int count = 0;
	index_source = 0;
	for(int i = cicle_start; i < cicle_start + cicle_count; i++) {
		bam::cicle* p = source->gcicle(i);
		count = imax(p->start + p->count, count);
		index_source = imin(p->start, index_source);
	}
	return count;
}

static pattern animation_character[] = {{"G11", 0 * 9, 9}, // Move
{"G17", 7 * 9, 9}, // Stand quite
{"G18", 8 * 9, 9}, // Stand quite and relax
{"G12", 2 * 9, 9}, // Stand quite and look around
{"G1", 1 * 9, 9}, // Combat stance
{"G13", 3 * 9, 9}, // Cobat Stance (Two handed)
{"G15", 4 * 9, 9}, // Get hit
{"G15", 5 * 9, 9}, // Get hit and drop down
{"G16", 6 * 9, 9}, // Agony and dead
{"G19", 9 * 9, 9}, // Get Up
{"A1", 0, 9},
{"A3", 0, 9},
{"A5", 0, 9},
{"A2", 0, 9},
{"A4", 0, 9},
{"A6", 0, 9},
{"A7", 0, 9},
{"A8", 0, 9},
{"A9", 0, 9},
{"SA", 0, 9},
{"SX", 0, 9},
{"SS", 0, 9},
{"CA"}, // Eight cicles of casting
{0}
};

static void initialize_animation(const char* name, pattern* p0, int& frame_count, int& cicle_count, int& index_count) {
	unsigned short frame_indexes[4096];
	frame_count = 0;
	cicle_count = 0;
	index_count = 0;
	for(pattern* p = p0; p->name; p++) {
		bam* source = 0;
		for(pattern* p1 = p0; p1 < p; p1++) {
			if(!p1->source)
				continue;
			if(strcmp(p1->name, p->name) == 0) {
				source = p1->source;
				break;
			}
		}
		if(!source) {
			int file_size;
			char nam1[260];
			zcpy(nam1, name);
			zcat(nam1, p->name);
			source = (bam*)loadr(nam1, file_size);
		}
		p->source = source;
		if(source) {
			if(!p->source_cicle_count)
				p->source_cicle_count = source->cicles;
			p->frame_start = frame_count;
			p->frame_count = get_frame_count_by_cicles(source, frame_indexes, p->source_cicle_start, p->source_cicle_count);
			p->cicle_start = cicle_count;
			p->cicle_count = p->source_cicle_count;
			p->index_start = index_count;
			p->index_count = get_cicle_indexes_count(source, p->source_cicle_start, p->source_cicle_count, p->index_source);
		} else {
			p->frame_start = 0;
			p->frame_count = 0;
			p->cicle_start = 0;
			p->cicle_count = 0;
			p->index_start = 0;
			p->index_count = 0;
		}
		frame_count += p->frame_count;
		cicle_count += p->cicle_count;
		index_count += p->index_count;
	}
}

static void cleanup_animation(pattern* p0) {
	for(pattern* p = p0; p->name; p++) {
		bam* source = p->source;
		if(!source)
			continue;
		for(pattern* p1 = p0; p1->name; p1++) {
			if(p1->source == source) {
				p1->source = 0;
				p1->frame_start = 0;
				p1->frame_count = 0;
				p1->cicle_start = 0;
				p1->cicle_count = 0;
				p1->index_start = 0;
				p1->index_count = 0;
				p1->index_source = 0;
			}
		}
		delete source;
	}
}

static short unsigned get_frame(short unsigned v, short unsigned* p, int count) {
	for(int i = 0; i < count; i++) {
		if(p[i] == v)
			return i;
	}
	return 0;
}

static void convert_animation(const char* name, pattern* p0, const char* folder = "pma") {
	int frame_count, cicle_count, index_count;
	initialize_animation(name, p0, frame_count, cicle_count, index_count);
	if(!p0->source) {
		cleanup_animation(p0);
		return;
	}
	auto source = create_sprite(frame_count, 1, 1000000 * 32,
		cicle_count,
		index_count);
	short unsigned* dci = (short unsigned*)((sprite::cicle*)source->offs(source->cicles_offset) + source->cicles);
	auto dc = (sprite::cicle*)source->getcicle(0);
	for(pattern* p = p0; p->name; p++) {
		if(!p->source)
			continue;
		short unsigned frame_indexes[2048];
		int frame_indexes_count = get_frame_count_by_cicles(p->source, frame_indexes, p->source_cicle_start, p->source_cicle_count);
		// Frames (auto increment)
		for(int i = 0; i < frame_indexes_count; i++)
			add_bam(source, p->source, frame_indexes[i]);
		// Cicles
		short unsigned* pci = (short unsigned*)p->source->offs(p->source->frames_lookup_offset);
		for(int i = 0; i < p->source_cicle_count; i++) {
			bam::cicle* pc = p->source->gcicle(i + p->source_cicle_start);
			dc[p->cicle_start + i].start = p->index_start + (pc->start - p->index_source);
			dc[p->cicle_start + i].count = pc->count;
			for(int j = 0; j < pc->count; j++)
				dci[p->index_start + (pc->start - p->index_source) + j] = p->frame_start + get_frame(pci[pc->start + j], frame_indexes, frame_indexes_count);
		}
	}
	delete_sprite(source, name, folder);
	cleanup_animation(p0);
}

static void convert_character(const char* url) {
	char temp[16];
	zcpy(temp, szfname(url));
	((char*)szext(temp))[-3] = 0;
	if(strcmp(temp, "CMNK1") == 0
		|| strcmp(temp, "CSHD") == 0)
		return;
	// 
	const char* pn = zend(temp) - 2;
	if(strcmp(pn, "T2") == 0
		|| strcmp(pn, "T3") == 0)
		return;
	convert_animation(temp, animation_character, "characters");
}

static void convert_bam_pma_animated(const char* su, const char* folder = "pma") {
	static pattern p[] = {
		{""},
	{0}
	};
	char temp[260];
	convert_animation(szfnamewe(temp, su), p, folder);
}

static void convert_bam_res(const char* su, const char* folder) {
	char rname[10];
	char result[260];
	zcpy(rname, su, 8);
	zcpy(result, path_to_dest);
	zcat(result, "/");
	zcat(result, folder);
	zcat(result, "/");
	zcat(result, rname);
	zcat(result, ".pma");
	if(io::file::exist(result))
		return;
	convert_bam_pma_animated(rname, folder);
}

static void convert_animation(const char* prefix, const char* name, pattern* data) {
	char temp[9];
	zcpy(temp, prefix);
	zcat(temp, name);
	convert_animation(temp, data);
}

static void convert_weapons() {
	static pattern w1h[] = {{"G1", 0 * 9, 9}, // Move
	{"G1", 7 * 9, 9}, // Stand quite
	{"G1", 8 * 9, 9}, // Stand quite and relax
	{"G1", 2 * 9, 9}, // Stand quite and look around
	{"G1", 1 * 9, 9}, // Combat stance
	{"G1", 3 * 9, 9}, // Cobat Stance (Two handed)
	{"G1", 4 * 9, 9}, // Get hit
	{"G1", 5 * 9, 9}, // Get hit and drop down
	{"G1", 6 * 9, 9}, // Agony and dead
	{"G1", 9 * 9, 9}, // Get Up
	{"A1"}, {"A3"}, {"A5"},
	{"OA7"}, {"OA8"}, {"OA9"},
	{"A7"}, {"A8"}, {"A9"},
	{0}
	};
	static pattern w2h[] = {{"G1", 0 * 9, 9}, // Move
	{"G1", 7 * 9, 9}, // Stand quite
	{"G1", 8 * 9, 9}, // Stand quite and relax
	{"G1", 2 * 9, 9}, // Stand quite and look around
	{"G1", 1 * 9, 9}, // Combat stance
	{"G1", 3 * 9, 9}, // Cobat Stance (Two handed)
	{"G1", 4 * 9, 9}, // Get hit
	{"G1", 5 * 9, 9}, // Get hit and drop down
	{"G1", 6 * 9, 9}, // Agony and dead
	{"G1", 9 * 9, 9}, // Get Up
	{"A2"}, {"A4"}, {"A6"},
	{"A2"}, {"A4"}, {"A6"},
	{"A2"}, {"A4"}, {"A6"},
	{0}
	};
	static pattern bow[] = {{"G1", 0 * 9, 9}, // Move
	{"G1", 7 * 9, 9}, // Stand quite
	{"G1", 8 * 9, 9}, // Stand quite and relax
	{"G1", 2 * 9, 9}, // Stand quite and look around
	{"G1", 1 * 9, 9}, // Combat stance
	{"G1", 3 * 9, 9}, // Cobat Stance (Two handed)
	{"G1", 4 * 9, 9}, // Get hit
	{"G1", 5 * 9, 9}, // Get hit and drop down
	{"G1", 6 * 9, 9}, // Agony and dead
	{"G1", 9 * 9, 9}, // Get Up
	{"SA"}, {"SA"}, {"SA"},
	{"SA"}, {"SA"}, {"SA"},
	{"SA"}, {"SA"}, {"SA"},
	{"SA"}, {"SA"}, {"SA"},
	{0}
	};
	static pattern xbow[] = {{"G1", 0 * 9, 9}, // Move
	{"G1", 7 * 9, 9}, // Stand quite
	{"G1", 8 * 9, 9}, // Stand quite and relax
	{"G1", 2 * 9, 9}, // Stand quite and look around
	{"G1", 1 * 9, 9}, // Combat stance
	{"G1", 3 * 9, 9}, // Cobat Stance (Two handed)
	{"G1", 4 * 9, 9}, // Get hit
	{"G1", 5 * 9, 9}, // Get hit and drop down
	{"G1", 6 * 9, 9}, // Agony and dead
	{"G1", 9 * 9, 9}, // Get Up
	{"SX"}, {"SX"}, {"SX"},
	{"SX"}, {"SX"}, {"SX"},
	{"SX"}, {"SX"}, {"SX"},
	{"SX"}, {"SX"}, {"SX"},
	{0}
	};
	static pattern throwing[] = {{"G1", 0 * 9, 9}, // Move
	{"G1", 7 * 9, 9}, // Stand quite
	{"G1", 8 * 9, 9}, // Stand quite and relax
	{"G1", 2 * 9, 9}, // Stand quite and look around
	{"G1", 1 * 9, 9}, // Combat stance
	{"G1", 3 * 9, 9}, // Cobat Stance (Two handed)
	{"G1", 4 * 9, 9}, // Get hit
	{"G1", 5 * 9, 9}, // Get hit and drop down
	{"G1", 6 * 9, 9}, // Agony and dead
	{"G1", 9 * 9, 9}, // Get Up
	{"SS"}, {"SS"}, {"SS"},
	{"SS"}, {"SS"}, {"SS"},
	{"SS"}, {"SS"}, {"SS"},
	{"SS"}, {"SS"}, {"SS"},
	{0}
	};
	static pattern shield[] = {{"G1", 0 * 9, 9}, // Move
	{"G1", 7 * 9, 9}, // Stand quite
	{"G1", 8 * 9, 9}, // Stand quite and relax
	{"G1", 2 * 9, 9}, // Stand quite and look around
	{"G1", 1 * 9, 9}, // Combat stance
	{"G1", 3 * 9, 9}, // Cobat Stance (Two handed)
	{"G1", 4 * 9, 9}, // Get hit
	{"G1", 5 * 9, 9}, // Get hit and drop down
	{"G1", 6 * 9, 9}, // Agony and dead
	{"G1", 9 * 9, 9}, // Get Up
	{"A1"}, {"A3"}, {"A5"},
	{"A1"}, {"A3"}, {"A5"},
	{"A1"}, {"A3"}, {"A5"},
	{"SS"}, {"SS"}, {"SS"},
	{0}
	};
	static pattern helm[] = {{"G1", 0 * 9, 9}, // Move
	{"G1", 7 * 9, 9}, // Stand quite
	{"G1", 8 * 9, 9}, // Stand quite and relax
	{"G1", 2 * 9, 9}, // Stand quite and look around
	{"G1", 1 * 9, 9}, // Combat stance
	{"G1", 3 * 9, 9}, // Cobat Stance (Two handed)
	{"G1", 4 * 9, 9}, // Get hit
	{"G1", 5 * 9, 9}, // Get hit and drop down
	{"G1", 6 * 9, 9}, // Agony and dead
	{"G1", 9 * 9, 9}, // Get Up
	{"A1"}, {"A3"}, {"A5"},
	{"A2"}, {"A4"}, {"A6"},
	{"A7"}, {"A8"}, {"A9"},
	{"SA"}, {"SX"}, {"SS"},
	{"CA"},
	{0}
	};
	static pattern_block source[] = {{"AX", w1h}, // топор
	{"BW", bow}, // лук
	{"CB", xbow}, // арбалет
	//{"CL", w1h}, // булава, дубина (дубль MC?)
	{"D1", shield}, // щиты
	{"D2", shield}, // щиты
	{"D3", shield}, // щиты (большой)
	{"D4", shield}, // щиты (башенный)
	{"DD", w1h}, // кинжал
	{"FL", w1h}, // цеп
	//{"FS", w1h}, // огненный меч?
	{"H0", helm}, // викингский шлем ( с прорезями для глаз )
	{"H1", helm}, // рогатый шлем ( рога торчат в право и лево )
	{"H2", helm}, // экзотический шлем с рогами как у летучей мыши
	{"H3", helm}, // рогатый шлем с закрученными рогами
	{"H4", helm}, // шлем с торчащей конским хвостом сверху
	{"H5", helm}, // шлем наподобии римского с ирокезом
	{"HB", w2h}, // огромный топор, алебарда
	{"MC", w1h}, // булава
	//{"MS", w1h}, // утренняя звезда (дубль FL?)
	{"QS", w2h}, // посох
	{"S1", w1h}, // меч
	{"S2", w2h}, // меч двуручный
	{"S3", w1h}, // меч как катана
	{"SC", w1h}, // скимитар
	{"SL", throwing}, // праща
	{"SP", w2h}, // копье
	{"SS", w1h}, // короткий меч
	{"WH", w1h}, // боевой молот
	};
	// Source files
	for(auto& e : source) {
		convert_animation("WQS", e.name, e.data);
		convert_animation("WQN", e.name, e.data);
		convert_animation("WQM", e.name, e.data);
		convert_animation("WQL", e.name, e.data);
	}
	// Update resource names
	char temp[260];
	io::file file(rsurl(temp, "util_weapons", "cpp"), StreamWrite | StreamText);
	file << "enum util_weapons_tags {\n";
	for(auto& e : source) {
		file << "WQS" << e.name << ", ";
		file << "WQN" << e.name << ", ";
		file << "WQM" << e.name << ", ";
		file << "WQL" << e.name << ",\n";
	}
	file << "};\n";
	file << "static const char* util_weapons[2] = {\n";
	for(auto& e : source) {
		file << "{\"" << "WQS" << e.name << "\"}, ";
		file << "{\"" << "WQN" << e.name << "\"}, ";
		file << "{\"" << "WQM" << e.name << "\"}, ";
		file << "{\"" << "WQL" << e.name << "\"},\n";
	}
	file << "};";
}

static void convert_portrait(const char* output_name, const char* pattern, bool large) {
	char temp[260];
	char name[260];
	auto col = portrait_info::getelements();
	auto count = col.count;
	sprite* source = create_sprite(count, 0, 1000000 * 32);
	if(large)
		add_bmp(source, rsurlo(temp, "NOPORTLG", "bmp"));
	else
		add_bmp(source, rsurlo(temp, "NOPORTSM", "bmp"));
	for(unsigned i = 1; i < count; i++) {
		const char* n = col[i].name;
		szprints(name, zendof(name), pattern, n);
		add_bmp(source, rsurlo(temp, name, "bmp"));
	}
	delete_sprite(source, output_name);
}

static void convert_files(const char* name, void(*cp)(const char* su)) {
	char temp[261];
	zcpy(temp, path_to_dest_original);
	zcat(temp, "/");
	zcat(temp, name);
	for(io::file::find e(temp); e; e.next()) {
		char source[261];
		if(e.name()[0] == '.')
			continue;
		e.fullname(source);
		cp(source);
	}
}

static void convert_files(const char* name, const char* pattern, void(*cp)(const char* su)) {
	char temp[261];
	zcpy(temp, path_to_dest_original);
	zcat(temp, "/");
	zcat(temp, name);
	for(io::file::find e(temp); e; e.next()) {
		char source[261];
		if(e.name()[0] == '.')
			continue;
		if(!szpmatch(e.name(), pattern))
			continue;
		e.fullname(source);
		cp(source);
	}
}

static void convert_ground() {
	sprite* source = create_sprite(54);
	add_bam(source, "GBELT01");
	add_bam(source, "GBLUN01"); // spiked club
	add_bam(source, "GBLUN06"); // mace
	add_bam(source, "GBOLT01");
	add_bam(source, "GBOOK01");
	add_bam(source, "GBOOT01");
	add_bam(source, "GBOW01");
	add_bam(source, "GRAC01");
	add_bam(source, "GRAC06"); // gauntlets
	add_bam(source, "GBULL01");
	add_bam(source, "GCHAN01");
	add_bam(source, "GCLCK01");
	add_bam(source, "GCLCK02"); // robe
	add_bam(source, "GDAGG01");
	add_bam(source, "GDART01");
	add_bam(source, "GGEM01");
	add_bam(source, "GHALB01");
	add_bam(source, "GHAMM01");
	add_bam(source, "GHELM01");
	add_bam(source, "GHIDEBEE");
	add_bam(source, "GHIDEUMB");
	add_bam(source, "GLEAT01");
	add_bam(source, "GMISC01"); // wolf's fur
	add_bam(source, "GMISC12"); // ankheg's mandibul's
	add_bam(source, "GMISC13"); // pickable male body
	add_bam(source, "GMISC54"); // pickable girl
	add_bam(source, "GMISC55"); // pickable man
	add_bam(source, "GMISC62"); // pickable cat
	add_bam(source, "GMISC65"); // pickable man
	add_bam(source, "GMISC66"); // pickable noble
	add_bam(source, "GMISC68"); // pickable udressed woman
	add_bam(source, "GMISC83"); // key
	add_bam(source, "GMISC85"); // journal
	add_bam(source, "GMISC86"); // hair skin
	add_bam(source, "GMISC86"); // hair skin
	add_bam(source, "GPLAT01"); // plate armour
	add_bam(source, "GPOTN01");
	add_bam(source, "GRING01");
	add_bam(source, "GSACK01");
	add_bam(source, "GSCRL01");
	add_bam(source, "GSHLD01");
	add_bam(source, "GSHLD03");
	add_bam(source, "GSHLD05");
	add_bam(source, "GSHLD08");
	add_bam(source, "GSPER01");
	add_bam(source, "GSTAF01");
	add_bam(source, "GSW1H01"); // long sword
	add_bam(source, "GSW1H07"); // short sword
	add_bam(source, "GSW2H01");
	add_bam(source, "GTREAS01");
	add_bam(source, "GTREAS02");
	add_bam(source, "GWAND01");
	add_bam(source, "GXBOW01");
	add_bam(source, "GYETI"); // yeti skin
	delete_sprite(source, "GROUND");
}

static void convert_items() {
	sprite* source = create_sprite(935 * 2);
	add_bam_files(source, "IAMUL*");
	add_bam_files(source, "IARM*");
	add_bam_files(source, "IARROW*");
	add_bam_files(source, "IAX1H*");
	add_bam_files(source, "IAX2H*");
	add_bam_files(source, "IBELT*");
	add_bam_files(source, "IBLUN*");
	add_bam_files(source, "IBOLT*");
	add_bam_files(source, "IBOOK*");
	add_bam_files(source, "IBOOT*");
	add_bam_files(source, "IBOW*");
	add_bam_files(source, "IBRAC*");
	add_bam_files(source, "IBULL*");
	add_bam_files(source, "ICLCK*");
	add_bam_files(source, "ICLUB*");
	add_bam_files(source, "IDAGG*");
	add_bam_files(source, "IDAGT*");
	add_bam_files(source, "IDART*");
	add_bam_files(source, "IFLAIL*");
	add_bam_files(source, "IGEN*");
	add_bam_files(source, "IHALB*");
	add_bam_files(source, "IHAMM*");
	add_bam_files(source, "IHAMT*");
	add_bam_files(source, "IHELM*");
	add_bam_files(source, "IHIDE*");
	add_bam_files(source, "IKEY*");
	add_bam_files(source, "IMACE*");
	add_bam_files(source, "IMISC*");
	add_bam_files(source, "IMSTR*");
	add_bam_files(source, "IPIECE*");
	add_bam_files(source, "IPOTN*");
	add_bam_files(source, "IRING*");
	add_bam_files(source, "ISHD*");
	add_bam_files(source, "ISLNG*");
	add_bam_files(source, "ISPER*");
	add_bam_files(source, "ISTAF*");
	add_bam_files(source, "ISWD*");
	add_bam_files(source, "IWAND*");
	add_bam_files(source, "IXBOW*");
	delete_sprite(source, "ITEMS");
}

static void convert_items_new() {
	char temp[260];
	sprite* source = create_sprite((LastItem + 1) * 2, LastItem + 1);
	for(int i = 0; i <= LastItem; i++)
		add_bam(source, rsurlo(temp, item::getfname(i)));
	delete_sprite(source, "ITEMS");
}

static void add_files_index(io::stream& file, const char* type, const char* pattern) {
	char temp[260];
	zcpy(temp, path_to_dest_original);
	zcat(temp, "/");
	zcat(temp, type);
	int count = 0;
	const int brline = 5;
	file << "\n// " << pattern << "\n";
	for(io::file::find e(temp); e; e.next()) {
		if(e.name()[0] == '.')
			continue;
		if(!szpmatch(e.name(), pattern))
			continue;
		char temp[260];
		zcpy(temp, e.name());
		char* ext = (char*)szext(temp);
		if(ext)
			ext[-1] = 0;
		file << "\"" << temp << "\", ";
		count++;
		if((count%brline) == 0)
			file << "\n";
	}
}

static void convert_scrolls() {
	sprite* source = create_sprite(400 * 2);
	add_bam_files(source, "SPPR*A", 0, 2);
	add_bam_files(source, "SPWI*A", 0, 2);
	add_bam_files(source, "SPIN*A", 0, 2);
	delete_sprite(source, "SCROLLS");
}

static void convert_form() {
	sprite* source = create_sprite(32);
	add_bam_files(source, "FORM*");
	delete_sprite(source, "FORM");
}

static void convert_spells_avatars() {
	char temp[260];
	sprite* source = create_sprite(400);
	add_bam_files(source, "SPPR*B", 0);
	add_bam_files(source, "SPWI*B", 0);
	add_bam_files(source, "SPIN*B", 0);
	delete_sprite(source, "SPELLS");
	io::file e(rsurl(temp, "util_spells_index", "cpp"), StreamWrite | StreamText);
	e << "static const char* util_spells_index[] = {";
	add_files_index(e, "bam", "SPPR*B");
	add_files_index(e, "bam", "SPWI*B");
	add_files_index(e, "bam", "SPIN*B");
	e << "0};";
}

static void convert_stonslot() {
	sprite* source = create_sprite(18);
	add_bam_files(source, "STONHELM", 0);
	add_bam_files(source, "STONAMUL", 0);
	add_bam_files(source, "STONARM", 0);
	add_bam_files(source, "STONCLOK", 0);
	add_bam_files(source, "STONRING", 0);
	add_bam_files(source, "STONRING", 0);
	add_bam_files(source, "STONGLET", 0);
	add_bam_files(source, "STONBELT", 0);
	add_bam_files(source, "STONBOOT", 0);
	add_bam_files(source, "STONFORM", 0);
	add_bam_files(source, "STONITEM", 0);
	add_bam_files(source, "STONQUIV", 0);
	add_bam_files(source, "STONRING", 0);
	add_bam_files(source, "STONSHIL", 0);
	add_bam_files(source, "STONSONG", 0);
	add_bam_files(source, "STONSPEC", 0);
	add_bam_files(source, "STONSPEL", 0);
	add_bam_files(source, "STONWEAP", 0);
	delete_sprite(source, "STON");
}

static void convert_mos() {
	create_mos("LOAD", "LOAD*");
	create_mos("WMAP", "WMAP*");
	create_mos("GACTN", "GACTN*");
	create_mos("GCOMM", "GCOMM*");
	create_mos("GUICHP", "GUICHP*");
	create_mos("GUICGB", "GUICGB*");
	create_mos("GUIHELP", "GUIHELP.mos");
	create_mos("GUIINV", "GUIINV*");
	create_mos("START", "START*");
	create_mos("GOPTION", "GOP*");
	create_mos("STONB", "STON1*");
	create_mos("GUIA", "GUIA*");
	create_mos("GBTNPLUS", "GBTNPLUS");
	create_mos("GBTNMINS", "GBTNMINS");
	create_mos("GUISEX", "GUISEX");
	create_mos("GUIFEAT", "GUIFEAT*");
	create_mos("GUIHSB", "GUIHSB*");
	create_mos("GUISPL", "GUISPL*");
	create_mos("COLOR", "COLOR");
	create_mos("GUIREC", "GUIREC08");
	create_mos("GUIMAP", "GUIMAP*");
	create_mos("GUICNAME", "GUICNAME*");
	create_mos("GUIERR", "GUIERR*");
	create_mos("GUICARBB", "GUICARBB*");
	create_mos("GUIRLVL", "GUIRLVL*");
	create_mos("GCGPARTY", "GCGPARTY*");
	create_mos("GMPMCHRB", "GMPMCHRB*");
	create_mos("GUIJRLN", "GUIJRLN*");
	create_mos("GOPT", "GOP*");
	create_mos("GUIMOVB", "GUIMOVB*");
	create_mos("STONEOPT", "STONEOPT*");
	create_mos("GUISRSVB", "GUISRSVB*");
	create_mos("GUISRRQB", "GUISRRQB*");
}

static void convert_tis_pma(const char* rn, bif* pb, int ti) {
	if(ti)
		ti--;
	if(ti >= pb->tile_count)
		return;
	bif::tile* pbt = (bif::tile*)((bif::file*)pb->offs(pb->file_offset) + pb->file_count) + ti;
	sprite* source = create_sprite(pbt->count, pbt->count, 32 * 8000000);
	for(int i = 0; i < pbt->count; i++) {
		unsigned char* p = pb->offs(pbt->offset) + i * pbt->element_size;
		source->store(p + 256 * 4, 64, 64, 64, 0, 0, sprite::RAW8, 0, (color*)p);
	}
	delete_sprite(source, rn, "area");
}

static void convert_tis_pma(const char* su) {
	char rs[32];
	bif* pb = getbif(su);
	if(!pb)
		return;
	convert_tis_pma(szfnamewe(rs, su), pb, 0);
	delete pb;
}

static bool isexist(const char* su, const char* ext, const char* postfix) {
	char temp[260];
	return io::file::exist(rsurlo(temp, su, ext, postfix));
}

static void calculate_verticles_count(wed::polygon* p, int count, int& max) {
	for(int i = 0; i < count; i++)
		max = imax(p[i].verticles_start + p[i].verticles_count, max);
}

static int calculate_verticles_count(wed* pb) {
	wed::door* pbd = (wed::door*)pb->offs(pb->doors_offset);
	wed::sheader* pbs = (wed::sheader*)pb->offs(pb->secondary_header_offset);
	int max = 0;
	calculate_verticles_count((wed::polygon*)pb->offs(pbs->poligons_offset), pbs->poligons_count, max);
	for(int i = 0; i < pb->doors_count; i++) {
		if(pbd[i].polygons_open_count) {
			calculate_verticles_count((wed::polygon*)pb->offs(pbd[i].polygons_open_offset),
				pbd[i].polygons_open_count, max);
		}
		if(pbd[i].polygons_close_count) {
			calculate_verticles_count((wed::polygon*)pb->offs(pbd[i].polygons_close_offset),
				pbd[i].polygons_close_count, max);
		}
	}
	return max;
}

static are::door* find_door(are* ph, char resname[8]) {
	are::door* pd = (are::door*)ph->offs(ph->doors_offset);
	for(int i = 0; i < ph->doors_count; i++) {
		if(memcmp(pd[i].name_wed, resname, 8) == 0)
			return pd + i;
	}
	return 0;
}

inline rect rsr(rects rc) {
	return{rc.x1, rc.y1, rc.x2, rc.y2};
}

static void store_bits(unsigned char* output, int output_scanline, int output_bpp,
	unsigned char* input, int input_scanline, int input_bpp,
	int width, int height) {
	int height_abs = iabs(height);
	for(int y = 0; y < height_abs; y++) {
		auto s = input + ((height < 0) ? y : (height_abs - y - 1))*input_scanline;
		auto p = output + y * output_scanline;
		for(auto x = 0; x < height; x++) {
			unsigned char b;
			switch(input_bpp) {
			case 4: b = (x & 1) ? s[x / 2] & 0x0F : s[x / 2] >> 4; break;
			case 8: b = s[x]; break;
			}
			p[x] = b;
		}
	}
}

static void read_bitmap(unsigned char* output, int output_scanline, int output_bpp, const char* name, const char* postfix, color* pal) {
	char temp[260];
	bmp_header* pb = (bmp_header*)loadb(rsurlo(temp, name, "bmp", postfix));
	bmp_info* pi = (bmp_info*)((char*)pb + sizeof(bmp_header));
	int input_bpp = pi->bpp;
	unsigned char* ppal = (unsigned char*)pi + sizeof(bmp_info);
	unsigned char* input = (unsigned char*)pb + pb->bits;
	store_bits(output, output_scanline, output_bpp,
		input, color::scanline(pi->width, input_bpp), input_bpp,
		pi->width, pi->height);
	if(pal)
		memcpy(pal, ppal, 256 * sizeof(color));
	if(pi->width != map::width || pi->height != map::height) {
		// TODO: Вывести сообщение об ошибке
	}
}

static void write_objects_end(io::stream& file) {
	file.write(0);
}

static void write_rewrite(io::stream& f, int offset, int count) {
	auto n = f.seek(offset, SeekSet);
	f.write(count);
	f.seek(n, SeekSet);
}

bool archive_ard(io::stream& file, bool writemode, char* sprites_resname);

static void convert_wed(const char* su) {
	char temp[4096];
	char rsnm[260];
	int file_size;
	if(!isexist(su, "bmp", "LM") || !isexist(su, "bmp", "HT") || !isexist(su, "bmp", "SR"))
		return;
	wed* pb = (wed*)loadr(su, file_size, "wed");
	if(!pb)
		return;
	are* pba = (are*)loadr(rsurlo(temp, szfnamewe(rsnm, su), "are"), file_size, "are");
	if(!pba) {
		delete pb;
		return;
	}
	wed::sheader* pbs = (wed::sheader*)pb->offs(pb->secondary_header_offset);
	wed::door* pbd = (wed::door*)pb->offs(pb->doors_offset);
	unsigned short* door_index_lookup = (unsigned short*)pb->offs(pb->doors_tile_cells_indices_offset);
	wed::overlay* po = (wed::overlay*)pb->offs(pb->overlays_offset);
	unsigned short* tile_index_lookup = (unsigned short*)pb->offs(po->tile_index_lookup_offset);
	map::clear();
	map::width = po->width * 4;
	map::height = ((po->height * 4) * 16 + 11) / 12;
	// Тайловая карта (Ш*В*2 байта)
	for(int y = 0; y < po->height; y++) {
		for(int x = 0; x < po->width; x++) {
			int index = y * po->width + x;
			wed::tilemap* pm = (wed::tilemap*)pb->offs(po->tilemap_offset) + index;
			map::settile(y * 64 + x, tile_index_lookup[pm->start]);
		}
	}
	// Карта высот
	// Карта теней
	read_bitmap(map::ptr("LM"), 256, 8, su, "LM", map::getpallette());
	// Карта поиска
	read_bitmap(map::ptr("SR"), 256, 8, su, "SR", 0);
	// Точки вершин
	verticles.count = pba->verticles_count;
	memcpy(verticles.data, pba->offs(pba->verticles_offset), sizeof(point)*pba->verticles_count);
	// Regions
	for(int i = 0; i < pba->regions_count; i++) {
		int sz = sizeof(are::region);
		are::region* pc = (are::region*)pba->offs(pba->regions_offset + i * sizeof(are::region));
		auto& pm = *region_data.add();
		temp[0] = 0;
		if(pc->info_strref != -1)
			getstr(temp, pc->info_strref);
		pm.name = szdup(temp);
		pm.type = (region_type_s)(pc->type & 0x3);
		memcpy(&pm.box, &rsr(pc->box), sizeof(pm.box));
		pm.launch = pc->launch;
		memcpy(pm.move_to_area, pc->destination_name, sizeof(pm.move_to_area));
		memcpy(pm.move_to_entrance, pc->entrance_name, sizeof(pm.move_to_area));
		pm.points.data = verticles.data + pc->verticles_start;
		pm.points.count = pc->verticles_count;
	}
	// Doors
	for(int i = 0; i < pb->doors_count; i++) {
		auto current_index = door_tiles_data.count;
		for(int j = pbd[i].start; j < pbd[i].start + pbd[i].count; j++) {
			short unsigned index = door_index_lookup[j];
			wed::tilemap* pc = (wed::tilemap*)pb->offs(po->tilemap_offset) + index;
			auto pt = door_tiles_data.add();
			pt->index = (pc->start / po[0].width) * 64 + (pc->start%po[0].width);
			pt->open = pc->start == 0xFFFF ? pt->index : pc->start;
			pt->closed = pc->start_alternate == 0xFFFF ? pt->open : pc->start_alternate;
		}
		auto& pm = *door_data.add();
		pm.clear();
		pm.tiles.data = door_tiles_data.data + current_index;
		pm.tiles.count = door_tiles_data.count - current_index;
		are::door* pad = find_door(pba, pbd[i].name);
		if(pad) {
			int flags = 0;
			if(pad->flags & 1)
				pm.opened = true;
			if(pad->flags & 2)
				pm.locked = true;
			pm.cursor = pad->cursor_index;
			pm.launch = pad->launch;
			pm.box = rsr(pad->close_rect);
			pm.points[0] = pad->points[0];
			pm.points[1] = pad->points[1];
			pm.open_points.data = verticles.data + pad->open_verticles_start;
			pm.open_points.count = pad->open_verticles_count;
			pm.close_points.data = verticles.data + pad->close_verticles_start;
			pm.close_points.count = pad->close_verticles_count;
		}
	}
	// Entrances
	for(int i = 0; i < pba->entrances_count; i++) {
		are::entrance* pc = (are::entrance*)pba->offs(pba->entrances_offset + i * sizeof(are::entrance));
		auto& pm = *entrance_data.add();
		memcpy(pm.name, pc->name, sizeof(pm.name));
		pm.orientation = (unsigned char)pc->orientation;
		pm.position = pc->pos;
	}
	// Containers
	for(int i = 0; i < pba->containers_count; i++) {
		are::conainer* pc = (are::conainer*)pba->offs(pba->containers_offset + i * sizeof(are::conainer));
		if(!pc->box)
			continue;
		auto& pm = *container_data.add();
		pm.type = pc->type;
		zcpy(pm.name, pc->name, sizeof(pm.name));
		pm.launch = pc->launch;
		pm.box = rsr(pc->box);
		pm.points.data = verticles.data + pc->verticles_start;
		pm.points.count = pc->verticles_count;
	}
	// Animations
	for(int i = 0; i < pba->animations_count; i++) {
		auto pc = (are::animation*)pba->offs(pba->animations_offset + i * sizeof(are::animation));
		auto& pm = *animation_data.add();
		zcpy(pm.rsname, pc->rsname, sizeof(pc->rsname));
		zcpy(pm.rsname_pallette, pc->rsname_pallette, sizeof(pc->rsname_pallette));
		pm.x = pc->position.x; pm.y = pc->position.y;
		pm.circle = pc->circle;
		pm.frame = pc->frame;
		pm.start_frame = pc->start_frame;
		pm.flags = pc->flags;
		pm.transparency = (unsigned char)pc->transparency;
		pm.skip_cycles = pc->skip_cycles;
		pm.schedule = pc->schedule;
		pm.chance_loop = pc->chance_loop;
		pm.height = pc->height;
		convert_bam_res(pm.rsname, "animations");
	}
	// Tiled objects
	for(int i = 0; i < pba->tiled_object_count; i++) {
		auto pc = (are::tile*)pba->offs(pba->tiled_object_offset + i * sizeof(are::tile));
		if(!pc->name[0])
			continue;
	}
	io::file file(rsurl(temp, szfnamewe(rsnm, su), "ard", "area"), StreamWrite);
	if(file)
		archive_ard(file, true, po->tileset);
	delete pb;
	delete pba;
}

static void convert_minimap_pma(const char* name) {
	sprite* p = create_sprite(1, 0);
	add_mos(p, name);
	if(use_cleanup)
		io::file::remove(name);
	char temp[260];
	szfnamewe(temp, name);
	zcat(temp, "MM");
	delete_sprite(p, temp, "area");
}

static void convert_wed_test() {
	struct counter : public fileop {
		int			count;
		short		max_width;
		short		max_height;
		int			max_overlay;
		counter() : count(0), max_width(0), max_height(0), max_overlay(0) {}
		void action(const char* su) override {
			int file_size;
			wed* pb = (wed*)loadr(su, file_size, "wed");
			if(!pb)
				return;
			int max_overlay = 0;
			for(int i = 0; i < pb->overlays_count; i++) {
				wed::overlay* po = (wed::overlay*)pb->offs(pb->overlays_offset) + i;
				if(po->width)
					max_overlay++;
				this->max_width = imax(po->width, this->max_width);
				this->max_height = imax(po->width, this->max_width);
				unsigned short* tile_index_lookup = (unsigned short*)pb->offs(po->tile_index_lookup_offset);
				for(int y = 0; y < po->height; y++) {
					for(int x = 0; x < po->height; x++) {
						int index = y * po->width + x;
						wed::tilemap* pm = (wed::tilemap*)pb->offs(po->tilemap_offset) + index;
						if(pm->count > 1)
							count++;
					}
				}
			}
			this->max_overlay = imax(max_overlay, this->max_overlay);
			delete pb;
		}
	};
	counter as;
	convert_files("wed", "*", as);
}

static char* textscreen(const char* s) {
	static char temp[256 * 4 * 16];
	char* d = temp;
	while(*s) {
		if(*s == '\n') {
			*d++ = '\\';
			*d++ = 'n';
		} else if(*s == '\r') {
			// noting
		} else if(*s == '\"') {
			*d++ = '\\';
			*d++ = '\"';
		} else
			*d++ = *s;
		s++;
	}
	*d = 0;
	return temp;
}

static void convert_itm_cpp(const char* name, const char* pattern) {
	char		temp[260];
	itm::info	objects[2500];
	struct adder : public fileop {
		int			count;
		itm::info*	objects;
		adder() : count(0) {}
		void action(const char* su) override {
			char description[1024 * 32];
			int file_size;
			itm* pb = (itm*)loadr(su, file_size, "itm");
			if(!pb)
				return;
			if(pb->name_identified == -1)
				return;
			if(pb->description == -1 && pb->description_identified == -1)
				return;
			const char* fn = szfname(su);
			itm::info& e = objects[count++];
			e.name = szdup(textscreen(getstr(description, pb->name_identified)));
			e.descrition = szdup(textscreen(getstr(description, (pb->description_identified == -1) ? pb->description : pb->description_identified)));
			e.magic = pb->enchantment;
			e.price = pb->price;
			e.weight = pb->weight;
			zcpy(e.resref, pb->resicon, 8);
		}
	};
	memset(objects, 0, sizeof(objects));
	adder as;
	as.objects = objects;
	convert_files("itm", pattern, as);
	qsort(objects, as.count, sizeof(objects[0]), itm::info::compare);
	io::file file(rsurl(temp, name, "cpp"), StreamWrite | StreamText);
	file << "static item_description_test objects[] = {\n";
	for(int i = 0; i < as.count; i++) {
		itm::info e = objects[i];
		file << "{";
		file << "\"" << e.name << "\", ";
		file << "{\"" << e.resref << "\"}, ";
		file << e.magic << ", ";
		file << e.price << ", ";
		file << e.weight << ", ";
		file << "\"" << e.descrition << "\"";
		file << "},\n";
	}
	file << "};";
}

static void convert_cre_cpp(const char* name, const char* pattern) {
	char		temp[260];
	cre::info	objects[2500];
	struct adder : public fileop {
		int			count;
		cre::info*	objects;
		adder() : count(0) {}
		void action(const char* su) override {
			char description[1024 * 32];
			int file_size;
			cre* pb = (cre*)loadr(su, file_size, "cre");
			if(!pb)
				return;
			if(pb->name_long == -1)
				return;
			cre::info& e = objects[count++];
			e.name = szdup(textscreen(getstr(description, pb->name_long)));
			e.minor = pb->color_minor;
			e.major = pb->color_major;
			e.skin = pb->color_skin;
			e.hair = pb->color_hair;
			e.armor = pb->color_armor;
			e.metal = pb->color_metal;
			e.leather = pb->color_leather;
		}
	};
	memset(objects, 0, sizeof(objects));
	adder as;
	as.objects = objects;
	convert_files("cre", pattern, as);
	qsort(objects, as.count, sizeof(objects[0]), cre::info::compare);
	io::file file(rsurl(temp, name, "cpp"), StreamWrite | StreamText);
	file << "static creature_description_test objects[] = {\n";
	for(int i = 0; i < as.count; i++) {
		cre::info e = objects[i];
		file << "{";
		file << "\"" << e.name << "\", ";
		file << (int)e.minor << ", ";
		file << (int)e.major << ", ";
		file << (int)e.skin << ", ";
		file << (int)e.hair << ", ";
		file << (int)e.armor << ", ";
		file << (int)e.metal << ", ";
		file << (int)e.leather;
		file << "},\n";
	}
	file << "};";
}

static void convert_gui() {
	convert_bam_pma("CURSORS");
	convert_bam_pma_animated("CURSARW");
	convert_bam_pma("CGEAR");
	convert_files(gext(TypeBam), "GBTN*.bam", convert_bam_pma_no_shadow);
	convert_files(gext(TypeBam), "GUI*.bam", convert_bam_pma_no_shadow);
	convert_files(gext(TypeBam), "INVBUT*.bam", convert_bam_pma_no_shadow);
	convert_files(gext(TypeBam), "INVBUT*.bam", convert_bam_pma_no_shadow);
	convert_bam_pma("COLGRAD");
	convert_bam_pma("GUIRZPOR");
	convert_bam_pma("GUIRSPOR");
	convert_bam_pma_no_shadow("GBTNRECB");
	convert_bam_pma_no_shadow("SPLBUT");
	convert_bam_pma_no_shadow("GACTNBTN");
	convert_bam_pma_no_shadow("STONSLOT");
	convert_bam_pma_no_shadow("GCOMMBTN");
	convert_bam_pma_no_shadow("GBTNLRG2");
	convert_bam_pma("LOCATER");
	convert_stonslot();
}

static void convert_map_files() {
	convert_files("wed", convert_tis_pma);
	convert_files("mos", "AR*.mos", convert_minimap_pma);
	convert_files("wed", convert_wed);
}

static void convert_container() {
	sprite* source = create_sprite(16);
	add_bam_files(source, "CONT*", 0);
	delete_sprite(source, "CONTAINER");
}

static void unpack_type(int type, const char* rsname = 0) {
	for(int i = 0; i < int(key->res_count); i++) {
		auto rs = key->gr(i);
		if(rs.type != type)
			continue;
		char resource[64];
		int bi = key->rbif(rs.locator);
		int fi = key->rfil(rs.locator);
		int ti = key->rtil(rs.locator);
		const char* kn = key->name(bi);
		key->rname(resource, i);
		if(rsname && strcmp(rsname, resource) != 0)
			continue;
		bif* p = getbif(bi);
		if(!p)
			continue;
		if(type == TypeTis)
			convert_tis_pma(resource, p, ti);
		else
			p->write(fi, resource);
	}
}

static void convert_portrait() {
	convert_portrait("PORTL", "%1_L", true);
	convert_portrait("PORTS", "%1_S", false);
}

static void convert_all_wed() {
	convert_files("wed", convert_wed);
}

static void convert_2da_plain(const char* url) {
	int file_size;
	const char* p = (const char*)loadr(url, file_size, "2da");
	const char* ph = get_header(p);
	delete p;
}

static void convert_prt_cpp(const char* name, int type) {
	unsigned char temp[64];
	char path[260];
	char rsnm[260];
	//get_colors_to_choose(temp, type);
	io::file e(rsurl(path, szfnamewe(rsnm, name), "cpp"), StreamWrite | StreamText);
	e << "static unsigned char " << name << "[];\n{";
	for(int i = 0; temp[i] != 0xFF; i++)
		e << (int)temp[i] << ", ";
	e << "0xFF}\n\n";
}

static void make_all_stuff(bool include_debug = false) {
	if(include_debug)
		unpack_type(TypeChu);
	unpack_type(TypeMos);
	unpack_type(TypeBmp);
	unpack_type(TypeCre);
	unpack_type(TypeBam);
	unpack_type(TypeTis);
	unpack_type(TypeWed);
	unpack_type(TypeAre);
	if(include_debug) {
		convert_files("chu", convert_chu_cpp);
		convert_files("mos", convert_mos_bmp);
		convert_cre_cpp("creatures", "*.cre");
	}
	convert_mos();
	convert_items_new();
	convert_ground();
	convert_gui();
	convert_scrolls();
	convert_spells_avatars();
	convert_portrait();
	convert_weapons();
	convert_files("bam", "C*A1.bam", convert_character);
	convert_form();
	convert_container();
	convert_all_wed();
}

int util_main() {
	load_key();
	if(!key)
		return -1;
	unpack_type(TypeBam);
	convert_files("bam", "C*A1.bam", convert_character);
	//make_all_stuff();
	//convert_all_wed();
	//convert_wed("AR1100");
	//convert_wed("AR1107");
	return 0;
}