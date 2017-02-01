#ifndef TILESET_INCLUDED
#define TILESET_INCLUDED
class Tileset
{
public:
    Tileset(const std::string& tileset_name, int tileSize);
    sf::Texture tileset;
    int tileSize;		
};
#endif //TILESET_INCLUDED
