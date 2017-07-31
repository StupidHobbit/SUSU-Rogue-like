enum {
    GOTO,    // absolute displacement
    MOVE,	 // relative displacement
    ENTER    // order to enter the dung
};



struct Order
{
	int type; // GOTO, MOVE, e.t.c.
	sf::Vector2i position;
	Order(int type, int x, int y):type(type), position(x, y) {};
};
