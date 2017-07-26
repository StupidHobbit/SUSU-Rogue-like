const int MAX_SCALE = 5; 
const float PLUS_ZOOM = 1.25, MINUS_ZOOM = 0.80, VELOCITY = 600, VELOCITY2 = 5;



class CameraManager{
public:
	CameraManager(sf::RenderTarget &app, int maxScale = MAX_SCALE): 
	app(&app), maxScale(maxScale), curScale(0), 
	cam(app.getDefaultView()), zoom(1),
	preferableCenter(cam.getCenter()){}
	void updateEvent(sf::Event eve){
		switch (eve.type){
		        case sf::Event::MouseWheelScrolled:
		        	if (eve.mouseWheelScroll.delta > 0){
		        		if (curScale < maxScale){
		        			curScale++;
		        			cam.zoom(PLUS_ZOOM);
		        			zoom *= PLUS_ZOOM;
		        		}
		        	}else{
		        		if (curScale > -maxScale){
		        			curScale--;
		        			cam.zoom(MINUS_ZOOM);
		        			zoom *= MINUS_ZOOM;
		        		}
		        	}
		       		break;
		       	case sf::Event::MouseMoved:
					ms = sf::Vector2f(eve.mouseMove.x, eve.mouseMove.y);
		            delta = ms - prev;
		            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
		                cam.move(-delta * zoom);
		            prev = ms;
					break;
				case sf::Event::Resized:
					cam = app->getDefaultView();
					curScale = 0;
					break;
				
				default:
					break;
			}
	} 
	void update(){
		float delta = deltaT.restart().asSeconds();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            cam.move(0.f,-VELOCITY * delta);
            
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            cam.move(-VELOCITY * delta,0.f);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            cam.move(0.f,VELOCITY * delta);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            cam.move(VELOCITY * delta,0.f);
        if (preferableCenter.x != 0){
			cam.move((preferableCenter - cam.getCenter()) * VELOCITY2 * delta);
		    if (abs((preferableCenter - cam.getCenter()).x) + abs((preferableCenter - cam.getCenter()).y) < 0.5) preferableCenter.x = 0;
        }
        app->setView(cam);
	} 
	void setCenter(const sf::Vector2f &center){
		cam.setCenter(center);
	}
	void setPreferableCenter(const sf::Vector2f &center){
		preferableCenter = center;
	}
private:
	sf::View cam;
	sf::Vector2f ms, delta, prev;
	sf::RenderTarget *app;
	sf::Vector2f preferableCenter;
	sf::Clock deltaT; 
	int curScale, maxScale;
	float zoom;
};
