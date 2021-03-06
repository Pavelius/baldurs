#include "main.h"

const item_s Gems = BlueQuarz;
const item_s Arts = CarvedStone;
const item_s MundaneItem = Mace;
const item_s MinorMagicItem = Shortsword;
const item_s MediumMagicItem = Longsword;
const item_s MajorMagicItem = Greatsword;

struct range {
	char		c, d;
	short		multiplier;
	int roll() const {
		int result = c;
		if(d) {
			for(auto i = 0; i < c; i++)
				result += rand() % d;
		}
		if(multiplier)
			result *= multiplier;
		return result;
	}
};
struct treasurei {
	struct coini {
		char		chance;
		range		dice;
		item_s		type;
	};
	aref<coini>		coins;
	aref<coini>		goods;
	aref<coini>		items;
};
static treasurei::coini coins_01[] = {{14},
{29, {1, 6, 1000}, CooperPieces},
{52, {1, 8, 100}, SilverPieces},
{95, {2, 8, 10}, GoldPieces},
{100, {2, 4, 10}, PlatinumPieces},
};
static treasurei::coini coins_02[] = {{13},
{23, {1, 10, 1000}, CooperPieces},
{43, {2, 10, 100}, SilverPieces},
{95, {4, 10, 10}, GoldPieces},
{100, {2, 8, 10}, PlatinumPieces},
};
static treasurei::coini coins_03[] = {{11},
{21, {2, 10, 1000}, CooperPieces},
{41, {4, 8, 100}, SilverPieces},
{95, {1, 4, 100}, GoldPieces},
{100, {1, 10, 10}, PlatinumPieces},
};
//
static treasurei::coini goods_01[] = {{90}, {95, {1}, Gems}, {100, {1}, Arts}};
static treasurei::coini goods_02[] = {{81}, {95, {1, 3}, Gems}, {100, {1, 3}, Arts}};
static treasurei::coini goods_03[] = {{77}, {95, {1, 3}, Gems}, {100, {1, 3}, Arts}};
//
static treasurei::coini items_01[] = {{71}, {95, {1}, MundaneItem}, {100, {1}, MinorMagicItem}};
static treasurei::coini items_02[] = {{49}, {85, {1}, MundaneItem}, {100, {1}, MinorMagicItem}};
static treasurei::coini items_03[] = {{49}, {79, {1, 3}, MundaneItem}, {100, {1}, MinorMagicItem}};
static treasurei treasure_data[] = {{},
{coins_01, goods_01},
{coins_02, goods_02},
{coins_03, goods_03},
};

template<class T> const T* match(const aref<T>& elements, int toughness) {
	for(auto& e : elements) {
		if(toughness <= e.chance)
			return &e;
	}
	return 0;
}

treasure::treasure() {
	clear();
}

void treasure::clear() {
	memset(this, 0, sizeof(treasure));
}

void treasure::generate(int level) {
	auto d = d100();
	auto p = match(treasure_data[level].coins, d);
	if(p) {
		switch(p->type) {
		case CooperPieces: cp += p->dice.roll(); break;
		case SilverPieces: sp += p->dice.roll(); break;
		case GoldPieces: gp += p->dice.roll(); break;
		case PlatinumPieces: pp += p->dice.roll(); break;
		}
	}
	d = d100();
	p = match(treasure_data[level].goods, d);
	if(p) {
		switch(p->type) {
		case Gems: gems += p->dice.roll(); break;
		case Arts: arts += p->dice.roll(); break;
		}
	}
	d = d100();
	p = match(treasure_data[level].items, d);
	if(p) {
		switch(p->type) {
		case MundaneItem: mundane += p->dice.roll(); break;
		case MinorMagicItem: minor += p->dice.roll(); break;
		case MediumMagicItem: medium += p->dice.roll(); break;
		case MajorMagicItem: major += p->dice.roll(); break;
		}
	}
}

void treasure::place(item_s it, int count, int min, int max) {
	adat<container*, 2048> source;
	for(auto& e : bsdata<container>())
		source.add(&e);
	zshuffle(source.data, source.count);
	auto index = 0;
	auto index_maximum = source.getcount();
	if(index_maximum <= 0)
		return;
	while(count > 0) {
		auto i = xrand(min, max);
		if(i > count)
			i = count;
		count -= i;
		auto p = source[index++];
		if(index >= index_maximum)
			index = 0;
		item ito(it);
		ito.setcount(i);
		p->add(ito);
	}
}

void treasure::place() {
	if(cp) {
		if(cp > 5000)
			place(GoldPieces, cp / GP, 10, 70);
		else if(cp > 500)
			place(SilverPieces, cp / SP, 10, 70);
		else
			place(GoldPieces, cp, 10, 70);
	}
	if(sp) {
		if(sp > 5000)
			place(PlatinumPieces, sp*SP / PP, 10, 70);
		else if(sp > 500)
			place(GoldPieces, sp*SP / GP, 10, 70);
		else
			place(SilverPieces, sp, 10, 70);
	}
	if(gp) {
		if(gp > 500)
			place(PlatinumPieces, gp*GP / PP, 10, 70);
		else
			place(GoldPieces, gp, 10, 70);
	}
	if(pp)
		place(PlatinumPieces, pp, 10, 70);
}