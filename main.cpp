#include <time.h>
#include <thread>
#include <iostream>

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
#include "Interface.h"
#include "Units.h"
#include "CameraManager.h"


const int W = 63, H = 51;


void gameLoop(Location &location){
	location.gameLoop();
}

int main(int argc, char* argv[])
{   
	srand(time(0));
	
    sf::RenderWindow app(sf::VideoMode(1280,720),"SUSU Rogue-like");
    app.setVerticalSyncEnabled(true);
    CameraManager cam(app);
    //cam.setViewport(sf::FloatRect(0, 0, 0.75, 1));
    
    Tileset tileset("data/tileset.png", 16);
    UnitInterface interface(sf::Vector2f(1024, 0), sf::Vector2i(560, 720), tileset);
    //interface.loadFrom(tileset);
    sf::View interfaceCam = app.getDefaultView();
    //interfaceCam.setViewport(sf::FloatRect(0.75, 0, 0.25, 1));
    
    Location location(W, H);
    location.interface = &interface;
    location.addUnit(getPattern("Knight"), PLAYER1);
    //location.addUnit(getPattern("Warrior"), PLAYER1);
    location.addUnit(getPattern("Skeleton"), MONSTERS);
    location.addUnit(getPattern("Skeleton"), MONSTERS);
    location.addUnit(getPattern("Zombie"), MONSTERS);
    location.addUnit(getPattern("Zombie"), MONSTERS);
    location.addUnit(getPattern("Slime"), MONSTERS);
    location.addUnit(getPattern("Slime"), MONSTERS);
    //std::thread gameLoopThread(gameLoop, std::ref(location));
    
    //interface.setUnit(location.units[0]);
    
    sf::Thread gameLoopThread(&Location::gameLoop, &location);
    gameLoopThread.launch();
    
    sf::Vector2f ms, delta, prev;
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
						location.mutex.lock();
						location.ordersQueue.push(Order(GOTO, (int)worldPos.x / 32, (int)worldPos.y / 32));
						location.mutex.unlock();
					}
					break;
				case sf::Event::KeyPressed:
					location.mutex.lock();
					
					if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
						location.ordersQueue.push(Order(MOVE, -1, 0));
						
					else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
						location.ordersQueue.push(Order(MOVE, 1, 0));
						
					else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
						location.ordersQueue.push(Order(MOVE, 0, -1));
						
					else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
						location.ordersQueue.push(Order(MOVE, 0, 1));
						
					else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
						location.ordersQueue.push(Order(MOVE, 0, 0));
						
					location.mutex.unlock();
				
				default:
					break;
			}
			cam.updateEvent(eve);
		}
            
        
        my_sprite.setPosition(tilePos);    
        
        app.clear();
        location.mutex.lock();
        app.draw(location.tileMap);
        app.draw(location.unitsSprites);
        app.draw(location.hpBars);
        app.draw(my_sprite);
        location.mutex.unlock();
        app.setView(interfaceCam);
        app.draw(interface);
        app.display();
    }
}
