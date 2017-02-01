#include <vector>
#include <queue>

#include <SFML/System.hpp>

#include "StaticTiledMap.h"
#include "Tileset.h" 
#include "Orders.h"
#include "SpriteVertex.h"
#include "UnitsPatterns.h"

class Unit;
class Location
{
public:
	sf::Mutex mutex;
	std::vector< std::vector<char> > map;
    std::vector< std::vector<Unit*> > unitsMap;
	std::queue<Order> ordersQueue;
	std::vector<Unit*> units;
	SpriteVertex unitsSprites;
	StaticTiledMap tileMap;
	Tileset tileset;
	
	Location(int w, int h);
	sf::Vector2i findFreePos();
	void addUnit(Unit *punit, int tileNum);
	void addUnit(int tileNum, int hp, int clan);
	void addUnit(UnitPattern pattern, int clan);
	void gameLoop();
	std::vector<std::vector<int> > visibilityMap;
	void updateVisibility(Unit *punit);
	
	int w, h;
	bool mathInitialized;
};
