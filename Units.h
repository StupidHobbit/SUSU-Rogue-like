#ifndef UNITS_INCLUDED
#define UNITS_INCLUDED
#include <set>
#include <vector>
#include <stack>
#include <SFML/System/Vector2.hpp>

const int PLAYER1 = 0,
		  MONSTERS = 13;


class Unit
{
private:
	std::stack <sf::Vector2i> path;
	Unit* target;
	sf::Vector2i lastplace;
	virtual bool MonstersTurn(Location &location);
	virtual bool PlayersTurn(Location &location);
		
public:
// when it's time of unit's turn, this function will be called
// unit can move only in adjacent cell
// must return position of unit after moving(even if he didn't moved)
	Unit(int hp, int clan);
	Unit(UnitPattern pattern, int clan);
	virtual bool makeTurn(Location &location);
	
	int hp; // hit points of unit
	int clan;
	int id;
	int lvl;
	int dmg;
	int visRadius;
	sf::Vector2i position; // pair <x, y>
	std::set<Unit*> visibleUnits;
	std::vector<sf::Vector2i > visibleCells;

	
};

#endif //UNITS_INCLUDED
