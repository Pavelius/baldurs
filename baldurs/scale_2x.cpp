#define SSDST(bits, num) (unsigned*)dst##num
#define SSSRC(bits, num) (const unsigned*)src##num
#define SCDST(i) (dst+(i)*dst_slice)
#define SCSRC(i) (src+(i)*src_slice)
#define SCMID(i) (mid[(i)])

static inline void scale2x_32_def(unsigned* dst0, unsigned* dst1, const unsigned* src0, const unsigned* src1, const unsigned* src2, unsigned count) {
	/* first pixel */
	if(src0[0] != src2[0] && src1[0] != src1[1]) {
		dst0[0] = src1[0] == src0[0] ? src0[0] : src1[0];
		dst0[1] = src1[1] == src0[0] ? src0[0] : src1[0];
		dst1[0] = src1[0] == src2[0] ? src2[0] : src1[0];
		dst1[1] = src1[1] == src2[0] ? src2[0] : src1[0];
	} else {
		dst0[0] = src1[0];
		dst0[1] = src1[0];
		dst1[0] = src1[0];
		dst1[1] = src1[0];
	}
	++src0;
	++src1;
	++src2;
	dst0 += 2;
	dst1 += 2;

	/* central pixels */
	count -= 2;
	while(count) {
		if(src0[0] != src2[0] && src1[-1] != src1[1]) {
			dst0[0] = src1[-1] == src0[0] ? src0[0] : src1[0];
			dst0[1] = src1[1] == src0[0] ? src0[0] : src1[0];
			dst1[0] = src1[-1] == src2[0] ? src2[0] : src1[0];
			dst1[1] = src1[1] == src2[0] ? src2[0] : src1[0];
		} else {
			dst0[0] = src1[0];
			dst0[1] = src1[0];
			dst1[0] = src1[0];
			dst1[1] = src1[0];
		}

		++src0;
		++src1;
		++src2;
		dst0 += 2;
		dst1 += 2;
		--count;
	}

	/* last pixel */
	if(src0[0] != src2[0] && src1[-1] != src1[0]) {
		dst0[0] = src1[-1] == src0[0] ? src0[0] : src1[0];
		dst0[1] = src1[0] == src0[0] ? src0[0] : src1[0];
		dst1[0] = src1[-1] == src2[0] ? src2[0] : src1[0];
		dst1[1] = src1[0] == src2[0] ? src2[0] : src1[0];
	} else {
		dst0[0] = src1[0];
		dst0[1] = src1[0];
		dst1[0] = src1[0];
		dst1[1] = src1[0];
	}
}

static inline void stage_scale2x(void* dst0, void* dst1, const void* src0, const void* src1, const void* src2, unsigned pixel_per_row) {
	scale2x_32_def(SSDST(32, 0), SSDST(32, 1), SSSRC(32, 0), SSSRC(32, 1), SSSRC(32, 2), pixel_per_row);
}

namespace draw {
void scale2x(void* void_dst, unsigned dst_slice, const void* void_src, unsigned src_slice, unsigned width, unsigned height) {
	unsigned char* dst = (unsigned char*)void_dst;
	const unsigned char* src = (const unsigned char*)void_src;
	unsigned count;
	count = height;
	stage_scale2x(SCDST(0), SCDST(1), SCSRC(0), SCSRC(0), SCSRC(1), width);
	dst = SCDST(2);
	count -= 2;
	while(count) {
		stage_scale2x(SCDST(0), SCDST(1), SCSRC(0), SCSRC(1), SCSRC(2), width);
		dst = SCDST(2);
		src = SCSRC(1);
		--count;
	}
	stage_scale2x(SCDST(0), SCDST(1), SCSRC(0), SCSRC(1), SCSRC(1), width);
}
}