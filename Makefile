SRCS = main.cpp StaticTiledMap.cpp Tileset.cpp SpriteVertex.cpp Location.cpp Units.cpp PathFinding.cpp UnitsPatterns.cpp
OBJS = main.o StaticTiledMap.o  Tileset.o SpriteVertex.o Location.o Units.o PathFinding.o UnitsPatterns.o
CFLGS = -O2 -std=c++14



main:
	g++ $(CFLGS) -c $(SRCS) 
	g++ $(CFLGS) $(OBJS) -o release/game -lsfml-graphics -lsfml-window -lsfml-system 
comp:
	g++ $(CFLGS) -c $(SRCS) -ISFML/include

