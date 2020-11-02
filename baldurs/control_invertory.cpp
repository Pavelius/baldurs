#include "main.h"

using namespace draw;

static citem ground(2, 3);

void creature::show(const char* header, item& it) {
	screenshoot screen(true);
	cursorset set;
	while(ismodal()) {
		int x = 144, y = 15;
		screen.restore();
		image(x, y, res::GIITMH08, 0, 0);
		label(x + 36, y + 37, 357, 30, header, 2); // STONEBIG
		rectb({x + 480, y + 109, x + 492, y + 420}, colors::white);
		if(true) {
			const auto sx = 64, sy = 64;
			draw::state push;
			setclip({x + 429, y + 20, x + 493, y + 84});
			auto sp = gres(res::ITEMS);
			auto& fr = sp->get(it.getportrait() + 1);
			image(x + 429 + 2 + (sx - fr.sx)/2, y + 20 + 2 + (sy - fr.sy) / 2, res::ITEMS, it.getportrait() + 1, ImageNoOffset);
		}
		char temp[512]; stringbuilder sb(temp);
		it.addinfo(sb);
		textf(x + 25, y + 113, 425, temp);
		if(!it.isknown())
			button(x + 179, y + 432, 0, res::GBTNMED, 0, 1, 2, 3, "Опознать", 0, 0);
		button(x + 338, y + 432, buttoncancel, 0, 0, res::GBTNMED, "Отмена", KeyEscape, 0); //4
		//label(x + 314, y + 111, 152, 18, "Что это за текст?"); // NORMAL
		domodal();
	}
}

static void item_description() {
	auto p = (creature*)hot.object;
	auto pi = (item*)hot.param;
	if(!p || !pi)
		return;
	p->show(pi->getname(), *pi);
}

static void enchant_item() {
	screenshoot screen(true);
	cursorset set;
	while(ismodal()) {
		int x = 252, y = 35;
		screen.restore();
		image(x, y, res::GUIINV, 1, 0);
		label(x + 23, y + 22, 254, 20, "Способности предмета"); // NORMAL
		button(x + 18, y + 49, Disabled, res::SPLBUT, 0, 1, 2, 3, 0, 0, 0);
		button(x + 18, y + 89, Disabled, res::SPLBUT, 0, 1, 2, 3, 0, 0, 0);
		button(x + 18, y + 129, Disabled, res::SPLBUT, 0, 1, 2, 3, 0, 0, 0);
		label(x + 71, y + 52, 205, 32, ""); // NORMAL
		label(x + 71, y + 92, 205, 32, ""); // NORMAL
		label(x + 71, y + 132, 205, 32, ""); // NORMAL
		label(x + 24, y + 175, 232, 155, "255, 255, 255");
		rectb({x + 266, y + 173, x + 278, y + 331}, colors::white);
		button(x + 162, y + 339, buttoncancel, 0, 0, res::GBTNSTD, "Отмена", KeyEscape, 0);
		button(x + 21, y + 339, buttonok, 0, 0, res::GBTNSTD, "Применить", KeyEnter, 0);
		domodal();
	}
}

static void choose_item_count() {
	screenshoot screen(true);
	cursorset set;
	while(ismodal()) {
		int x = 262, y = 205;
		screen.restore();
		image(x, y, res::GUIINV, 2, 0);
		//button(x + 20, y + 20, cmpr(buttonparam, 1), 0, res::GUICTRL, 0, 0, 1, 0, 0, 0, 0);
		button(x + 20, y + 92, 0, res::GBTNSTD, 0, 1, 2, 3, 0, 0, 0);
		button(x + 142, y + 92, 0, res::GBTNSTD, 0, 1, 2, 3, 0, 0, 0);
		button(x + 222, y + 44, 0, res::GBTNPLUS, 2, 1, 2, 3, 0, 0, 0);
		button(x + 242, y + 44, 0, res::GBTNMINS, 0, 1, 2, 3, 0, 0, 0);
		label(x + 71, y + 20, 187, 20, "Укажите количество"); // NORMAL
		//rectb({x + 180, y + 46, x + 215, y + 61}, colors::white);
		domodal();
	}
}

static bool get_drag_target(creature* player, itemdrag& di) {
	cursorset set(res::ITEMS, di.value.getdragportrait(), true);
	hot.pressed = false;
	while(ismodal()) {
		di.target = 0;
		di.target_slot = Backpack;
		player->invertory(&di);
		menumodal(false);
		if(hot.key == MouseLeft || hot.key== (MouseLeft | Shift))
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
	di.source->remove();
	auto enable = get_drag_target(player, di);
	if(enable && di.target_slot >= Head && di.target_slot <= Legs) {
		enable = di.value.is(di.target_slot);
		if(enable)
			enable = player->isallow(di.value);
	}
	if(enable) {
		if(di.target_slot == LastBackpack)
			map::drop(player->getindex(), di.value);
		else if(di.target)
			di.target->equip(di.value);
		else
			di.source->equip(di.value);
	} else
		di.source->equip(di.value);
}

static void choose_weapon() {
	creature::getactive()->setquick(hot.param);
}

void creature::icon(int x, int y, slot_s id, itemdrag* pd) {
	icon(x, y, wears + id, id, pd, choose_item);
}

void creature::icon(int x, int y, item* pi, slot_s id, itemdrag* pd, fnitem proc, bool show_background) {
	int m = 0;
	rect rc = {x + 2, y + 2, x + 34, y + 34};
	auto ar = area(rc);
	if(pd && (ar == AreaHilited || ar == AreaHilitedPressed)) {
		pd->target = pi;
		pd->target_slot = id;
	}
	auto enabled = pi && *pi && isallow(*pi);
	if(show_background) {
		auto hilite = pd && pd->value.is(id);
		if(hilite)
			hilite = isallow(pd->value);
		if(hilite) {
			if(pd->target == pi)
				draw::image(x, y, res::STONSLOT, 4 + m, 0);
			else if(id == Backpack)
				draw::image(x, y, res::STONSLOT, m, 0);
			else
				draw::image(x, y, res::STONSLOT, 17 + m, 0);
		} else
			draw::image(x, y, res::STONSLOT, m, 0);
	}
	//draw::rectb(rc, colors::red);
	if(proc) {
		if(hot.key == MouseLeft && ar == AreaHilitedPressed)
			execute(proc, (int)pi, this);
	}
	if(hot.key == MouseRight && ar == AreaHilitedPressed)
		execute(item_description, (int)pi, this);
	if(show_background) {
		if(!pi || !(*pi)) {
			if(id >= Head && id <= Legs)
				draw::image(x + 2, y + 2, gres(res::STON), id - Head, 0, 0x80);
			else if(id == QuickWeapon)
				draw::image(x + 2, y + 2, gres(res::STON), 17, 0);
			else if(id == QuickOffhand)
				draw::image(x + 2, y + 2, gres(res::STON), 13, 0);
		}
	}
	if(pi && *pi) {
		auto i = pi->getportrait();
		if(i) {
			if(!enabled)
				rectf(rc, colors::red, 0x20);
			draw::image(x + 1, y + 1, gres(res::ITEMS), i, 0);
		}
		auto count = pi->getcount();
		if(count > 1) {
			char temp[16]; stringbuilder sb(temp); sb.add("%1i", count);
			auto push_stroke = fore_stroke;
			fore_stroke = colors::black;
			text(x + 32 - textw(temp), y + 20, temp, -1, TextStroke);
			fore_stroke = push_stroke;
		}
	}
}

void creature::iconqw(int x, int y, int n, itemdrag* pd) {
	unsigned flags = 0;
	if(active_weapon == n)
		flags |= Checked;
	if(button(x, y, flags, res::INVBUT3, n * 3 + 2, n * 3, n * 3 + 1, n * 3, 0, 0, 0))
		execute(choose_weapon, n);
	icon(x + 28, y + 1, wears + QuickWeapon + n * 2, QuickWeapon, pd, choose_item);
	icon(x + 28 + 39, y + 1, wears + QuickOffhand + n * 2, QuickOffhand, pd, choose_item);
}

void creature::invertory(itemdrag* pd) {
	const int d = 38;
	char temp[260]; stringbuilder sb(temp);
	int x, y;
	updategame();
	draw::image(0, 0, gres(res::GUIINV), 0, 0);
	label(22, 23, 206, 28, getname(), 2);
	label(575, 20, 206, 22, "Оружие наготове");
	iconqw(572, 48, 0, pd); iconqw(679, 48, 2, pd);
	iconqw(572, 88, 1, pd); iconqw(679, 88, 3, pd);
	label(574, 131, 111, 22, "Колчан");
	for(auto i = 0; i < 3; i++)
		icon(572 + 39 * i, 159, wears + Quiver + i, Quiver, pd, choose_item);
	label(574, 200, 111, 22, "На поясе");
	for(auto i = 0; i < 3; i++)
		icon(572 + 39 * i, 228, wears + QuickItem + i, QuickItem, pd, choose_item);
	label(574, 270, 111, 22, "Земля");
	ground.update(map::getindex(getposition(), getsize()), 2);
	for(auto i = 0; i < 6; i++) {
		auto j = ground.maximum_items - ground.origin * 2;
		icon(572 + 39 * (i % 2), 298 + 39 * (i / 2), (i < j) ? ground.data[i] : 0, LastBackpack, pd, choose_item);
	}
	view({574, 302, 650, 414}, {655, 302, 667, 414}, 64, ground);
	sb.clear(); sb.add("%1i", getac(false)); label(721, 243, 34, 32, temp, 3);
	sb.clear(); sb.add("%1i", gethits()); label(710, 354, 54, 16, temp, 3);
	sb.clear(); sb.add("%1i", gethitsmax()); label(710, 372, 54, 16, temp, 3);
	sb.clear(); sb.add("%1i", getmoney() / SP); labelr(704, 141, 70, 20, temp, 0, MetalSilver);
	sb.clear(); sb.add("%1i / %2i lbs", 143, 223); label(341, 281, 117, 14, temp, 0, MetalGold);
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
	for(auto i = Backpack; i <= LastBackpack; i = (slot_s)(i + 1)) {
		int x1 = x + (i % 8)*d;
		int y1 = y + (i / 8)*d;
		icon(x1, y1, wears + i, Backpack, pd, choose_item);
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