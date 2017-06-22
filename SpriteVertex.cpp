#include <SFML/Graphics.hpp>
//#include <SFML/Time.hpp>

#include "Tileset.h"
#include "SpriteVertex.h"


SpriteVertex::SpriteVertex():m_tileset(NULL), tileSize(32), mult(1.0)
{
    m_vertices.setPrimitiveType(sf::Quads);
}

void SpriteVertex::loadFrom(Tileset& tileset, std::vector<std::vector<int> > *visMap) 
{
	loadFrom(tileset);
    visibilityMap = visMap;
}

void SpriteVertex::loadFrom(Tileset& tileset) 
{
    m_tileset = &(tileset.tileset);
    tileSize = tileset.tileSize;
    visibilityMap = nullptr;
}

int SpriteVertex::append(int i, int j, int tileNumber, sf::Vector2f v, sf::Vector2f ofs){
	// find its position in the tileset texture
    int tu = tileNumber % (m_tileset->getSize().x / tileSize);
    int tv = tileNumber / (m_tileset->getSize().x / tileSize);
	if (heap.empty()){
		sf::Vertex ver;
		velocity.push_back(v);
		velocity.push_back(ofs);
		ver.position = sf::Vector2f(i * tileSize * mult, j * tileSize * mult);
		ver.texCoords = sf::Vector2f(tu * tileSize, tv * tileSize);
		m_vertices.append(ver);
		ver.position = sf::Vector2f((i * mult + 1) * tileSize, j * tileSize * mult);
		ver.texCoords = sf::Vector2f((tu + 1) * tileSize, tv * tileSize);
		m_vertices.append(ver);
		ver.position = sf::Vector2f((i * mult + 1) * tileSize, (j * mult + 1) * tileSize);
		ver.texCoords = sf::Vector2f((tu + 1) * tileSize, (tv + 1) * tileSize);
		m_vertices.append(ver);
		ver.position = sf::Vector2f(i * tileSize * mult, (j * mult + 1) * tileSize);
		ver.texCoords = sf::Vector2f(tu * tileSize, (tv + 1) * tileSize);
		m_vertices.append(ver);
		return (m_vertices.getVertexCount() / 4) - 1;
	}else{
		int id = -heap.top();
		heap.pop();
		velocity[id] = v;
		velocity[id] = ofs;
		id *= 4;
		
		m_vertices[id].position = sf::Vector2f(i * tileSize * mult, j * tileSize * mult);
		m_vertices[id].texCoords = sf::Vector2f(tu * tileSize, tv * tileSize);
		
		m_vertices[id + 1].position = sf::Vector2f((i * mult + 1) * tileSize, j * tileSize * mult);
		m_vertices[id + 1].texCoords = sf::Vector2f((tu + 1) * tileSize, tv * tileSize);

		m_vertices[id + 2].position = sf::Vector2f((i * mult + 1) * tileSize, (j * mult + 1) * tileSize);
		m_vertices[id + 2].texCoords = sf::Vector2f((tu + 1) * tileSize, (tv + 1) * tileSize);

		m_vertices[id + 3].position = sf::Vector2f(i * tileSize * mult, (j * mult + 1) * tileSize);
		m_vertices[id + 3].texCoords = sf::Vector2f(tu * tileSize, (tv + 1) * tileSize);
		
		return id / 4;
	}
}
 
void SpriteVertex::setPos(int id, sf::Vector2i pos){
	int i = pos.x, j = pos.y;
	id *= 4;
	
	m_vertices[id].position = sf::Vector2f(i * tileSize * mult, j * tileSize * mult);
    m_vertices[id + 1].position = sf::Vector2f((i * mult + 1) * tileSize, j * tileSize * mult);
    m_vertices[id + 2].position = sf::Vector2f((i * mult + 1) * tileSize, (j * mult + 1) * tileSize);
    m_vertices[id + 3].position = sf::Vector2f(i * tileSize * mult, (j * mult + 1) * tileSize);
}

void SpriteVertex::setTile(int id, int tileNumber){
	int tu = tileNumber % (m_tileset->getSize().x / tileSize);
    int tv = tileNumber / (m_tileset->getSize().x / tileSize);
	id *= 4;
	
	m_vertices[id].texCoords = sf::Vector2f(tu * tileSize, tv * tileSize);
	m_vertices[id + 1].texCoords = sf::Vector2f((tu + 1) * tileSize, tv * tileSize);
	m_vertices[id + 2].texCoords = sf::Vector2f((tu + 1) * tileSize, (tv + 1) * tileSize);
	m_vertices[id + 3].texCoords = sf::Vector2f(tu * tileSize, (tv + 1) * tileSize);
}

void SpriteVertex::setTileSize(int size){
	tileSize = size;
}

void SpriteVertex::setMultiplicator(double m){
	mult = m;
}

void SpriteVertex::erase(int id){
	setPos(id, sf::Vector2i(0, 0));
	velocity[id] = offset[id] = sf::Vector2f(0.0, 0.0);
	heap.push(-id);
}

void SpriteVertex::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the tileset texture
    states.texture = m_tileset;

    // draw the vertex array
    if (visibilityMap == nullptr){
    	target.draw(m_vertices, states);
    }else{
		int n = (m_vertices.getVertexCount());
		sf::VertexArray quad(sf::Quads, 4);
		sf::Vector2f pos;
		for(int i = 0; i < n; i+=4){
			pos = m_vertices[i].position / (float)(tileSize * mult);
			//if (pos.x < 0) pos.x = 0;
			//if (pos.x >= (*visibilityMap)[0].size()) pos.x = (*visibilityMap)[0].size() - 1;
			//if (pos.y < 0) pos.y = 0;
			//if (pos.y >= (*visibilityMap).size()) pos.y = (*visibilityMap).size() - 1;
			if ((*visibilityMap)[(int)pos.y][(int)pos.x]){
				quad[0] = m_vertices[i];
				quad[1] = m_vertices[i + 1];
				quad[2] = m_vertices[i + 2];
		   	 	quad[3] = m_vertices[i + 3];
				target.draw(quad, states);
			}
		}
	}
}

