#include <SFML/Graphics.hpp>

#include "Tileset.h"
#include "SpriteVertex.h"


SpriteVertex::SpriteVertex():m_tileset(NULL), tileSize(32)
{
    m_vertices.setPrimitiveType(sf::Quads);
}

void SpriteVertex::loadFrom(Tileset& tileset, std::vector<std::vector<int> > *visMap) 
{
    m_tileset = &(tileset.tileset);
    tileSize = tileset.tileSize;
    visibilityMap = visMap;
}

int SpriteVertex::append(int i, int j, int tileNumber){
	// find its position in the tileset texture
    int tu = tileNumber % (m_tileset->getSize().x / tileSize);
    int tv = tileNumber / (m_tileset->getSize().x / tileSize);

    sf::Vertex ver;

    ver.position = sf::Vector2f(i * tileSize, j * tileSize);
    ver.texCoords = sf::Vector2f(tu * tileSize, tv * tileSize);
    m_vertices.append(ver);
    ver.position = sf::Vector2f((i + 1) * tileSize, j * tileSize);
    ver.texCoords = sf::Vector2f((tu + 1) * tileSize, tv * tileSize);
    m_vertices.append(ver);
    ver.position = sf::Vector2f((i + 1) * tileSize, (j + 1) * tileSize);
    ver.texCoords = sf::Vector2f((tu + 1) * tileSize, (tv + 1) * tileSize);
    m_vertices.append(ver);
    ver.position = sf::Vector2f(i * tileSize, (j + 1) * tileSize);
    ver.texCoords = sf::Vector2f(tu * tileSize, (tv + 1) * tileSize);
	m_vertices.append(ver);
	return (m_vertices.getVertexCount() / 4) - 1; 
 }
 
void SpriteVertex::setPos(int id, sf::Vector2i pos){
	int i = pos.x, j = pos.y;
	id *= 4;
	
	m_vertices[id].position = sf::Vector2f(i * tileSize, j * tileSize);
    m_vertices[id + 1].position = sf::Vector2f((i + 1) * tileSize, j * tileSize);
    m_vertices[id + 2].position = sf::Vector2f((i + 1) * tileSize, (j + 1) * tileSize);
    m_vertices[id + 3].position = sf::Vector2f(i * tileSize, (j + 1) * tileSize);
}

void SpriteVertex::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the tileset texture
    states.texture = m_tileset;

    // draw the vertex array
    int n = (m_vertices.getVertexCount());
    sf::VertexArray quad(sf::Quads, 4);
    sf::Vector2f pos;
    for(int i = 0; i < n; i+=4){
    	pos = m_vertices[i].position / (float)tileSize;
    	if ((*visibilityMap)[(int)pos.y][(int)pos.x]){
			quad[0] = m_vertices[i];
			quad[1] = m_vertices[i + 1];
			quad[2] = m_vertices[i + 2];
	   	 	quad[3] = m_vertices[i + 3];
    		target.draw(quad, states);
    	}
    }
    //target.draw(m_vertices, states);
}

