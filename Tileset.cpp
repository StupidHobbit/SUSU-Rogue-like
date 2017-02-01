#include <string>

#include<SFML/Graphics.hpp>

#include"Tileset.h"


Tileset::Tileset(const std::string& tileset_name, int tileSize): tileSize(tileSize){
	tileset.loadFromFile(tileset_name);
}
