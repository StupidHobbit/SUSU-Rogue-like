#ifndef LOCATION_INCLUDED
#define LOCATION_INCLUDED
#include <vector>
#include <set>
#include <queue>

#include <SFML/System.hpp>

#include "StaticTiledMap.h"
#include "Tileset.h" 
#include "Orders.h"
#include "Interface.h"
#include "SpriteVertex.h"
#include "UnitsPatterns.h"
#include "Units.h"

class UnitInterface;
class Unit;
struct Room;
class Location
{
public:
	sf::Mutex mutex;
	std::vector< std::vector<char> > map;
    std::vector< std::vector<Unit*> > unitsMap;
	std::queue<Order> ordersQueue;
	std::set<Unit*> units;
	SpriteVertex unitsSprites;
	StaticTiledMap tileMap;
	Tileset tileset;
	UnitInterface *interface;
	
	Location(int w, int h);
	sf::Vector2i findFreePos();
	void addUnit(Unit *punit, int tileNum);
	void addUnit(int tileNum, int hp, int clan);
	void addUnit(UnitPattern pattern, int clan);
	void removeUnit(Unit *punit);
	void gameLoop();
	std::vector<std::vector<int> > visibilityMap;
	void updateVisibility(Unit *punit);
	
	int w, h;
	bool mathInitialized;
};


struct Room{
	sf::Vector2i pos, size;
	 
	Room(sf::Vector2i pos, sf::Vector2i size);	 
	sf::Vector2i getRandWall(); 
};

int randint(int a, int b);

#endif //LOCATION_INCLUDED
