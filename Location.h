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
// main class. Contains information about location map and units,
// and visual information. Operates the proccess of making turns 
{
public:
	sf::Mutex mutex; // mutex for blocking gameloop while drawing something
	std::vector< std::vector<char> > map;	// location map, contains types of tiles
    std::vector< std::vector<Unit*> > unitsMap;	// contains pointers to units, standing in related tiles
	std::queue<Order> ordersQueue;
	std::set<Unit*> units;	// set of all units in location 
	SpriteVertex unitsSprites; // visual information for units
	SpriteVertex hpBars; // visual inforamtion for health bars
	SpriteVertex effects; 
	StaticTiledMap tileMap; 	// visual inforamtion for map
	Tileset tileset;	// information about tileset
	UnitInterface *interface;	// pointer to user interfase object
	
	Location(int w, int h);
	sf::Vector2i findFreePos();		// returns coords of any free from units tile
	void addUnit(Unit *punit, int tileNum);		// add unit in location
	void addUnit(int tileNum, int hp, int clan);
	void addUnit(UnitPattern pattern, int clan);
	void removeUnit(Unit *punit);				// remove unit
	void gameLoop();	// forever loop, making units turns 
	std::vector<std::vector<int> > visibilityMap;	// contains information about level of visibility(for player) of tiles
	void updateVisibility(Unit *punit);	// update visibility with field of view of unit
	
	int w, h;	// width and height of unit
	bool mathInitialized;
};


struct Room{
// utility class for rooms in dung
	sf::Vector2i pos, size;
	 
	Room(sf::Vector2i pos, sf::Vector2i size);	 
	sf::Vector2i getRandWall();		// return random wall of room 
};

int randint(int a, int b);

#endif //LOCATION_INCLUDED
