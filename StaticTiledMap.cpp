#include <algorithm>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "StaticTiledMap.h"

		
void StaticTiledMap::draw(sf::RenderTarget& target,sf::RenderStates states)const
{

    int left=0,right=0,top=0,bottom=0;
    //get chunk indices into which top left and bottom right points of view fall:
    sf::Vector2f temp=target.getView().getCenter()-(target.getView().getSize()/2.f);//get top left point of view
    left=static_cast<int>(temp.x/(chunksize*tilesize));
    top=static_cast<int>(temp.y/(chunksize*tilesize));
    temp+=target.getView().getSize();//get bottom right point of view
    right=1+static_cast<int>(temp.x/(chunksize*tilesize));
    bottom=1+static_cast<int>(temp.y/(chunksize*tilesize));
    //clamp these to fit into array bounds:
    left=std::max(0,std::min(left,chunks_x));
    top=std::max(0,std::min(top,chunks_y));
    right=std::max(0,std::min(right,chunks_x));
    bottom=std::max(0,std::min(bottom,chunks_y));
    //set texture and draw visible chunks:
    states.texture=m_texture;
    for(int ix=left;ix<right;++ix)
    {
        for(int iy=top;iy<bottom;++iy)
        {	
            //if (ix >= 0 && iy >= 0 && ix < map_x && iy < map_y && (*visibilityMap)[iy][ix])	
            	target.draw(m_chunks[iy][ix], states);
        }
    }
}

StaticTiledMap::StaticTiledMap(void):
map_x(0),map_y(0),chunks_x(0),chunks_y(0)
{

}

void StaticTiledMap::LoadFrom(Tileset& tileset, std::vector<std::vector<int> > *visMap, int tilemap[], int w, int h)
{
    m_texture = &(tileset.tileset);
    tilesize = tileset.tileSize;
    visibilityMap = visMap;
    map_x = w;
    map_y = h;
    if((map_x*map_y)==0)//empty map - possibly forgotten to fill data struct
    {
        //to stop displaying at all after failed loading:
        chunks_x=0;
        chunks_y=0;
        m_chunks.clear();
        return;
    }
    chunks_x=(map_x/chunksize)+1;
    chunks_y=(map_y/chunksize)+1;
    m_chunks.assign(chunks_y,std::vector<sf::VertexArray>(chunks_x,sf::VertexArray(sf::Quads)));//ready up empty 2d arrays
    int tiles_per_string = (m_texture->getSize().x / tilesize);
    for(int gy=0;gy<map_y;++gy)
    {
        for(int gx=0;gx<map_x;++gx)
        {
            sf::VertexArray *garr = &(m_chunks[gy/chunksize][gx/chunksize]);
             
            int tex_x = tilemap[gy * w + gx] % tiles_per_string,
    		tex_y = tilemap[gy * w + gx] / tiles_per_string;
    	
		    sf::Vertex ver;
		    ver.position=sf::Vector2f(gx*tilesize,gy*tilesize);
		    ver.texCoords=sf::Vector2f(tex_x*tilesize,tex_y*tilesize);
		    garr->append(ver);

		    ver.position=sf::Vector2f(gx*tilesize+tilesize,gy*tilesize);
		    ver.texCoords=sf::Vector2f(tex_x*tilesize+tilesize,tex_y*tilesize);
		    garr->append(ver);

		    ver.position=sf::Vector2f(gx*tilesize+tilesize,gy*tilesize+tilesize);
		    ver.texCoords=sf::Vector2f(tex_x*tilesize+tilesize,tex_y*tilesize+tilesize);	
		    garr->append(ver);

		    ver.position=sf::Vector2f(gx*tilesize,gy*tilesize+tilesize);
		    ver.texCoords=sf::Vector2f(tex_x*tilesize,tex_y*tilesize+tilesize);
		    garr->append(ver);
        }	
    }
    
    for(int gy=0;gy<map_y;++gy)
    {
        for(int gx=0;gx<map_x;++gx)
        {
            setAlpha(sf::Vector2i(gx, gy), 0);
        }
    }
}

void StaticTiledMap::setAlpha(sf::Vector2i pos, int val){
	static int chunk_x, chunk_y, i;
	chunk_x = pos.x/chunksize;
	chunk_y = pos.y/chunksize;
	i = ((pos.y - chunk_y * chunksize) * std::min(chunksize, map_x - chunk_x * chunksize) + pos.x - chunk_x * chunksize) * 4;
	m_chunks[chunk_y][chunk_x][i].color.a = val;
	m_chunks[chunk_y][chunk_x][i+1].color.a = val;
	m_chunks[chunk_y][chunk_x][i+2].color.a = val;
	m_chunks[chunk_y][chunk_x][i+3].color.a = val;
}
