#include <thread>
#include <chrono>
#include <time.h>
#include <cmath>
#include <iostream>

#include "Location.h"
#include "MapSprites.h"
#include "PathFinding.h"


float SPIKE_CHANCE = 0.006, BROKEN_PLATE_CHANCE = 0.05;

Location::Location(int w, int h): tileset("data/tileset.png", 32), w(w), h(h)
{
	map.assign(h, std::vector<char>(w, 1));
	unitsMap.assign(h, std::vector<Unit*>(w, NULL));
	visibilityMap.assign(h, std::vector<int>(w, 0));
	unitsSprites.loadFrom(tileset, &visibilityMap);
	hpBars.loadFrom(tileset, &visibilityMap);
	hpBars.setMultiplicator(4.0);
	hpBars.setTileSize(8);

	int t_map[h * w];
	for(int i = 0, n = w * h; i < n; i++) t_map[i] = WALL2;

	const int roomsNum = 10,
			  minSize = 5,
			  maxSize = 12,
			  gridSize = 12,
			  room_rate = 80;
	int illegal_door_num = w * h / gridSize / gridSize / 4;

	std::vector< std::vector<char> > tempMap(h, std::vector<char>(w, 0));
	for (int ix = 2; ix + gridSize + 1 < w; ix += gridSize)
		for (int iy = 2; iy + gridSize + 1 < h; iy += gridSize){
			if (randint(1, 100) < room_rate){
				int rw, rh, rx, ry;
				//int wall_type = rand() % 2;
				rw = randint(minSize, maxSize);
				rh = randint(minSize, maxSize);
				rx = randint(ix, ix + gridSize - rw);
				ry = randint(iy, iy + gridSize - rh);
				rooms.push_back(Room(sf::Vector2i(rx, ry), sf::Vector2i(rw,rh)));
				for (int ix = rx + 1, nx = rx + rw - 1; ix < nx; ix++)
					for (int iy = ry + 1, ny = ry + rh - 1; iy < ny; iy++){
						map[iy][ix] = 0;
						t_map[iy * w + ix] = PLATE2;
						if (checkChance(BROKEN_PLATE_CHANCE)) t_map[iy * w + ix] = PLATE3;
					}
				for (int ix = rx, nx = rx + rw; ix < nx; ix++)
					for (int iy = ry, ny = ry + rh; iy < ny; iy++){
						tempMap[iy][ix] = 1;
					}
			}
		}

	for(int i = 0, wall_num = w * h / 4, x, y; i < wall_num; i++){
		x = randint(1, w - 1);
		y = randint(1, h - 1);
		tempMap[y][x] = 1;
	}
	for (int x = 0; x < w; x++){
		tempMap[0][x] = 1;
		tempMap[h-1][x] = 1;
	}
	for (int y = 0; y < h; y++){
		tempMap[y][0] = 1;
		tempMap[y][w-1] = 1;
	}
    //std::cout << '!' << std::endl;
	for(int i = 0, pillar_num = w * h / 125, x, y, l, flag; i < pillar_num; i++){
		x = randint(1, w - 1);
		y = randint(1, h - 1);
		l = randint(3, maxSize);
		flag = rand()%2;
        for (int j = 0; j < l && x && w && y < h; j++, x += flag, y += !flag){
            map[y][x] = 1;
            t_map[y * w + x] = PILLAR;
        }
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
				int tx = (path.top() + (path.top() - door1)).x,
                    ty = (path.top() + (path.top() - door1)).y;
				if (t_map[ty*w + tx] == PILLAR){
                    t_map[ty*w + tx] = PLATE2;
                    map[ty][tx] = 0;
                }
				ix = path.top().x, iy = path.top().y;
				path.pop();
				sf::Vector2i prev_i;
				while(!path.empty()){
					t_map[iy * w + ix] = PLATE2;
					if (checkChance(BROKEN_PLATE_CHANCE)) t_map[iy * w + ix] = PLATE3;
					if (checkChance(SPIKE_CHANCE)) t_map[iy * w + ix] = SPIKES;
					map[iy][ix] = 0;
					prev_i = {ix, iy};
					ix = path.top().x, iy = path.top().y;
					path.pop();
				}
				t_map[iy * w + ix] = randint(DOOR2, DOOR4);
				tx = (prev_i + (prev_i - door2)).x,
				ty = (prev_i + (prev_i - door2)).y;
				if (t_map[ty*w + tx] == PILLAR){
                    t_map[ty*w + tx] = PLATE2;
                    map[ty][tx] = 0;
                }
				map[iy][ix] = 0;
				f = true;
			}
			else{
				tempMap[door1.y][door1.x] = tempMap[door2.y][door2.x] = 1;
			}
		}
	}
	for(int i = 0; i < illegal_door_num; i++){
		for(int j = 0; j < 10000; j++){
			auto pos = rooms[rand() % rooms.size()].getRandWall();
			if (t_map[pos.y * w + pos.x] == WALL2 && (t_map[pos.y * w + pos.x + 1] == WALL2 && t_map[pos.y * w + pos.x - 1] == WALL2 &&
				t_map[pos.y * w + pos.x + w] != WALL2 && t_map[pos.y * w + pos.x - w] != WALL2 ||
				t_map[pos.y * w + pos.x + 1] != WALL2 && t_map[pos.y * w + pos.x - 1] != WALL2 &&
				t_map[pos.y * w + pos.x + w] == WALL2 && t_map[pos.y * w + pos.x - w] == WALL2)){
					t_map[pos.y * w + pos.x] = ILLEGAL_DOOR;
					map[pos.y][pos.x] = 0;
					break;
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
	sf::Vector2i pos = findFreeRoomPos();
	punit->position = pos;
	unitsMap[pos.y][pos.x] = punit;
	units.insert(punit);
	punit->id = unitsSprites.append(pos.x, pos.y, tileNum);
	hpBars.append(pos.x, pos.y, HP0 - HP_NUM + 1);
	punit->location = this;
	//std::cout << tileNum << std::endl;
	//updateVisibility(punit);
}

void Location::addUnit(Unit *punit, sf::Vector2i pos){
	if (!pos.x) pos = findFreeRoomPos();
	punit->position = pos;
	unitsMap[pos.y][pos.x] = punit;
	units.insert(punit);
	punit->id = unitsSprites.append(pos.x, pos.y, punit->spriteNum);
	hpBars.append(pos.x, pos.y, HP0 - HP_NUM + 1);
	punit->location = this;
	//std::cout << tileNum << std::endl;
	//updateVisibility(punit);
}

void Location::addUnit(UnitPattern pattern, int clan){
	Unit *punit = new Unit(pattern, clan);
	addUnit(punit, pattern.spriteNum);
}

void Location::removeUnit(Unit *punit){
	unitsSprites.erase(punit->id);
	std::cout << '!' << std::endl;
	hpBars.erase(punit->id);
	units.erase(punit);
	unitsMap[punit->position.y][punit->position.x] = NULL;
}

sf::Vector2i Location::findFreeRoomPos(int roomNum){
    if (roomNum == -1) roomNum = rand() % rooms.size();
	while(true){
        auto temp = rooms[roomNum].getRandGrid();
		if (!map[temp.y][temp.x]) return temp;
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

sf::Vector2i Room::getRandGrid(){
	return pos + sf::Vector2i(randint(1, size.x - 2), randint(1, size.x - 2));
}

Room::Room(sf::Vector2i pos, sf::Vector2i size):
pos(pos), size(size)
{}

int randint(int a, int b){
    //if (a > b) return b;
	return a + rand() % (b - a + 1);
}
