const int MAX_SCALE = 5; 
const float PLUS_ZOOM = 1.25, MINUS_ZOOM = 0.80;


class CameraManager{
public:
	CameraManager(sf::RenderTarget &app, int maxScale = MAX_SCALE): 
	app(&app), maxScale(maxScale), curScale(0), 
	cam(app.getDefaultView()), zoom(1){}
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
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            cam.move(0.f,-10.f);
            
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            cam.move(-10.f,0.f);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            cam.move(0.f,10.f);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            cam.move(10.f,0.f);

        app->setView(cam);
	} 
	void setCenter(const sf::Vector2f &center){
		cam.setCenter(center);
	}
private:
	sf::View cam;
	sf::Vector2f ms, delta, prev;
	sf::RenderTarget *app;
	int curScale, maxScale;
	float zoom;
};
