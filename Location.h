#ifndef LOCATION_INCLUDED
#define LOCATION_INCLUDED
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <utility>

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

struct vectorComparator{
    bool operator()(const sf::Vector2i a, const sf::Vector2i b){
        return a.x * 10000 + a.y < b.x * 10000 + b.y;
    }
};
class Location
// main class. Contains information about location map and units,
// and visual information. Operates the proccess of making turns
{
public:
	SpriteVertex unitsSprites; // visual information for units
	SpriteVertex hpBars; // visual information for health bars
	SpriteVertex effects;
	Tileset tileset;	// information about tileset
    StaticTiledMap tileMap; 	// visual information for map
    std::map<sf::Vector2i, std::pair<Location*, sf::Vector2i>, vectorComparator> exits;
    std::vector< std::vector<char> > map;	// location map, contains types of tiles
	std::set<Unit*> units;	// set of all units in location
    std::vector< std::vector<Unit*> > unitsMap;	// contains pointers to units, standing in related tiles
	Location(int w, int h);
	sf::Vector2i findFreePos();		// returns coords of any free from units tile
	sf::Vector2i findFreeRoomPos(int roomNum = -1);		// returns coords of any free from units tile in any room
	void addUnit(Unit *punit, int tileNum);		// add unit in location
	void addUnit(Unit *punit, sf::Vector2i pos = sf::Vector2i(0, 0));
	void addUnit(int tileNum, int hp, int clan);
	void addUnit(UnitPattern pattern, int clan);
	void removeUnit(Unit *punit);				// remove unit
	std::vector<std::vector<int> > visibilityMap;	// contains information about level of visibility(for player) of tiles
    std::vector<Room> rooms;
	int w, h;	// width and height of unit
};


struct Room{
// utility class for rooms in dung
	sf::Vector2i pos, size;

	Room(sf::Vector2i pos, sf::Vector2i size);
	sf::Vector2i getRandWall();		// return random wall of room
	sf::Vector2i getRandGrid();		// return random wall of room
};

int randint(int a, int b);
inline bool checkChance(float chance){
	return rand() / (float)RAND_MAX <= chance;
}

#endif //LOCATION_INCLUDED
