#include "main.h"

using namespace draw;

adat<animation, 128> animation_data;

enum animation_flag_s {
	AnimationEnabled = 1,
	RenderBlackAsTransparent = 2,
	NonSelfIllumination = 4,
	PartialAnimation = 8,
	Synchronized = 16,
	UseRandomStartFrame = 32,
	WallNoHideAnimation = 64,
	DisableOnSlowMachine = 128,
	NotCover = 256,
	PlayAllFrames = 512,
	UsePalletteBitmap = 1024,
	Mirrored = 2048,
	ShowInCombat = 4096,
};

template<> void archive::set<animation>(animation& e) {
	set(e.rsname, sizeof(e.rsname));
	set(e.rsname_pallette, sizeof(e.rsname_pallette));
	set(e.x); set(e.y);
	set(e.circle);
	set(e.frame);
	set(e.start_frame);
	set(e.flags);
	set(e.height);
	set(e.transparency);
	set(e.chance_loop);
	set(e.skip_cycles);
}

bool animation::isvisible() const {
	return is(AnimationEnabled);
}

void animation::painting(point screen) const {
	info e; getinfo(e);
	if(!e)
		return;
	draw::image(x - screen.x, y - screen.y, e.source, e.frame, e.flags, e.transparent);
}

rect animation::getrect() const {
	info e; getinfo(e);
	if(!e)
		return {0, 0, 0, 0};
	return e.source->get(e.frame).getrect(x, y, e.flags);
}

void animation::getinfo(animation::info& e) const {
	e.source = gres(rsname, "data/animations");
	if(!e.source)
		e.source = gres(rsname, "data/pma");
	e.frame = 0;
	if(e.source) {
		if(!e.source->cicles)
			e.frame = draw::getframe(getfps()) % e.source->count;
		else
			e.frame = e.source->getanim(circle, draw::getframe(getfps()));
	}
	e.flags = 0;
	e.transparent = 0xFF;
	if(is(RenderBlackAsTransparent))
		e.transparent = 0x40;
}