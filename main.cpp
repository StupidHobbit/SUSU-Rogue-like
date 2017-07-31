#include <time.h>
#include <thread>
#include <iostream>
#include <set>

//#include <SFML/System/Vector2.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Location.h"
#include "MapSprites.h"
#include "Interface.h"
#include "Units.h"
#include "CameraManager.h"


const int W = 63, H = 51;


Location *cur_location, *render_location;
sf::Mutex mutex;
std::queue<Order> ordersQueue;
std::set<Unit*> units;
Tileset tileset("data/tileset.png", 16);
UnitInterface interface(sf::Vector2f(1024, 0), sf::Vector2i(560, 720), tileset);
sf::RenderWindow app(sf::VideoMode(1280,720),"SUSU Rogue-like");
CameraManager cam(app);
void clearVisibility(Unit *punit){
    for(auto v:punit->visibleCells){
		cur_location->visibilityMap[v.y][v.x]--;
		cur_location->tileMap.setAlpha(v, 128);
	}
	punit->visibleCells.clear();
}
void updateVisibility(Unit *punit){
	static const float PI = 3.14159265359,
					   RAD_DELTA = PI / 100;
	static const int   ANGLES_NUM = 2 * PI / RAD_DELTA;
	static float m_sin[ANGLES_NUM], m_cos[ANGLES_NUM], angle = 0;
	static int mathInitialized = false;
	if (!mathInitialized){
		for (int i = 0; i < ANGLES_NUM; i++, angle += RAD_DELTA){
			m_sin[i] = sin(angle);
			m_cos[i] = cos(angle);
		}
		mathInitialized = true;
	}

	sf::Vector2i pos = punit->position;
	bool isPlayer = punit->clan == PLAYER1;
	int cur_x = pos.x, cur_y = pos.y;
    clearVisibility(punit);
	if (isPlayer){
		cur_location->visibilityMap[cur_y][cur_x]++;
		punit->visibleCells.push_back(sf::Vector2i(cur_x, cur_y));
	}
	for(int angle = 0, R = punit->visRadius, x, y; angle < ANGLES_NUM; angle++){
		for(int r = 1; r <= R; r++){
			x = cur_x + (int)(r * m_cos[angle] + 0.5);
			y = cur_y + (int)(r * m_sin[angle] + 0.5);
			if (x == cur_x && y == cur_y) continue;
			if (x < 0 || y < 0 || x >= cur_location->w || y >= cur_location->h) break;
			if (isPlayer){
				cur_location->visibilityMap[y][x]++;
				cur_location->tileMap.setAlpha(sf::Vector2i(x, y), 255);
				punit->visibleCells.push_back(sf::Vector2i(x, y));
			}
			if (cur_location->unitsMap[y][x] != NULL) {
				punit->visibleUnits.insert(cur_location->unitsMap[y][x]);
				break;
			}
			if (cur_location->map[y][x]) break;
		}
	}
}

void gameLoop(){
	while(true) for(Unit *punit:units){
        if (punit->clan == PLAYER1) render_location = punit->location;
        cur_location = punit->location;
        if (cur_location != render_location) continue;
		if (!punit->isAlive){
			cur_location->removeUnit(punit);
			units.erase(punit);
			delete punit;
			continue;
		}
		mutex.lock();
		updateVisibility(punit);
		mutex.unlock();
		//std::cout << '!' << std::endl;
		sf::Vector2i pos, prevPos = punit->position;
		char flag = false;
		if (punit->clan == PLAYER1){
			mutex.lock();
			interface.setUnit(punit);
			mutex.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
		if (!flag){
            mutex.lock();
            //flag = punit->makeTurn(ordersQueue);
            mutex.unlock();
            while(!flag){
                mutex.lock();
                if(ordersQueue.front().type == ENTER){
                    ordersQueue.pop();
                    std::cout << punit->position.x << std::endl;
                    auto exit = cur_location->exits.find(punit->position);
                    if (exit != cur_location->exits.end()){
                        std::cout << punit->position.x << std::endl;
                        flag = true;
                        clearVisibility(punit);
                        cur_location->removeUnit(punit);
                        exit->second.first->addUnit(punit, exit->second.second);
                        cur_location = exit->second.first;
                        if (punit->clan == PLAYER1) render_location = cur_location;
                        mutex.unlock();
                        break;
                    }
                }
                flag = punit->makeTurn(ordersQueue);
                mutex.unlock();
                if (!flag) std::this_thread::sleep_for(std::chrono::milliseconds(15));
            }
            pos = punit->position;
            cur_location->unitsMap[prevPos.y][prevPos.x] = NULL;
            cur_location->unitsMap[pos.y][pos.x] = punit;
            mutex.lock();
            cur_location->unitsSprites.setPos(punit->id, pos);
            cur_location->hpBars.setPos(punit->id, pos);
            cur_location->hpBars.setTile(punit->id, HP0 - (int)(0.5 + HP_NUM * punit->hp / (float)(punit->maxHp)) + 1);
            if (punit->clan == PLAYER1)
                cam.setPreferableCenter(((sf::Vector2f)punit->position + sf::Vector2f(0.5, 0.5)) * 32.0f);
            mutex.unlock();
		}
	}
}

Location *createWorld(int depth = 3, int beginSize = 30, int deltaSize = 13, int maxBranches = 2, int maxEdgeMultiplicity = 3){
    Location *cur_location = new Location(beginSize, beginSize);
    if (depth)
    for (int i = 0, branchesNum = randint(1, maxBranches); i < branchesNum; i++){
        auto temp_location = createWorld(depth - 1, beginSize + deltaSize, deltaSize, maxBranches, maxEdgeMultiplicity);
        for (int j = 0, edgesNum = randint(1, maxEdgeMultiplicity); j < edgesNum; j++){
            sf::Vector2i in = cur_location->findFreeRoomPos(),
                         out = temp_location->findFreeRoomPos();
            int x = out.x, y = out.y;
            std::cout << x << ' ' <<  y << ' ' << (int)temp_location->map[y][x] << std::endl;
            cur_location->tileMap.setTile(in, DOWNSTAIRS);
            temp_location->tileMap.setTile(out, UPSTAIRS);
            //sf::Vector2i(0, 1) < sf::Vector2i(0, 2);
            cur_location->exits[in] = {temp_location, out};
            temp_location->exits[out] = {cur_location, in};
        }
    }
    return cur_location;
}

Location *fillWorldMonsters(Location* cur_location, int monstersNum = 1, float monstersRate = 0.5){
    for (int j = 0; j < cur_location->rooms.size(); j++)
        for (int i = 0; i < monstersNum; i++)
            if(checkChance(monstersRate)){
                Unit *punit = new Unit(getPattern(), MONSTERS);
                units.insert(punit);
                cur_location->addUnit(punit);
            }
    for (auto loc: cur_location->exits){
        if (!loc.second.first->units.size())
            fillWorldMonsters(loc.second.first, monstersNum, monstersRate);
    }
    return cur_location;
}

int main(int argc, char* argv[])
{
	srand(time(0));

    app.setVerticalSyncEnabled(true);
    sf::View interfaceCam = app.getDefaultView();
    cur_location = render_location = createWorld();
    fillWorldMonsters(cur_location);
    Unit *phero = new Unit(getPattern("Knight"), PLAYER1);
    units.insert(phero);
    cur_location->addUnit(phero);
    cam.setPreferableCenter(((sf::Vector2f)phero->position + sf::Vector2f(0.5, 0.5)) * 32.0f);

    //std::cout << '!' << std::endl;
    //std::thread gameLoopThread(gameLoop, std::ref(location));

    sf::Thread gameLoopThread(gameLoop);
    gameLoopThread.launch();

    sf::Texture my_tex;
    my_tex.loadFromFile("data/green_allocation.png");
    sf::Sprite my_sprite;
    my_sprite.setTexture(my_tex);

    while(app.isOpen())
    {

    	cam.update();
    	sf::Vector2i pixelPos = sf::Mouse::getPosition(app);
        sf::Vector2f worldPos = app.mapPixelToCoords(pixelPos);
        sf::Vector2f tilePos = sf::Vector2f((int)(0.0001 + worldPos.x / 32.0) * 32,
        									(int)(0.0001 + worldPos.y / 32.0) * 32);

        interface.updateDescription((sf::Vector2f)pixelPos);

        sf::Event eve;
        while(app.pollEvent(eve)){
        	switch (eve.type)
        	{
		        case sf::Event::Closed:
		            gameLoopThread.terminate();
		            app.close();
		            break;
				case sf::Event::MouseButtonPressed:
					if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
						mutex.lock();
						ordersQueue.push(Order(GOTO, (int)worldPos.x / 32, (int)worldPos.y / 32));
						mutex.unlock();
					}
					break;
				case sf::Event::KeyPressed:
					mutex.lock();

					if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
						ordersQueue.push(Order(MOVE, -1, 0));

					else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
						ordersQueue.push(Order(MOVE, 1, 0));

					else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
						ordersQueue.push(Order(MOVE, 0, -1));

					else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
						ordersQueue.push(Order(MOVE, 0, 1));

					else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
						ordersQueue.push(Order(MOVE, 0, 0));

                    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
						ordersQueue.push(Order(ENTER, 0, 0));

					mutex.unlock();

				default:
					break;
			}
			cam.updateEvent(eve);
		}


        my_sprite.setPosition(tilePos);

        app.clear();
        mutex.lock();
        app.draw(render_location->tileMap);
        app.draw(render_location->unitsSprites);
        app.draw(render_location->hpBars);
        app.draw(my_sprite);
        mutex.unlock();
        app.setView(interfaceCam);
        app.draw(interface);
        app.display();
    }
}
