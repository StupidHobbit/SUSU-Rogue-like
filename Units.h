#ifndef UNITS_INCLUDED
#define UNITS_INCLUDED
#include <set>
#include <string>
#include <vector>
#include <stack>
#include <SFML/System/Vector2.hpp>

#include "Location.h"
#include "UnitsPatterns.h"

const int PLAYER1 = 0,
		  MONSTERS = 13;

enum{
    ATTACK
};
class Location;

class Unit
{
private:
	std::stack <sf::Vector2i> path;
	Unit* target;
	int actiontype;
	sf::Vector2i lastplace;
	void attack(Unit *unit);
	virtual bool monstersTurn();
	virtual bool playersTurn(std::queue<Order> &ordersQueue);

public:
// when it's time of unit's turn, this function will be called
// unit can move only in adjacent cell
// must return bool did unit moved or not
	//Unit(int hp, int clan);
	Unit(UnitPattern pattern, int clan);
	virtual bool makeTurn(std::queue<Order> &ordersQueue);
	void kill();
	Location *location;
	int spriteNum;
	bool isAlive;
	int hp, maxHp; // hit points of unit
	int clan;
	int id;
	int lvl;
	int dmg;
	int visRadius;
	std::string name, description;
	sf::Vector2i position; // pair <x, y>
	std::set<Unit*> visibleUnits;
	std::vector<sf::Vector2i > visibleCells;


};


struct hashUnit{
   inline size_t operator() (const Unit &unit) const{
      return unit.id;
   }
};

struct equalAtUnit{
   bool operator()(const Unit &unit1, const Unit &unit2) const{
      return unit1.id == unit2.id;
   }
};
#endif //UNITS_INCLUDED
