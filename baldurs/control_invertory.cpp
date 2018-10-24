#include "main.h"

using namespace draw;

static struct scrollground : public scrolllist {
	item* data[6] = {};
	scrollground() {
		origin = 0;
		maximum = 5;
		increment = 64;
	}
	void row(rect rc, int n) {}
} ground;

static bool get_drag_target(creature* player, itemdrag& di) {
	cursorset set(res::ITEMS, di.value.getdragportrait(), true);
	hot.pressed = false;
	while(ismodal()) {
		di.target = 0;
		di.target_slot = Backpack;
		player->invertory(&di);
		menumodal(false);
		if(hot.key == MouseLeft)
			return true;
	}
	return false;
}

static void choose_item() {
	itemdrag di = {0};
	di.source = (item*)hot.param;
	auto player = creature::getactive();
	if(!player)
		return;
	if(!di.source || !*di.source)
		return;
	di.value = *di.source;
	di.source->clear();
	auto enable = get_drag_target(player, di);
	if(enable && di.target_slot >= Head && di.target_slot <= Legs) {
		enable = di.value.is(di.target_slot);
		if(enable)
			enable = player->isallow(di.value);
	}
	if(enable)
		*di.target = di.value;
	else
		*di.source = di.value;
}

static void choose_weapon() {
	creature::getactive()->setquick(hot.param);
}

void creature::icon(int x, int y, item* pi, slot_s id, itemdrag* pd) {
	int m = 0;
	rect rc = {x+2, y+2, x + 34, y + 34};
	auto ar = area(rc);
	if(pd && (ar == AreaHilited || ar == AreaHilitedPressed)) {
		pd->target = pi; 
		pd->target_slot = id;
	}
	auto enabled = pi && *pi && isallow(*pi);
	auto hilite = pd && pd->value.is(id);
	if(hilite)
		hilite = isallow(pd->value);
	if(hilite) {
		if(pd->target==pi)
			draw::image(x, y, res::STONSLOT, 4 + m, 0);
		else if(id == Backpack)
			draw::image(x, y, res::STONSLOT, m, 0);
		else
			draw::image(x, y, res::STONSLOT, 17 + m, 0);
	} else
		draw::image(x, y, res::STONSLOT, m, 0);
	//draw::rectb(rc, colors::red);
	if(hot.key == MouseLeft && ar == AreaHilitedPressed)
		execute(choose_item, (int)pi);
	if(!pi || !(*pi)) {
		if(id >= Head && id <= Legs)
			draw::image(x + 2, y + 2, gres(res::STON), id - Head, 0, 0x80);
		else if(id == QuickWeapon)
			draw::image(x + 2, y + 2, gres(res::STON), 17, 0);
		else if(id == QuickOffhand)
			draw::image(x + 2, y + 2, gres(res::STON), 13, 0);
	}
	if(pi && *pi) {
		auto i = pi->getportrait();
		if(i) {
			if(!enabled)
				rectf(rc, colors::red, 0x20);
			draw::image(x + 1, y + 1, gres(res::ITEMS), i, 0);
		}
	}
}

void creature::iconqw(int x, int y, int n, itemdrag* pd) {
	unsigned flags = 0;
	if(active_weapon == n)
		flags |= Checked;
	button(x, y, cmpr(choose_weapon, n), flags, res::INVBUT3, n * 3 + 2, n * 3, n * 3 + 1, n * 3, 0, 0, 0);
	icon(x + 28, y + 1, wears + QuickWeapon + n * 2, QuickWeapon, pd);
	icon(x + 28 + 39, y + 1, wears + QuickOffhand + n * 2, QuickOffhand, pd);
}

void creature::invertory(itemdrag* pd) {
	const int d = 38;
	char temp[260];
	int x, y;
	updategame();
	draw::image(0, 0, gres(res::GUIINV), 0, 0);
	label(22, 23, 206, 28, getname(), 2);
	label(575, 20, 206, 22, "Оружие наготове");
	iconqw(572, 48, 0, pd); iconqw(679, 48, 2, pd);
	iconqw(572, 88, 1, pd); iconqw(679, 88, 3, pd);
	label(574, 131, 111, 22, "Колчан");
	for(auto i=0; i<3; i++) 
		icon(572 + 39 * i, 159, wears + Quiver + i, Quiver, pd);
	label(574, 200, 111, 22, "На поясе");
	for(auto i = 0; i<3; i++)
		icon(572 + 39 * i, 228, wears + QuickItem + i, QuickItem, pd);
	label(574, 270, 111, 22, "Земля");
	memset(ground.data, 0, sizeof(ground.data));
	for(auto i = 0; i<6; i++)
		icon(572 + 39 * (i%2), 298 + 39*(i/2), ground.data[i], LastBackpack, pd);
	view({574, 302, 650, 414}, {655, 302, 667, 414}, 64, ground);
	label(721, 243, 34, 32, szprints(temp, zendof(temp), "%1i", getac(false)), 3);
	label(710, 354, 54, 16, szprints(temp, zendof(temp), "%1i", gethits()), 3);
	label(710, 372, 54, 16, szprints(temp, zendof(temp), "%1i", gethitsmax()), 3);
	labelr(704, 141, 70, 20, sznum(temp, 100), 0, SkinGrey);
	label(341, 281, 117, 14, szprints(temp, zendof(temp), "%1i / %2i lbs", 143, 223), 0, MetalGold);
	// Wears items
	icon(255, 22, Body, pd);
	icon(319, 22, Rear, pd);
	icon(383, 22, Head, pd);
	icon(446, 22, Neck, pd);
	icon(510, 22, Gridle, pd);
	icon(255, 79, LeftFinger, pd);
	icon(510, 79, RightFinger, pd);
	icon(255, 136, Hands, pd);
	icon(510, 136, Legs, pd);
	// Invertory
	x = 251; y = 299;
	for(auto i = Backpack; i <= LastBackpack; i = (slot_s)(i+1)) {
		int x1 = x + (i % 8)*d;
		int y1 = y + (i / 8)*d;
		icon(x1, y1, wears + i, Backpack, pd);
	}
	//
	pickcolors({252, 191}, {252, 231}, {507, 191}, {507, 231});
	//
	paperdoll(400, 195);
	draw::image(20, 79, gres(res::PORTL), getportrait(), 0);
}

void creature::invertory() {
	invertory(0);
}