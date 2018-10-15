#include "main.h"

void draw::translate(hotkey* p) {
	for(; *p; p++) {
		if(hot.key == p->key) {
			p->proc();
			hot.key = InputUpdate;
		}
	}
}