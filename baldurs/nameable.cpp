#include "main.h"

race_s nameable::getrace() const {
	switch(kind.type) {
	case Race: return (race_s)kind.value;
	case Monster: return kind.getmonster()->race;
	default: return Human;
	}
}

void nameable::random_name() {
	name = random_name(getgender(), getrace());
}