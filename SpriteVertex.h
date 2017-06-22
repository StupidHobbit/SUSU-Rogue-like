#ifndef SPRITE_VERTEX_INCLUDED
#define SPRITE_VERTEX_INCLUDED
#include <queue>
class SpriteVertex : public sf::Drawable
{
public:
    SpriteVertex();
    void loadFrom(Tileset& tileset, std::vector<std::vector<int> > *visMap);
    void loadFrom(Tileset& tileset);
	int append(int i, int j, int tileNumber, sf::Vector2f v = sf::Vector2f(0.0, 0.0), sf::Vector2f ofs = sf::Vector2f(0.0, 0.0));
	void setPos(int id, sf::Vector2i pos);
	void setTile(int id, int TileNum);
	void setMultiplicator(double mult);
	void setTileSize(int size);
	void erase(int id);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	double mult;
	std::priority_queue<int> heap;
    sf::VertexArray m_vertices;
    sf::Texture *m_tileset;
    std::vector<std::vector<int> > *visibilityMap;
    std::vector<sf::Vector2f> velocity, offset;
    int tileSize;
};
#endif //SPRITE_VERTEX_INCLUDED
