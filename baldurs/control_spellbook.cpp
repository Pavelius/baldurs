#include "main.h"

using namespace draw;

static adat<variant, 8>	power_class;
static int				power_index;
static int				power_origin;
static int				current_level = 1;

static void choose_class() {
	power_index = hot.param;
}

static void choose_level() {
	current_level = hot.param;
}

static void power_modify() {
	power_origin += hot.param;
}

static void add_power() {
	auto id = (spell_s)hot.param;
	auto type = power_class[power_index];
	auto player = creature::getactive();
	auto count = player->getprepared(id, type);
	player->setprepared(id, type, count + 1);
}

static void remove_power() {
	auto id = (spell_s)hot.param;
	auto type = power_class[power_index];
	auto player = creature::getactive();
	auto count = player->getprepared(id, type);
	if(count)
		player->setprepared(id, type, count - 1);
}

static void info_power() {
	auto id = (spell_s)hot.param;
	creature::spellinfo(id);
}

static struct scrollspell : scrolllist {
	variant		data[64];
	int			free;
	void row(rect rc, int n) {
		static int offset[8] = {1, 0, -1, -2, -1, -2, -3, -4};
		auto id = (spell_s)data[n].value;
		auto oy = offset[n - origin];
		draw::image(rc.x1 + 2, rc.y1 + oy, gres(res::SPELLS), id, 0);
		label(rc.x1 + 38, rc.y1 + oy - 4, rc.width() - 46, rc.height() - 2, getstr(data[n]), 0);
		if(area(rc) == AreaHilitedPressed && hot.key == MouseRight)
			draw::execute(info_power, id);
		if(free > 0) {
			if(area(rc) == AreaHilitedPressed && hot.key == MouseLeft)
				draw::execute(add_power, id);
		}
	}
} known;

void creature::spellbook() {
	adat<preparation*> memorized;
	char temp[260]; stringbuilder sb(temp);
	background(res::GUISPL);
	draw::image(20, 79, gres(res::PORTL), getportrait(), 0);
	label(22, 23, 206, 28, getname(), 2);
	label(253, 50, 228, 19, "����������� ����������");
	label(492, 50, 228, 19, "��������� ����������");
	label(253, 391, 187, 19, "��������� ����� ����������");
	power_class.clear();
	for(auto e = FirstClass; e <= LastClass; e = (class_s)(e + 1)) {
		if(!bsdata<classi>::elements[e].spells)
			continue;
		if(classes[e] >= bsdata<classi>::elements[e].spells[0].minimal)
			power_class.add(e);
	}
	// ������������� ������ �������
	int power_maximum = power_class.getcount();
	if(power_index > power_maximum)
		power_index = power_maximum - 1;
	if(power_origin > power_maximum - 4)
		power_origin = power_maximum - 4;
	if(power_origin < 0)
		power_origin = 0;
	// ������ ������ ����������
	auto x = 273, y = 19;
	for(int i = power_origin, im = i + 4; i < im; i++) {
		if(i >= power_maximum)
			button(x, y, buttoncancel, Disabled, res::GBTNSPB1, 0);
		else {
			auto e = power_class[i];
			unsigned flags = 0;
			if(e == power_class[power_index])
				flags |= Checked;
			if(button(x, y, flags, res::GBTNSPB1, getstr(e))) {
				execute(choose_class, power_class.indexof(e));
			}
		}
		x += 108;
	}
	if(button(252, 19, 0, res::GBTNSPB3, 0, 0, 1, 0, 0, 0, 0))
		execute(power_modify, -1);
	if(button(705, 19, 0, res::GBTNSPB3, 2, 2, 3, 2, 0, 0, 0))
		execute(power_modify, 1);
	if(power_maximum <= 0)
		draw::rectf({0, 0, 800, 492}, colors::black, 128);
	else {
		auto current_power = power_class[power_index];
		auto slots_maximum = getspellslots(current_power, current_level), slots_used = getprepared(current_power);
		known.free = slots_maximum - slots_used;
		// ������� ��������� �����
		//label(449, 390, 32, 20, szprints(temp, zendof(temp), "%1i", known.free));
		// ������ ����������
		x = 740; y = 62;
		for(int i = 1; i <= 9; i++) {
			unsigned flags = (current_level == i) ? Checked : 0;
			if(button(x, y, flags, res::GBTNSPB2, 0, 0, 0))
				execute(choose_level, i);
			y += 39;
		}
		// �������� ������ ���������
		auto pb = known.data;
		auto pe = pb + sizeof(known.data) / sizeof(known.data[0]);
		for(auto e = FirstSpell; e <= LastSpell; e = (spell_s)(e + 1)) {
			if(bsdata<spelli>::elements[e].levels[current_power.value] != current_level)
				continue;
			if(!isknown(e))
				continue;
			if(pb < pe)
				*pb++ = e;
		}
		known.maximum = pb - known.data;
		qsort(known.data, known.maximum, sizeof(known.data[0]), compare_variant);
		// ����������� ����������
		x = 254; y = 85; auto index = 0;
		for(auto& e : powers) {
			if(e && e.type == current_power && bsdata<spelli>::elements[e.id].levels[current_power.value] == current_level) {
				auto id = e.id;
				rect rc = {x, y, x + 79, y + 39};
				sb.clear(); sb.add("%1i/%2i", e.count, e.count_maximum);
				label(x, y, 29, 18, temp);
				draw::image(x + 37, y - 7, gres(res::SPELLS), id, 0);
				if(area(rc) == AreaHilitedPressed) {
					if(hot.key == MouseLeft)
						draw::execute(remove_power, id);
					else if(hot.key == MouseRight)
						draw::execute(info_power, id);
				}
				x += 79;
				if(((++index) % 3) == 0) {
					x = 254;
					y += 39;
				}
			}
		}
		view({492, 79, 702, 409}, {708, 79, 720, 409}, 43, known);
	}
}

void creature::spellinfo(spell_s id) {
	screenshoot e(true);
	while(ismodal()) {
		e.restore();
		auto x = (getwidth() - 429) / 2, y = (getheight() - 446) / 2;
		image(x, y, gres(res::GUISPL), 2, 0);
		label(x + 22, y + 22, 343, 20, "���������� ��� ����������");
		label(x + 22, y + 52, 343, 20, getstr(id));
		image(x + 375, y + 22, gres(res::SPELLS), id, 0);
		//rectb({x + 23, y + 83, x + 23 + 363, y + 83 + 312}, colors::green);
		//rectb({x + 396, y + 82, x + 408, y + 395}, colors::white);
		if(button(x + 135, y + 402, 0, res::GBTNMED, 0, 1, 2, 3, "�������", KeyEscape, 0))
			execute(buttoncancel);
		domodal();
	}
}