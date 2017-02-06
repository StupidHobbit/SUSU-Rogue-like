#include <queue>

#include <SFML/System/Vector2.hpp>

#include "Units.h"
#include "PathFinding.h"


bool Unit::PlayersTurn(Location &location) {
	//ГЇГ°Г®ГўГҐГ°ГЄГ  Г§Г¤Г®Г°Г®ГўГјГї
	int w = location.w, h = location.h;
	sf::Vector2i pos;
	if (!location.ordersQueue.empty()) {
		Order order = location.ordersQueue.front();
		location.ordersQueue.pop();
		sf::Vector2i place;
		if (order.type == MOVE) {
			place = position + order.position;
		}
		else {
			place = order.position;
		}
		if (place == position) {
			return true;
		}
		if (!check(place.x, place.y, w, h) || location.map[place.y][place.x] || !find_path(location.map, position, place, path)) {
			return false;
		}
	}
	else {
		if (!path.size()) {
			return false;
		}
	}
	pos = path.top();
	path.pop();
	if (location.unitsMap[pos.y][pos.x] != NULL) {
		if (clan != location.unitsMap[pos.y][pos.x]->clan) {
			if (!path.size()) {
				//attack
				return true;
			}
			else {
				while (!path.empty()) {
					path.pop();
				}
				return false;
			}
		}
		else {
			while (!path.empty()) {
				path.pop();
			}
			return false;
		}
	}
	position = pos;
	return true;
}


bool Unit::MonstersTurn(Location &location) {
	int w = location.w, h = location.h;
	//ГЇГ°Г®ГўГҐГ°ГЄГ  Г§Г¤Г®Г°Г®ГўГјГї
	if (position == lastplace) {
		lastplace.x = -1;
		target = NULL;
	}
	for (auto unit: visibleUnits) {
		if (clan != unit->clan) {
			if (find_path(location.map, position, unit->position, path) && (target == NULL || unit->dmg < target->dmg)) {
				target = unit;
				lastplace = unit->position;
			}
			else {
				if (find_path(location.map, position, unit->position, path) && unit->dmg == target->dmg && unit->hp < target->hp) {
					target = unit;
					lastplace = unit->position;
				}
				else {
					if (find_path(location.map, position, unit->position, path) && target == unit) {
						lastplace = unit->position;
					}
				}
			}
		}
	}
	if (!path.size()) {
		sf::Vector2i place;
		int count = 0;
		do {
			count++;
			place.x = rand() % w;
			place.y = rand() % h;
		} while ((place == position || !find_path(location.map, position, place, path)) && count != 100);
		if (count == 100) {
			return true;
		}
	}
        if (!path.size()){
            return true;
         }
	sf::Vector2i pos;
	pos = path.top();
	path.pop();
	if (location.unitsMap[pos.y][pos.x] != NULL) {
               lastplace.x = -1;
               target = NULL;
               while (!path.empty()) {
                        path.pop();
               }
		if (clan  != location.unitsMap[pos.y][pos.x]->clan) {
			//attack
			return true;
		}
		else {
			return true;
		}
	}
	position = pos;
	return true;
}

bool Unit::makeTurn(Location &location) {
	if (clan == MONSTERS) {
		return MonstersTurn(location);
	}
	else {
		return PlayersTurn(location);
	}
}

void Unit::kill(){
	isAlive = false;
}

Unit::Unit(UnitPattern pattern, int clan):
hp(pattern.hp), dmg(pattern.dmg), isAlive(true),
lvl(pattern.lvl), clan(clan), visRadius(10),
name(pattern.name), description(pattern.description),
target(NULL), lastplace(sf::Vector2i(-1, -1))
{
}
