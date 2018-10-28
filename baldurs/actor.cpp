#include "main.h"

enum animate_s : unsigned char {
	AnimateMove,
	AnimateStand, AnimateStandRelax, AnimateStandLook,
	AnimateCombatStance, AnimateCombatStanceTwoHanded,
	AnimateGetHit,
	AnimateGetHitAndDrop, AnimateAgony,
	AnimateGetUp,
	AnimateMeleeOneHanded, AnimateMeleeOneHandedSwing, AnimateMeleeOneHandedThrust,
	AnimateMeleeTwoHanded, AnimateMeleeTwoHandedSwing, AnimateMeleeTwoHandedThrust,
	AnimateMeleeTwoWeapon, AnimateMeleeTwoWeaponSwing, AnimateMeleeTwoWeaponThrust,
	AnimateShootBow, AnimateShootSling, AnimateShootXBow,
	AnimateCastBig, AnimateCastBigRelease, AnimateCast, AnimateCastRelease, AnimateCastThird, AnimateCastThirdRelease, AnimateCastFour, AnimateCastFourRelease
};
static struct animate_info {
	const char*		name;
	bool			disable_overlay;
	bool			ready;
} animate_data[] = {{"AnimateMove"},
{"AnimateStand", false, true}, {"AnimateStandRelax", false, true}, {"AnimateStandLook", false, true},
{"AnimateCombatStance", false, true}, {"AnimateCombatStanceTwoHanded", false, true},
{"AnimateGetHit"}, {"AnimateGetHitAndDrop"},
{"AnimateAgony", false, true},
{"AnimateGetUp", true},
{"AnimateMeleeOneHanded"}, {"AnimateMeleeOneHandedSwing"}, {"AnimateMeleeOneHandedThrust"},
{"AnimateMeleeTwoHanded"}, {"AnimateMeleeTwoHandedSwing"}, {"AnimateMeleeTwoHandedThrust"},
{"AnimateMeleeTwoWeapon"}, {"AnimateMeleeTwoWeaponSwing"}, {"AnimateMeleeTwoWeaponThrust"},
{"AnimateShootBow"}, {"AnimateShootSling"}, {"AnimateShootXBow"},
{"AnimateCastBig", true},
{"AnimateCastBigRelease", true},
{"AnimateCast", true},
{"AnimateCastRelease", true},
{"AnimateCastThird", true},
{"AnimateCastThirdRelease", true},
{"AnimateCastFour", true},
{"AnimateCastFourRelease", true},
};
assert_enum(animate, AnimateCastFourRelease);
getstr_enum(animate)

static short unsigned actions_mid1[AnimateCastFourRelease + 1] = {9, 1, 0, 0, 1, 1, 2, 3, 4, 5,
10, 11, 11, 10, 11, 11, 10, 11, 11, 10, 11, 11,
7, 8, 7, 8, 7, 8, 7, 8,
};

using namespace draw;

#define M_PI_2	1.57079632679489661923   // pi/2
#define M_PI	3.14159265358979323846   // pi

extern "C" double atan(double x);
extern "C" double cos(double x);
extern "C" double sin(double x);

static point formations[10][20] = {{{0, 0}, {0, 36}, {0, 72}, {0, 108}, {0, 144}, {0, 180}, {0, 216}, {0, 252}, {0, 288}, {0, 324}, {0, 360}, {0, 396}, {0, 432}, {0, 468}, {0, 504}, {0, 540}, {0, 576}, {0, 612}, {0, 648}, {0, 684}},// Follow
{{0, 0}, {48, 0}, {-48, 0}, {0, 48}, {0, 84}, {0, 120}, {0, 156}, {0, 192}, {0, 228}, {0, 264}, {0, 300}, {0, 336}, {0, 372}, {0, 408}, {0, 444}, {0, 480}, {0, 516}, {0, 552}, {0, 588}, {0, 624}},// T
{{0, -36}, {48, -24}, {-48, -24}, {48, 24}, {-48, 24}, {0, 36}, {48, 48}, {-48, 48}, {0, 72}, {48, 72}, {-48, 72}, {0, 108}, {48, 96}, {-48, 96}, {0, 144}, {48, 120}, {-48, 120}, {0, 180}, {48, 144}, {-48, 144}}, // Gather
{{0, 0}, {64, 0}, {-64, 0}, {128, 0}, {0, 48}, {64, 48}, {-64, 48}, {128, 48}, {0, 96}, {64, 96}, {-64, 96}, {128, 96}, {0, 144}, {64, 144}, {-64, 144}, {128, 144}, {0, 192}, {64, 192}, {-64, 192}, {128, 192}}, // 4 and 2
{{0, 0}, {64, 0}, {-64, 0}, {0, 48}, {64, 48}, {-64, 48}, {0, 96}, {64, 96}, {-64, 96}, {0, 144}, {64, 144}, {-64, 144}, {0, 192}, {64, 192}, {-64, 192}, {0, 240}, {64, 240}, {-64, 240}, {0, 288}, {64, 288}}, // 3 by 2
{{0, 0}, {0, -36}, {-64, 0}, {64, 0}, {-32, 48}, {32, 48}, {0, 24}, {0, 48}, {0, 72}, {0, 96}, {0, 120}, {0, 144}, {0, 168}, {0, 192}, {0, 216}, {0, 240}, {0, 264}, {0, 288}, {0, 312}, {0, 336}}, // Protect
};
const char orient_dx[16] = {0, -4, -7, -9, -10, -9, -7, -4, 0, 4, 7, 9, 10, 9, 7, 4};
const char orient_dy[16] = {10, 9, 7, 4, 0, -4, -7, -9, -10, -9, -7, -4, 0, 4, 7, 9};

point actor::getposition(point src, point dst, formation_s formation, int pos) {
	if(formation >= sizeof(formations) / sizeof(formations[0]))
		return src;
	pos = imin(pos, (int)(sizeof(formations[0]) / sizeof(formations[0][0])));
	// calculate angle
	double angle;
	double xdiff = src.x - dst.x;
	double ydiff = src.y - dst.y;
	if(ydiff == 0) {
		if(xdiff > 0)
			angle = M_PI_2;
		else
			angle = -M_PI_2;
	} else {
		angle = atan(xdiff / ydiff);
		if(ydiff < 0)
			angle += M_PI;
	}
	// calculate new coordinates by rotating formation around (0,0)
	double newx = -formations[formation][pos].x * cos(angle) + formations[formation][pos].y * sin(angle);
	double newy = formations[formation][pos].x * sin(angle) + formations[formation][pos].y * cos(angle);
	dst.x = imin(imax(dst.x + (short)newx, 8), (short)map::width * 16);
	dst.y = imin(imax(dst.y + (short)newy, 8), (short)map::height * 12);
	return dst;
}

res::tokens actor::getanimation(item_s type) {
	switch(type) {
	case BattleAxe: return res::WQSAX;
	case Mace: case Club: return res::WQSMC;
	case Dagger: return res::WQSDD;
	case Hammer: return res::WQSWH;
	case Halberd: return res::WQSHB;
	case Spear: return res::WQSSP;
	case Staff: return res::WQSQS;
	case Longsword: return res::WQSS1;
	case Shortsword: return res::WQSSS;
	case Greatsword: return res::WQSS2;
	case ShieldSmall: return res::WQSD1;
	case ShieldLarge: return res::WQSD3;
	case ShieldTower: return res::WQSD4;
	case Helm: return res::WQSH1; // Helm is separate?
	default: return res::NONE;
	}
}

res::tokens actor::getanimation(race_s race, gender_s gender, class_s type, int ai, int& ws) {
	res::tokens icn;
	switch(race) {
	case Dwarf:
	case Gnome:
		icn = res::CDMB1;
		ws = 0; // верно
		break;
	case Elf:
	case HalfElf:
		if(gender == Female)
			icn = res::CEFB1;
		else
			icn = res::CEMB1;
		ws = 2; // верно
		break;
	case Halfling:
		if(gender == Female)
			icn = res::CIFB1;
		else
			icn = res::CIMB1;
		if(type == Wizard || type == Sorcerer)
			type = Rogue;
		if(ai > 1)
			ai = 1;
		ws = 0;
		break;
	default:
		if(gender == Female) {
			ws = 1; // верно для людей
			icn = res::CHFB1;
		} else {
			ws = 3; // верно для людей, но не для оружия
			icn = res::CHMB1;
		}
		break;
	}
	if(type == Wizard || type == Sorcerer)
		icn = (res::tokens)(icn + (res::CDMW1 - res::CDMB1) + ai);
	else if(type == Cleric)
		icn = (res::tokens)(icn + ai);
	else if(type == Rogue && ai)
		icn = (res::tokens)(icn + (res::CDMT1 - res::CDMB1));
	else {
		if(ai == 3)
			icn = (res::tokens)(icn + 4);
		else
			icn = (res::tokens)(icn + ai);
	}
	return icn;
}

const sprite* actor::getsprite(res::tokens id, int wi) {
	if(!id)
		return 0;
	return gres((res::tokens)(id + wi));
}

void actor::say(const char* format, ...) const {
	char temp[4096];
	szprints(temp, zendof(temp), "[%1]: ", getname());
	szprintvs(zend(temp), zendof(temp), format, xva_start(format));
	mslogv(temp);
}

void actor::act(int player, const char* format, ...) {
	char temp[4096];
	szprints(temp, zendof(temp), "%1 ", getname());
	szprintvs(zend(temp), zendof(temp), format, xva_start(format));
	szprints(zend(temp), zendof(temp), ".");
	mslogv(temp);
}

void actor::clearcolors() {
	memset(&colors, 0, sizeof(colors));
}

void actor::clearpath() {
	if(path)
		path = map::removeall(path);
}

int	actor::getzorder() const {
	if(action == AnimateAgony)
		return -64;
	return 0;
}

const sprite* actor::getsprite(int& wi) const {
	auto kind = getkind();
	switch(monster_data[kind].sptype) {
	case CID1:
		return draw::gres(getanimation(getrace(), getgender(), getclass(), getwear(Body).getarmorindex(), wi));
	case MID1:
		return draw::gres(monster_data[kind].sprites[0]);
	default:
		return 0;
	}
}

int actor::getflags() const {
	auto kind = getkind();
	switch(monster_data[kind].sptype) {
	case CID1:
		if(orientation >= 9)
			return ImageMirrorH;
		return 0;
	default:
		return 0;
	}
}

int actor::getcicle() const {
	auto kind = getkind();
	switch(monster_data[kind].sptype) {
	case CID1:
		if(orientation >= 9)
			return action * 9 + ((9 - 1) * 2 - orientation);
		return action * 9 + orientation;
	case MID1:
		return actions_mid1[action] * 8 + orientation / 2;
	default:
		return 0;
	}
}

void actor::stop() {
	action = AnimateStand;
	action_target.clear();
	frame = 0;
	duration = draw::getframe();
	clearpath();
}

void actor::set(animate_s id) {
	if(action == id)
		return;
	action = id;
	frame = 0;
	duration = draw::getframe();
	if(id == AnimateStand)
		clearpath();
}

int actor::getciclecount(int cicle) const {
	int wi;
	auto s = getsprite(wi);
	if(!s)
		return 0;
	return s->getcicle(cicle)->count;
}

bool actor::move(point destination, short unsigned maximum_range, short unsigned minimum_reach) {
	clearpath();
	if(!destination)
		return false;
	if(map::getindex(position) == map::getindex(destination))
		return true;
	dest = destination;
	auto s = getsize();
	auto i = map::getindex(position, s);
	auto start = map::getindex(position, s);
	auto goal = map::getindex(destination, s);
	map::blockimpassable(Blocked - 1);
	blockimpassable();
	map::setcost(start, Blocked - 1);
	map::createwave(goal, s);
	auto cost = map::getcost(start);
	if(cost >= Blocked - 1)
		return false;
	if(minimum_reach && cost <= minimum_reach)
		return true;
	path = map::route(start, map::stepto, maximum_range, minimum_reach);
	if(!path)
		return false;
	set(AnimateMove);
	this->start = position;
	range = 0;
	return true;
}

int	actor::getmovedistance(point destination, short unsigned minimum_reach) const {
	if(!destination)
		return 1;
	if(map::getindex(position) == map::getindex(destination))
		return 1;
	auto s = getsize();
	auto start = map::getindex(position, s);
	auto goal = map::getindex(destination, s);
	map::blockimpassable(Blocked - 1);
	blockimpassable();
	if(map::getcost(goal) == Blocked)
		return 0;
	map::createwave(start, s);
	return map::getcost(goal);
}

void actor::setposition(point newpos) {
	position = newpos;
}

void actor::update() {
	if(duration > draw::getframe())
		return; // Еще не пришло время
	if(isstunned()) {
		if(action != AnimateCombatStanceTwoHanded) {
			set(AnimateCombatStanceTwoHanded);
			return;
		}
	}
	frame++;
	duration += 1000 / getfps();
	auto s = getsize();
	if(action == AnimateMove) {
		point newpos = position;
		range += getspeed();
		while(true) {
			if(!path) {
				auto e = action_target;
				stop();
				if(e.method)
					interacting(e);
				return;
			}
			newpos = map::getposition(path->index, s);
			if(position != newpos)
				orientation = map::getorientation(position, newpos);
			int m = distance(start, newpos);
			int dx = newpos.x - start.x;
			int dy = newpos.y - start.y;
			if(m == 0 || range > m) {
				setposition(newpos);
				start = position;
				range -= m;
				path = map::remove(path);
				continue;
			}
			newpos.x = start.x + dx * range / m;
			newpos.y = start.y + dy * range / m;
			break;
		}
		if(isblock(newpos)) {
			set(Stunned, xrand(2, 5));
			return;
		}
		setposition(newpos);
		return;
	}
	int cicle_count = getciclecount(getcicle());
	if(!cicle_count)
		return;
	int cicle_frame = frame % cicle_count;
	if((cicle_frame + 1) == cicle_count) {
		switch(action) {
		case AnimateStand:
			if(d100() < 25)
				set(AnimateStandLook);
			break;
		case AnimateStandLook:
			set((d100() < 25) ? AnimateStandRelax : AnimateStand);
			break;
		case AnimateStandRelax:
			set(AnimateStand);
			break;
		case AnimateGetHitAndDrop:
			set(AnimateAgony);
			break;
		case AnimateAgony:
			if(d100() < 25) {
				frame = cicle_count - 1;
				duration += xrand(10, 20) * 1000;
			}
			break;
		default:
			stop();
			break;
		}
	}
}

rect actor::getrect() const {
	return {position.x - 64, position.y - 128, position.x + 64, position.y + 64};
}

void actor::update_portrait() {
	auto i = getportrait();
	if(i)
		colors.set(i);
}

bool actor::hittest(point pt) const {
	int s = getsize() + 1;
	rect rc = {position.x - s * 8, position.y - 6 * s, position.x + s * 8, position.y + s * 6};
	return pt.in(rc);
}

void actor::painting(point screen) const {
	color pallette[256];
	int x = position.x - screen.x;
	int y = position.y - screen.y;
	int cicle_index = getcicle();
	int cicle_flags = getflags();
	colors.upload(pallette);
	if(isselected())
		marker(x, y, getsize(), colors::green, false, true);
	int wi;
	const sprite* sprites[4] = {getsprite(wi)};
	auto kind = getkind();
	switch(monster_data[kind].sptype) {
	case CID1:
		if(!animate_data[action].disable_overlay) {
			sprites[1] = getsprite(getanimation(getwear(Head).gettype()), wi);
			sprites[2] = getsprite(getanimation(getwear(QuickWeapon).gettype()), wi);
			sprites[3] = getsprite(getanimation(getwear(QuickOffhand).gettype()), wi);
		}
		break;
	case MID1:
		memcpy(pallette, sprites[0]->offs(sprites[0]->get(0).pallette), sizeof(pallette));
		for(auto i = 1; i < sizeof(monster_data[kind].sprites) / sizeof(monster_data[kind].sprites[0]); i++)
			sprites[i] = draw::gres(monster_data[kind].sprites[i]);
		break;
	}
	auto shadow = map::getshadow(position);
	for(auto& e : pallette) {
		e.r = (e.r * shadow.r) >> 8;
		e.g = (e.g * shadow.g) >> 8;
		e.b = (e.b * shadow.b) >> 8;
	}
	for(auto p : sprites) {
		if(!p)
			continue;
		auto cicle_frame = p->getanim(cicle_index, getframe());
		image(x, y, p, cicle_frame, cicle_flags, 0xFF, pallette);
	}
}

static void painting_equipment(int x, int y, item equipment, int ws, int frame, unsigned flags, color* pallette) {
	if(!equipment)
		return;
	res::tokens t1 = res::tokens(actor::getanimation(equipment.gettype()) + ws);
	image(x, y, gres(t1), frame, flags, 255, pallette);
}

void actor::paperdoll(int x, int y) const {
	paperdoll(x, y, colors, getrace(), getgender(), getclass(), AnimateStand, 2,
		getwear(Body), getwear(QuickWeapon), getwear(QuickOffhand), getwear(Head));
}

void actor::paperdoll(int x, int y, const coloration& colors, race_s race, gender_s gender, class_s type) {
	paperdoll(x, y, colors, race, gender, type, AnimateStand, 2);
}

void actor::paperdoll(int x, int y, const coloration& colors, race_s race, gender_s gender, class_s type, animate_s animation, int orientation, item armor, item weapon, item offhand, item helm) {
	sprite* source;
	unsigned flags;
	int ws;
	source = gres(getanimation(race, gender, type, armor.getarmorindex(), ws));
	if(!source)
		return;
	const int directions = 9;
	int o = orientation;
	if(o >= directions) {
		flags = ImageMirrorH;
		o = (9 - 1) * 2 - o;
	} else
		flags = 0;
	color pallette[256]; colors.upload(pallette);
	auto frame = source->getanim(animation*directions + o, draw::getframe(13));
	draw::image(x, y, source, frame, flags, 255, pallette);
	painting_equipment(x, y, weapon, ws, frame, flags, pallette);
	painting_equipment(x, y, helm, ws, frame, flags, pallette);
	painting_equipment(x, y, offhand, ws, frame, flags, pallette);
}

point actor::getforward(point start, int step, int orientation) {
	return{(short)(start.x + orient_dx[orientation] * step), (short)(start.y + orient_dy[orientation] * step)};
}

point actor::getbackward(point start, int step, int orientation) {
	return{(short)(start.x - orient_dx[orientation] * step), (short)(start.y - orient_dy[orientation] * step)};
}

void actor::marker(int x, int y, int size, color c, bool flicking, bool double_border) {
	auto r = (size + 1) * 6;
	if(flicking)
		r += iabs(int(draw::getframe(16) % 6) - 3) - 1;
	circle(x, y, r, c);
	if(double_border)
		circle(x, y, r + 1, c);
}

void actor::render_marker(const rect& rc, int ox, int oy) const {
	if(action == AnimateMove) {
		if(dest) {
			int x = dest.x - ox + rc.x1;
			int y = dest.y - oy + rc.y1;
			int s = getsize();
			int r = s + 1;
			int p = 80 + iabs((int)(draw::getframe(50) % 40) - 20);
			int w = r * 8 * p / 100;
			int h = r * 6 * p / 100;
			rect rce = {x - w, y - h, x + w, y + h};
			if(rc.intersect(rce))
				marker(x, y, getsize(), colors::yellow, true, false);
		}
	}
}

void actor::render_path(const rect& rc, int mx, int my) const {
	if(action == AnimateMove) {
		point p1;
		auto s = getsize();
		p1.x = position.x - mx + rc.x1;
		p1.y = position.y - my + rc.y1;
		for(auto p = path; p; p = p->next) {
			point p2 = map::getposition(p->index, s);
			p2.x = p2.x - mx + rc.x1;
			p2.y = p2.y - my + rc.y1;
			draw::line(p1, p2, colors::yellow);
			p1 = p2;
		}
	}
}

void actor::clear() {
	memset(&colors, 0, sizeof(colors));
	action_target.clear();
	position.clear();
	dest.clear();
	start.clear();
	frame = 0;
	orientation = 0;
	duration = 0;
	path = 0;
	range = 0;
}

animate_s actor::getattackanimate(int number) const {
	auto w1 = getwear(QuickWeapon);
	auto w2 = getwear(QuickOffhand);
	if(w2.is(QuickOffhand) && w2.is(QuickWeapon))
		return animate_s(AnimateMeleeTwoWeapon + number);
	else if(w1.isbow())
		return animate_s(AnimateShootBow + number);
	else if(w1.isxbow())
		return animate_s(AnimateShootXBow + number);
	else if(w1.isthown())
		return animate_s(AnimateShootSling + number);
	else if(w1.istwohand())
		return animate_s(AnimateMeleeTwoHanded + number);
	return animate_s(AnimateMeleeOneHanded + number);
}

void actor::render_attack(int number, point destination) {
	if(position != destination)
		orientation = map::getorientation(position, destination);
	set(getattackanimate(number));
}

void actor::render_hit(bool fatal) {
	set(fatal ? AnimateGetHitAndDrop : AnimateGetHit);
}

bool actor::isready() const {
	return animate_data[action].ready;
}