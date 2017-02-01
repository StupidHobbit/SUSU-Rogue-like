const int GOTO = 0,  // absolute displacement
		  MOVE = 1;	 // relative displacement



struct Order
{
	int type; // GOTO, MOVE, e.t.c.
	sf::Vector2i position;
	Order(int type, int x, int y):type(type), position(x, y) {};
};
