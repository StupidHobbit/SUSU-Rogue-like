#ifndef UNITS_PATTERNS_INCLUDED
#define UNITS_PATTERNS_INCLUDED
#include <string>
#include <unordered_map>
#include <fstream>
#include "UnitSprites.h"

struct UnitPattern{
 int hp,
 	 dmg,
	 lvl,
	 spriteNum;
};

UnitPattern getPattern(std::string name);

#endif //UNITS_PATTERNS_INCLUDED
