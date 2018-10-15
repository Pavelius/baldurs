#include "main.h"

using namespace draw;

int draw::button(int x, int y, const runable& cmd, unsigned flags, res::tokens r, int checked, int normal, int pressed, int disabled, const char* name, int key, button_states* button_state, bool pressed_execute) {
	sprite* e = gres(res::tokens(r));
	if(!e)
		return 0;
	bool need_execute = false;
	bool is_disabled = false;
	int tx = 0;
	int ty = 0;
	auto s = normal;
	if(button_state)
		*button_state = ButtonNormal;
	const sprite::frame& f = e->get(s);
	rect rc = {x, y, x + f.sx, y + f.sy};
	auto a = area(rc);
	if(flags&Disabled) {
		is_disabled = true;
		s = disabled;
		if(button_state)
			*button_state = ButtonDisabled;
	} else if(key && hot.key == key)
		need_execute = true;
	else if(a == AreaHilitedPressed) {
		tx += 2;
		ty += 2;
		s = pressed;
		if(button_state)
			*button_state = ButtonPressed;
	} else if(flags&Checked) {
		s = checked;
		if(button_state)
			*button_state = ButtonChecked;
	}
	if(!is_disabled && (a == AreaHilited || a == AreaHilitedPressed)) {
		if(hot.key == MouseLeft && hot.pressed == pressed_execute)
			need_execute = true;
	}
	if(is_disabled && disabled == normal) {
		color pal[256];
		const sprite::frame& f = e->get(s);
		memcpy(pal, e->offs(f.pallette), sizeof(pal));
		for(auto& c : pal)
			c = c.gray();
		draw::image(x, y, e, s, 0, 255, pal);
	} else
		draw::image(x, y, e, s, 0);
	if(name) {
		color old_fore = fore;
		if(flags&Disabled)
			fore = fore.mix(colors::black, 8);
		draw::text({x + tx, y + ty, x + f.sx, y + f.sy}, name, AlignCenterCenter);
		fore = old_fore;
	}
	if(!is_disabled) {
		//if(drag::hints && zchr(drag::hints, id)) {
		//	if(r == res::STONSLOT)
		//		draw::image(x, y, gres(res::STONSLOT), 17, 0);
		//	else
		//		draw::rectb(rc, colors::black.mix(colors::red, 160));
		//}
		//if(drag::targets && hot.mouse.in(rc) && zchr(drag::targets, id)) {
		//	drag::target = id;
		//	if(r == res::STONSLOT)
		//		draw::image(x, y, gres(res::STONSLOT), 26, 0);
		//	else
		//		draw::rectb(rc, colors::red);
		//}
	}
	if(need_execute)
		cmd.execute();
	return rc.height();
}

int draw::button(int x, int y, const runable& cmd, unsigned flags, res::tokens r, const char* name, int key, button_states* state) {
	char st[4];
	sprite* f = gres(res::tokens(r));
	int count;
	int m = cmd.getid();
	switch(r) {
	case res::GBTNMINS:
	case res::GBTNPLUS:
		count = f->count / 3;
		if(count) {
			st[0] = (m % count) * 3 + 1;
			st[1] = (m % count) * 3 + 0;
			st[2] = (m % count) * 3 + 1;
			st[3] = (m % count) * 3 + 2;
		}
		break;
	case res::GBTNRECB:
		count = f->count / 3;
		if(count) {
			st[0] = (m % count) * 3 + 2;
			st[1] = (m % count) * 3 + 0;
			st[2] = (m % count) * 3 + 1;
			st[3] = (m % count) * 3 + 0;
		}
		break;
	case res::GUIBTBUT:
		st[0] = 2;
		st[1] = 0;
		st[2] = 1;
		st[3] = 0;
		break;
	case res::GBTNSPB2:
		st[0] = (m - 1) * 3 + 0;
		st[1] = (m - 1) * 3 + 1;
		st[2] = (m - 1) * 3 + 2;
		st[3] = (m - 1) * 3 + 1;
		break;
	default:
		if(f)
			count = f->count / 4;
		if(count) {
			st[0] = (m % count) * 4 + 0;
			st[1] = (m % count) * 4 + 1;
			st[2] = (m % count) * 4 + 2;
			st[3] = (m % count) * 4 + 3;
		}
		break;
	}
	return button(x, y, cmd, flags, r, st[0], st[1], st[2], st[3], name, key, state, false);
}