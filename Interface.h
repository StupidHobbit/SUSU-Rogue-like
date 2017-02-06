#ifndef INTERFACE_INCLUDED
#define INTERFACE_INCLUDED
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Tileset.h"
#include "Units.h"

class Unit;
class BaseInterface: public sf::Drawable
{
public:
    BaseInterface(sf::Vector2f pos, sf::Vector2i size, Tileset& tileset);
    void loadFrom(Tileset& tileset);
    int append(int row, int tileNumber, std::string description);
    int append(int row, std::string text, std::string description);
	int append(sf::Vector2f pos, int tileNumber, std::string description);
	int append(sf::Vector2f pos, std::string text, std::string description);
	int setString(int id, std::string text);
	int setString(int id, int textNum);
	int setLabelDescription(int id, std::string text);
	void updateDescription(sf::Vector2f pos);

//private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
    int tileSize;
	sf::Texture *m_tileset;
	sf::Texture backgroundTexture;
	sf::Sprite background;
	sf::Vector2f position;
	sf::Vector2i size;
    std::vector<sf::Text> labels;
    std::vector<sf::Text> labelsDescriptions;
    std::vector<sf::Sprite> icons;
    std::vector<sf::Text> iconsDescriptions;
    sf::Text *curDescription;
    sf::Font font;
    std::vector<int> rowOffsets;
};

class UnitInterface: public BaseInterface
{
public:
    UnitInterface(sf::Vector2f pos, sf::Vector2i size, Tileset& tileset);
    void setUnit(Unit *punit);
    
    int hpLabel, dmgLabel, nameLabel, lvlLabel;
	Unit *punit;
};

#endif //INTERFACE_INCLUDED
