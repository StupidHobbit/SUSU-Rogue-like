#ifndef STATIC_TILED_MAP_INCLUDED
#define STATIC_TILED_MAP_INCLUDED
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <vector>

#include "Tileset.h"

class StaticTiledMap : public sf::Drawable
{
private:
    const int chunksize=1;//change values of these to match your needs and improve performance
    virtual void draw(sf::RenderTarget& target,sf::RenderStates states)const;
    sf::Texture *m_texture;
    int map_x,map_y,chunks_x,chunks_y;//map x and y - dimensions of map in tiles, chunks x and y - amount of chunks
    int tilesize;
    std::vector<std::vector<sf::VertexArray> > m_chunks;
    std::vector<std::vector<int> > *visibilityMap;
public:
    StaticTiledMap(void);
    virtual ~StaticTiledMap(void){};
    void LoadFrom(Tileset& tileset, std::vector<std::vector<int> > *visMap, int tilemap[], int w, int h);
};

#endif //STATIC_TILED_MAP_INCLUDED
