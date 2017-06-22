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


const int W = 63, H = 51;


void gameLoop(Location &location){
	location.gameLoop();
}

int main(int argc, char* argv[])
{   
	srand(time(0));
	
    sf::RenderWindow app(sf::VideoMode(1280,720),"SUSU Rogue-like");
    app.setVerticalSyncEnabled(true);
    sf::View cam=app.getDefaultView();
    //cam.setViewport(sf::FloatRect(0, 0, 0.75, 1));
    int maxScale = 5, curScale = 0; 
    
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
    
    	app.setView(cam);
    	sf::Vector2i pixelPos = sf::Mouse::getPosition(app);
        sf::Vector2f worldPos = app.mapPixelToCoords(pixelPos);
        sf::Vector2f tilePos = sf::Vector2f((int)(0.0001 + worldPos.x / 32.0) * 32,
        									(int)(0.0001 + worldPos.y / 32.0) * 32);

        interface.updateDescription((sf::Vector2f)pixelPos);
    	
        sf::Event eve;
        while(app.pollEvent(eve))
        	switch (eve.type)
        	{
		        case sf::Event::Closed:
		            gameLoopThread.terminate();
		            app.close();
		            break;
		        case sf::Event::MouseWheelScrolled:
		        	if (eve.mouseWheelScroll.delta > 0){
		        		if (curScale < maxScale){
		        			curScale++;
		        			cam.zoom(1.25);
		        		}
		        	}else{
		        		if (curScale > -maxScale){
		        			curScale--;
		        			cam.zoom(0.80);
		        		}
		        	}
		       		break;
		       	case sf::Event::MouseMoved:
					ms = sf::Vector2f(eve.mouseMove.x, eve.mouseMove.y);
		            delta = ms - prev;
		            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
		                cam.move(-delta);
		            prev = ms;
					break;
				case sf::Event::MouseButtonPressed:
					if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
						location.mutex.lock();
						location.ordersQueue.push(Order(GOTO, (int)worldPos.x / 32, (int)worldPos.y / 32));
						location.mutex.unlock();
					}
						//std::cout << (int)worldPos.x / 32 << ' ' << (int)worldPos.y / 32 << std::endl;
					break;
				case sf::Event::Resized:
					cam = app.getDefaultView();
					curScale = 0;
					break;
				case sf::Event::KeyPressed:
					if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
						location.mutex.lock();
						location.ordersQueue.push(Order(MOVE, -1, 0));
						location.mutex.unlock();
					}
					else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
						location.mutex.lock();
						location.ordersQueue.push(Order(MOVE, 1, 0));
						location.mutex.unlock();
					}
					else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
						location.mutex.lock();
						location.ordersQueue.push(Order(MOVE, 0, -1));
						location.mutex.unlock();
					}
					else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
						location.mutex.lock();
						location.ordersQueue.push(Order(MOVE, 0, 1));
						location.mutex.unlock();
					}
					else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
						location.mutex.lock();
						location.ordersQueue.push(Order(MOVE, 0, 0));
						location.mutex.unlock();
					}
				
				default:
					break;
			}
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            cam.move(0.f,-10.f);
            
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            cam.move(-10.f,0.f);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            cam.move(0.f,10.f);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            cam.move(10.f,0.f);
            
        
        my_sprite.setPosition(tilePos);    
        
        app.clear();
        app.setView(cam);
        location.mutex.lock();
        app.draw(location.tileMap);
        app.draw(location.unitsSprites);
        app.draw(location.hpBars);
        app.draw(my_sprite);
        location.mutex.unlock();
        app.setView(interfaceCam);
        app.draw(interface);
        //std::cout << "!" << std::endl;
        app.display();
    }
}
