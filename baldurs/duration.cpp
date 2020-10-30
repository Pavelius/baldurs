#include "main.h"

enum duration_round_s : unsigned {
	RRound = 1,
	RMinute = RRound * 10,
	RHour = RMinute * 60,
	RDay = RHour * 24,
};
BSDATA(durationi) = {{"Instant"},
{"Round", RRound},
{"Round1p", 0, RRound, 1},
{"Minute", RMinute},
{"Minute1p", 0, RMinute, 1},
{"Hour", RHour},
{"Hour1p", 0, RHour, 1},
{"Day", RDay},
{"Permanent", 0xFFFFFFFF},
};
assert_enum(durationi, Permanent)

unsigned durationi::getrounds(int level) const {
	auto r = rounds;
	if(per_levels)
		r += rounds_increment*(level / per_levels);
	return r;
}