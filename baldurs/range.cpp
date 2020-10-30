#include "main.h"

BSDATA(rangei) = {{"Touch"},
{"Reach"},
{"Range10", 10},
{"Range20", 20},
{"Range30", 30},
{"Range50", 50},
{"Range60", 60},
{"Range70", 70},
{"Range80", 80},
{"Range100", 100},
{"Range120", 120},
};
assert_enum(rangei, Range120)

int rangei::get(int level) const {
	return feets;
}