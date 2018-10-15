#include "draw.h"

#pragma once

namespace res {
struct element {
	const char*		name;
	const char*		folder;
	sprite*			data;
	bool			isfolder;
	bool			notfound;
};
void				cleanup();
extern element		elements[];
const char*			getname(int rid);
}
namespace draw {
sprite*				gres(int rid);
}
