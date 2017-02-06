#include <thread>
#include <chrono>
#include <time.h>
#include <cmath>
#include <iostream>

#include "Location.h"
#include "MapSprites.h"
#include "PathFinding.h"


Location::Location(int w, int h): tileset("data/tileset.png", 32), w(w), h(h), mathInitialized(false)
{
	map.assign(h, std::vector<char>(w, 1));
	unitsMap.assign(h, std::vector<Unit*>(w, NULL));
	visibilityMap.assign(h, std::vector<int>(w, 0));
	unitsSprites.loadFrom(tileset, &visibilityMap);

	int t_map[h * w];
	for(int i = 0, n = w * h; i < n; i++) t_map[i] = WALL1;
	
	const int roomsNum = 10,
			  minSize = 5,
			  maxSize = 12,
			  gridSize = 12;
	
	std::vector< std::vector<char> > tempMap(h, std::vector<char>(w, 0));
	std::vector<Room> rooms;
	for (int ix = 2; ix + gridSize + 1 < w; ix += gridSize)
		for (int iy = 2; iy + gridSize + 1 < h; iy += gridSize){
			if (randint(0, 99) > 20){
				int rw, rh, rx, ry;
				rw = randint(minSize, maxSize);
				rh = randint(minSize, maxSize);
				rx = randint(ix, ix + gridSize - rw);
				ry = randint(iy, iy + gridSize - rh);
				rooms.push_back(Room(sf::Vector2i(rx, ry), sf::Vector2i(rw,rh)));
				for (int ix = rx + 1, nx = rx + rw - 1; ix < nx; ix++)
					for (int iy = ry + 1, ny = ry + rh - 1; iy < ny; iy++){
						map[iy][ix] = 0;
						t_map[iy * w + ix] = PLATE2;
					}
				for (int ix = rx, nx = rx + rw; ix < nx; ix++)
					for (int iy = ry, ny = ry + rh; iy < ny; iy++){
						tempMap[iy][ix] = 1;
					}
			}
		}
	
	for(int i = 0, wall_num = w * h / 4, x, y; i < wall_num; i++){
		x = rand() % w;
		y = rand() % h;
		tempMap[y][x] = 1;
	}
	
	for (int i = 0; i < rooms.size(); i++){
		bool f = false;
		while(!f){
			std::stack <sf::Vector2i> path;
			int temp = randint(0, rooms.size() - 1);
			if (temp == i){
				temp = (temp + 1 - 2 * (rand() % 2) + rooms.size()) % rooms.size();
			}
			Room room1 = rooms[i], room2 = rooms[temp];
			sf::Vector2i door1 = room1.getRandWall(),
						 door2 = room2.getRandWall();
			{
			bool f = true;
			int dx[] = { 0, 1, 0, -1 };
			int dy[] = { 1, 0, -1, 0 };
			for (int i = 0; i < 4; i++){
				if (t_map[(door1.y + dy[i]) * w + door1.x + dx[i]] >= DOOR2 &&
				    t_map[(door1.y + dy[i]) * w + door1.x + dx[i]] <= DOOR4 ||
					t_map[(door2.y + dy[i]) * w + door2.x + dx[i]] >= DOOR2 &&
				    t_map[(door2.y + dy[i]) * w + door2.x + dx[i]] <= DOOR4){
					f = false;
					break;    
				}
			}
			if (!f) continue;
			}
			tempMap[door1.y][door1.x] = tempMap[door2.y][door2.x] = 0;
			find_path(tempMap, door1, door2, path);
			if (!path.empty()){
				int ix = door1.x, iy = door1.y;
				t_map[iy * w + ix] = randint(DOOR2, DOOR4);
				map[iy][ix] = 0;
				ix = path.top().x, iy = path.top().y;
				path.pop();
				while(!path.empty()){
					t_map[iy * w + ix] = PLATE2;
					if (randint(0, 199) == 1) t_map[iy * w + ix] = SPIKES;
					map[iy][ix] = 0;
					ix = path.top().x, iy = path.top().y;
					path.pop();
				}
				t_map[iy * w + ix] = randint(DOOR2, DOOR4);
				map[iy][ix] = 0;
				f = true;
			}
			else{
				tempMap[door1.y][door1.x] = tempMap[door2.y][door2.x] = 1;
			}
		}
	}
	
	/*for (int x = 0; x < w; x++){
		map[0][x] = 1;
		map[h-1][x] = 1;
		//t_map[x] = t_map[w * (h - 1) + x] = 18; 
	}
	for (int y = 0; y < w; y++){
		map[y][0] = 1;
		map[y][w-1] = 1;
		//t_map[y * w] = t_map[y * w + w - 1] = 18; 
	}
	map[0][0] = 1;
	t_map[w + 1] = 18; 
	*/	
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
	units.insert(punit);
	punit->id = unitsSprites.append(pos.x, pos.y, tileNum);
	//std::cout << tileNum << std::endl;
	//updateVisibility(punit);
}

void Location::addUnit(UnitPattern pattern, int clan){
	Unit *punit = new Unit(pattern, clan);
	addUnit(punit, pattern.spriteNum);
}

void Location::removeUnit(Unit *punit){
	unitsSprites.erase(punit->id);
	units.erase(punit);
	unitsMap[punit->position.y][punit->position.x] = NULL;
	delete punit;
}

void Location::updateVisibility(Unit *punit){
	static const float PI = 3.14159265359,
					   RAD_DELTA = PI / 40;
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
		if (!punit->isAlive){
			removeUnit(punit);
			continue;
		}
	 
		mutex.lock();
		updateVisibility(punit);
		mutex.unlock();
		
		sf::Vector2i pos, prevPos = punit->position;
		mutex.lock();
		bool flag = punit->makeTurn(*this);
		mutex.unlock();
		if (punit->clan == PLAYER1){
			interface->setUnit(punit);
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
		while(!flag){
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
			mutex.lock();
			flag = punit->makeTurn(*this);
			mutex.unlock();
		}
		pos = punit->position;
		
		unitsMap[prevPos.y][prevPos.x] = NULL;
		unitsMap[pos.y][pos.x] = punit;
		
		mutex.lock();
		unitsSprites.setPos(punit->id, pos);
		mutex.unlock();
	}
}


sf::Vector2i Room::getRandWall(){
	if (rand() % 2){
		int temp = randint(1, size.x - 2);
		return pos + sf::Vector2i(temp, (rand() % 2) * (size.y - 1)); 
	}else{
		int temp = randint(1, size.y - 2);
		return pos + sf::Vector2i((rand() % 2) * (size.x - 1), temp); 
	}
} 


Room::Room(sf::Vector2i pos, sf::Vector2i size): 
pos(pos), size(size)
{}

int randint(int a, int b){
	return a + rand() % (b - a + 1);
}
