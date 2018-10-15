#include "main.h"

using namespace draw;

void creature::help() {
	char temp[8192];
	variant elements[128];
	stringcreator sc;
	stringbuilder sb(sc, temp);
	while(ismodal()) {
		background(res::GUIHELP);
		menumodal();
	}
}