#ifndef SPRITE_VERTEX_INCLUDED
#define SPRITE_VERTEX_INCLUDED
class SpriteVertex : public sf::Drawable
{
public:
    SpriteVertex();
    void loadFrom(Tileset& tileset, std::vector<std::vector<int> > *visMap);
	int append(int i, int j, int tileNumber);
	void setPos(int id, sf::Vector2i pos);

private:
	
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices;
    sf::Texture *m_tileset;
    std::vector<std::vector<int> > *visibilityMap;
    int tileSize;
};
#endif //SPRITE_VERTEX_INCLUDED
