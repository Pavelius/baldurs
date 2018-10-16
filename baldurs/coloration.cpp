#include "main.h"

void coloration::upload(color* col) const {
	draw::set(col, metal, 0x04);//metal
	draw::set(col, minor, 0x10);//minor
	draw::set(col, major, 0x1C);//major
	draw::set(col, skin, 0x28);//skin
	draw::set(col, leather, 0x34);//leather
	draw::set(col, armor, 0x40);//armor
	draw::set(col, hair, 0x4C);//hair

	memcpy(col + 0x58, col + 0x11, 8 * sizeof(color));//minor
	memcpy(col + 0x60, col + 0x1d, 8 * sizeof(color));//major
	memcpy(col + 0x68, col + 0x11, 8 * sizeof(color));//minor
	memcpy(col + 0x70, col + 0x05, 8 * sizeof(color));//metal
	memcpy(col + 0x78, col + 0x35, 8 * sizeof(color));//leather
	memcpy(col + 0x80, col + 0x35, 8 * sizeof(color));//leather
	memcpy(col + 0x88, col + 0x11, 8 * sizeof(color));//minor
	//leather: bracers (90-97)
	for(int i = 0x90; i < 0xA8; i += 0x08)
		memcpy(col + i, col + 0x35, 8 * sizeof(color));
	//skin
	memcpy(col + 0xB0, col + 0x29, 8 * sizeof(color));
	//leather
	for(int i = 0xB8; i < 0xFF; i += 0x08)
		memcpy(col + i, col + 0x35, 8 * sizeof(color));
}