#include <queue>

#include <SFML/System/Vector2.hpp>
#include<iostream>
#include "Units.h"
#include "PathFinding.h"

void Unit::attack(Unit *unit){
    unit->hp-=dmg;
}

bool Unit::playersTurn(std::queue<Order> &ordersQueue) {
	int w = location->w, h = location->h;
	sf::Vector2i pos;
	if (!ordersQueue.empty()) {
		Order order = ordersQueue.front();
		ordersQueue.pop();
		switch(order.type){
        case MOVE:{
            sf::Vector2i place = position + order.position;
            if (place == position)
                return true;
            if (!check(place, w, h) || location->map[place.y][place.x] || !astar_find_path(location, position, place, path))
                return false;
            actiontype = ATTACK;
            break;
        }
        case GOTO:{
            sf::Vector2i place = order.position;
            if (place == position)
                return true;
            if (!check(place, w, h) || location->map[place.y][place.x] || !astar_find_path(location, position, place, path))
                return false;
            actiontype = ATTACK;
            break;
        }
		}
	}
	else
        if (!path.size())
            return false;
	pos = path.top();
	path.pop();
	switch(actiontype){
    case ATTACK:
        if(location->unitsMap[pos.y][pos.x] == NULL)
            position = pos;
        else
            if (location->unitsMap[pos.y][pos.x]->clan == MONSTERS && !path.size())
                attack(location->unitsMap[pos.y][pos.x]);
            else{
                while (!path.empty())
                    path.pop();
                return false;
            }
        break;
	}
	return true;
}


bool Unit::monstersTurn() {
	int w = location->w, h = location->h;
	if (position == lastplace) {
		lastplace.x = -1;
		target = NULL;
	}
	for (auto unit: visibleUnits) {
		if (clan != unit->clan) {
			if (find_path(location->map, position, unit->position, path) && (target == NULL || unit->dmg < target->dmg)) {
				target = unit;
				lastplace = unit->position;
			}
			else {
				if (find_path(location->map, position, unit->position, path) && unit->dmg == target->dmg && unit->hp < target->hp) {
					target = unit;
					lastplace = unit->position;
				}
				else {
					if (find_path(location->map, position, unit->position, path) && target == unit) {
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
		} while ((place == position || !find_path(location->map, position, place, path)) && count != 100);
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
	if (location->unitsMap[pos.y][pos.x] != NULL) {
               lastplace.x = -1;
               target = NULL;
               while (!path.empty()) {
                        path.pop();
               }
		if (clan  != location->unitsMap[pos.y][pos.x]->clan) {
			//attack
            attack(location->unitsMap[pos.y][pos.x]);
			return true;
		}
		else {
			return true;
		}
	}
	position = pos;
	return true;
}

bool Unit::makeTurn(std::queue<Order> &ordersQueue) {
    if(hp <= 0){
        kill();
    	return true;
    }
    switch(clan){
    case MONSTERS:
        return monstersTurn();
        break;
    case PLAYER1:
        return playersTurn(ordersQueue);
        break;
    }
}

void Unit::kill(){
	isAlive = false;
}

Unit::Unit(UnitPattern pattern, int clan):
hp(pattern.hp), maxHp(pattern.hp), spriteNum(pattern.spriteNum),
dmg(pattern.dmg), isAlive(true),
lvl(pattern.lvl), clan(clan), visRadius(10),
name(pattern.name), description(pattern.description),
target(NULL), lastplace(sf::Vector2i(-1, -1)), actiontype(0)
{
}
