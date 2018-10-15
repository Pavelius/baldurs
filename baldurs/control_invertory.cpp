#include "main.h"

using namespace draw;
const int QWMarker = 10;

static struct scrollground : public scrolllist {
	item* data[6] = {};
	scrollground() {
		origin = 0;
		maximum = 5;
		increment = 64;
	}
	void row(rect rc, int n) {}
} ground;

void creature::icon(int x, int y, const runable& cmd, item* pi, slot_s id) {
	int m = (id % 4);
	button(x, y, cmd, 0, res::STONSLOT, 31 + m, 0 + m, 0 + m, 0 + m, 0, 0, 0, true);
	if(!pi || !(*pi)) {
		if(id >= Head && id <= Legs)
			draw::image(x + 2, y + 2, gres(res::STON), id - Head, 0, 0x80);
		else if(id >= QuickWeapon && id <= LastQuickWeapon) {
			if((id & 1) == 0)
				draw::image(x + 2, y + 2, gres(res::STON), 17, 0);
			else
				draw::image(x + 2, y + 2, gres(res::STON), 13, 0);
		}
	}
	if(pi && *pi) {
		auto i = pi->getportrait();
		if(i)
			draw::image(x + 1, y + 1, gres(res::ITEMS), i, 0);
	}
}

static void choose_item() {}

static void choose_weapon() {
	creature::getplayer()->setquick(hot.param);
}

void creature::iconqw(int x, int y, int n) {
	auto id = (slot_s)(QuickWeapon + n * 2);
	unsigned flags = 0;
	if(active_weapon == n)
		flags |= Checked;
	button(x, y, cmpr(choose_weapon, n), flags, res::INVBUT3, n * 3 + 2, n * 3, n * 3 + 1, n * 3, 0, 0, 0);
	icon(x + 28, y + 1, (slot_s)(id + 0));
	icon(x + 28 + 39, y + 1, (slot_s)(id + 1));
}

void creature::icon(int x, int y, slot_s id) {
	icon(x, y, cmpr(choose_item, id), id);
}

static void pick_item() {
	auto pi = (item*)hot.param;
	auto player = creature::getplayer();
	if(!player)
		return;
	if(!pi)
		return;
	player->add(*pi);
	pi->clear();
}

void creature::invertory() {
	const int d = 38;
	char temp[260];
	int x, y;
	updategame();
	draw::image(0, 0, gres(res::GUIINV), 0, 0);
	label(22, 23, 206, 28, getname(), 2);
	label(575, 20, 206, 22, "Оружие наготове");
	iconqw(572, 48, 0); iconqw(679, 48, 2);
	iconqw(572, 88, 1); iconqw(679, 88, 3);
	label(574, 131, 111, 22, "Колчан");
	for(auto i=0; i<3; i++) 
		icon(572 + 39 * i, 159, slot_s(Quiver+i));
	label(574, 200, 111, 22, "На поясе");
	for(auto i = 0; i<3; i++)
		icon(572 + 39 * i, 228, slot_s(QuickItem + i));
	label(574, 270, 111, 22, "Земля");
	memset(ground.data, 0, sizeof(ground.data));
	for(auto i = 0; i<6; i++)
		icon(572 + 39 * (i%2), 298 + 39*(i/2), cmpr(pick_item, (int)ground.data[i]), ground.data[i], Backpack);
	view({574, 302, 650, 414}, {655, 302, 667, 414}, 64, ground);
	label(721, 243, 34, 32, szprints(temp, zendof(temp), "%1i", getac(false)), 3);
	label(710, 354, 54, 16, szprints(temp, zendof(temp), "%1i", gethits()), 3);
	label(710, 372, 54, 16, szprints(temp, zendof(temp), "%1i", gethitsmax()), 3);
	labelr(704, 141, 70, 20, sznum(temp, 100), 0, ColorDarkSilver);
	label(341, 281, 117, 14, szprints(temp, zendof(temp), "%1i / %2i lbs", 143, 223), 0, ColorLightGold);
	// Wears items
	icon(255, 22, Body);
	icon(319, 22, Rear);
	icon(383, 22, Head);
	icon(446, 22, Neck);
	icon(510, 22, Gridle);
	icon(255, 79, LeftFinger);
	icon(510, 79, RightFinger);
	icon(255, 136, Hands);
	icon(510, 136, Legs);
	// Invertory
	x = 251; y = 299;
	for(auto i = Backpack; i <= LastBackpack; i = (slot_s)(i+1)) {
		int x1 = x + (i % 8)*d;
		int y1 = y + (i / 8)*d;
		icon(x1, y1, i);
	}
	//
	pickcolors({252, 191}, {252, 231}, {507, 191}, {507, 231});
	//
	paperdoll(400, 195);
	draw::image(20, 79, gres(res::PORTL), getportrait(), 0);
	menumodal();
	//default:
	//	if(id >= FirstInvertory && id <= LastInvertory) {
	//		int* item_source = get_creature_items(rec, id);
	//		if(!item_source)
	//			break;
	//		int item_drag = *item_source;
	//		if(!item_drag)
	//			break;
	//		int targets[128];
	//		int hints[128];
	//		targets[0] = 0;
	//		hints[0] = 0;
	//		*item_source = 0;
	//		// Invertory slots controls
	//		for(int i = FirstInvertory; i <= LastInvertory; i++) {
	//			if(bsget(item::get(item_drag, Type), i)) {
	//				zcat(targets, i);
	//				if(i<FirstBackpack || i>LastBackpack)
	//					zcat(hints, i);
	//			}
	//		}
	//		// Players who is nearby
	//		for(auto rec : players) {
	//			if(!is_creature_ready(rec))
	//				continue;
	//			if(find_space_in_backpack(rec, item_drag))
	//				zcat(targets, rec);
	//		}
	//		cursorset push(res::ITEMS, bsget(item_drag, Portrait));
	//		drag::source = id;
	//		drag::targets = targets;
	//		drag::hints = hints;
	//		id = invertory_drag(rec, ground, ch, id, items);
	//		if(id == OK) {
	//			int* item_target;
	//			if(drag::target >= FirstCreature && drag::target <= LastCreature)
	//				item_target = find_space_in_backpack(drag::target, item_drag);
	//			else
	//				item_target = get_creature_items(rec, drag::target);
	//			if(item_target) {
	//				*item_source = *item_target;
	//				*item_target = item_drag;
	//			}
	//		} else
	//			*item_source = item_drag;
	//		drag::source = 0;
	//		drag::target = 0;
	//		drag::targets = 0;
	//		drag::hints = 0;
	//	} else if(id >= QWMarker && id <= QWMarker + 4)
	//		bsset(rec, ActiveWeapon, id - QWMarker);
}