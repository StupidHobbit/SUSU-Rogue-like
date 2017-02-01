#include <thread>
#include <chrono>
#include <time.h>
#include <cmath>
#include <iostream>

#include "Location.h"
#include "Units.h"


Location::Location(int w, int h): tileset("data/tileset.png", 32), w(w), h(h), mathInitialized(false)
{
	map.assign(h, std::vector<char>(w, 0));
	unitsMap.assign(h, std::vector<Unit*>(w, NULL));
	visibilityMap.assign(h, std::vector<int>(w, 0));
	unitsSprites.loadFrom(tileset, &visibilityMap);

	int t_map[h * w];
	for(int i = 0, n = w * h; i < n; i++) t_map[i] = 10;
	for(int i = 0, wall_num = w * h / 2, x, y; i < wall_num; i++){
		x = rand() % w;
		y = rand() % h;
		map[y][x] = 1;
		t_map[y * w + x] = 18;
	}
	for (int x = 0; x < w; x++){
		map[0][x] = 1;
		map[h-1][x] = 1;
		t_map[x] = t_map[w * (h - 1) + x] = 18; 
	}
	for (int y = 0; y < w; y++){
		map[y][0] = 1;
		map[y][w-1] = 1;
		t_map[y * w] = t_map[y * w + w - 1] = 18; 
	}
		
    StaticTiledMap TileMap;
    tileMap.LoadFrom(tileset, &visibilityMap, t_map, w, h); 
}

sf::Vector2i Location::findFreePos(){
	int x, y;
	while(true){
		x = rand() % w;
		y = rand() % h;
		if (!map[y][x]) return sf::Vector2i(x, y);
	}
}

void Location::addUnit(Unit *punit, int tileNum){
	sf::Vector2i pos = findFreePos();
	punit->position = pos;
	unitsMap[pos.y][pos.x] = punit;
	units.push_back(punit);
	punit->id = unitsSprites.append(pos.x, pos.y, tileNum);
	//std::cout << tileNum << std::endl;
	updateVisibility(punit);
}

void Location::addUnit(int hp, int clan, int tileNum){
	Unit *punit = new Unit(hp, clan);
	addUnit(punit, tileNum);
}

void Location::addUnit(UnitPattern pattern, int clan){
	Unit *punit = new Unit(pattern, clan);
	addUnit(punit, pattern.spriteNum);
}

void Location::updateVisibility(Unit *punit){
	static const float PI = 3.14159265359,
					   RAD_DELTA = PI / 30;
	static const int   ANGLES_NUM = 2 * PI / RAD_DELTA;
	static float m_sin[ANGLES_NUM], m_cos[ANGLES_NUM], angle = 0;
	if (!mathInitialized){
		for (int i = 0; i < ANGLES_NUM; i++, angle += RAD_DELTA){
			m_sin[i] = sin(angle);
			m_cos[i] = cos(angle);
		}
		mathInitialized = true;
	}
	
	sf::Vector2i pos = punit->position;
	punit->visibleUnits.clear();
	bool isPlayer = punit->clan == PLAYER1;
	int cur_x = pos.x, cur_y = pos.y;
	
	for(auto v:punit->visibleCells){
		visibilityMap[v.y][v.x]--;
	}
	punit->visibleCells.clear();
	if (isPlayer){
				visibilityMap[cur_y][cur_x]++;
				punit->visibleCells.push_back(sf::Vector2i(cur_x, cur_y));
	}
	for(int angle = 0, R = punit->visRadius, x, y; angle < ANGLES_NUM; angle++){
		for(int r = 1; r <= R; r++){
			x = cur_x + (int)(r * m_cos[angle] + 0.5);
			y = cur_y + (int)(r * m_sin[angle] + 0.5);
			if (x < 0 || y < 0 || x >= w || y >= h) break;
			if (isPlayer){
				visibilityMap[y][x]++;
				punit->visibleCells.push_back(sf::Vector2i(x, y));
			}
			if (unitsMap[y][x] != NULL) punit->visibleUnits.insert(unitsMap[y][x]); 
			if (map[y][x]) break;
		}
	}
}

void Location::gameLoop(){
	
	while(true) for(auto punit:units){
		sf::Vector2i pos, prevPos = punit->position;
		
		do{
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}while(!(punit->makeTurn(*this)));
		pos = punit->position;
		
		updateVisibility(punit);
		
		unitsMap[prevPos.y][prevPos.x] = NULL;
		unitsMap[pos.y][pos.x] = punit;
		unitsSprites.setPos(punit->id, pos);
	}
}


 
