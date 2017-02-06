#include "Interface.h"
#include <iostream>
#include <sstream>
#include <stdio.h>

void BaseInterface::loadFrom(Tileset& tileset) 
{
    m_tileset = &(tileset.tileset);
    tileSize = tileset.tileSize;
}

BaseInterface::BaseInterface(sf::Vector2f pos, sf::Vector2i size, Tileset& tileset):
size(size), position(pos)
{
	if (!font.loadFromFile("data/Font.ttf"))
	{
		std::cout << "Bad!";
	}
	if (!backgroundTexture.loadFromFile("data/texture.jpg"))
	{
		std::cout << "Bad!";
	}
	m_tileset = &(tileset.tileset);
    tileSize = tileset.tileSize;
	backgroundTexture.setRepeated(true);
	background.setTexture(backgroundTexture);
	background.setPosition(pos);
	background.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), size));
	rowOffsets.assign(size.y / tileSize, 10);
}

int BaseInterface::append(int row, int tileNumber, std::string description){
	append(sf::Vector2f(rowOffsets[row], row * tileSize), tileNumber, description);
	rowOffsets[row] += icons[icons.size() - 1].getGlobalBounds().width;
	return icons.size() - 1;
}

int BaseInterface::append(sf::Vector2f pos, int tileNumber, std::string description){
	sf::Text descrText;
	descrText.setString(description); 
	//descrText.setPosition(position + pos + sf::Vector2f(tileSize, tileSize));
	descrText.setCharacterSize(tileSize * 2);
	descrText.setFont(font);
	iconsDescriptions.push_back(descrText);
	
	sf::Sprite icon;
	icon.setPosition(position + pos);
	int tu = tileNumber % (m_tileset->getSize().x / tileSize);
    int tv = tileNumber / (m_tileset->getSize().x / tileSize);
    icon.setTexture(*m_tileset);
    icon.setTextureRect(sf::IntRect(tu * tileSize, tv * tileSize, tileSize, tileSize));
    icons.push_back(icon);
    return icons.size() - 1;
}

int BaseInterface::append(int row, std::string text, std::string description){
	append(sf::Vector2f(rowOffsets[row], row * tileSize), text, description);
	rowOffsets[row] += labels[labels.size() - 1].getGlobalBounds().width;
    return labels.size() - 1;
}

int BaseInterface::append(sf::Vector2f pos, std::string text, std::string description){
	sf::Text descrText;
	descrText.setString(description); 
	descrText.setCharacterSize(tileSize * 2);
	//descrText.setPosition(position + pos + sf::Vector2f(tileSize, tileSize));
	descrText.setFont(font);
	labelsDescriptions.push_back(descrText);
	
	sf::Text label;
	label.setFont(font);
	label.setPosition(position + pos);
	label.setCharacterSize(tileSize);
	label.setString(text); 
	labels.push_back(label);
    return labels.size() - 1;
}

void BaseInterface::updateDescription(sf::Vector2f pos){
	curDescription = nullptr;
	for(int i = 0 ; i < labels.size(); i++)
		if (labels[i].getGlobalBounds().contains(pos)){
			curDescription = &labelsDescriptions[i];
			curDescription->setPosition(pos);
			return;
		}
	for(int i = 0 ; i < icons.size(); i++)
		if (icons[i].getGlobalBounds().contains(pos)){
			curDescription = &iconsDescriptions[i];
			curDescription->setPosition(pos);
			return;
		}
}

int BaseInterface::setString(int id, std::string text){
	labels[id].setString(text);
}

int BaseInterface::setLabelDescription(int id, std::string text){
	labelsDescriptions[id].setString(text);
}

int BaseInterface::setString(int id, int textNum){
	char text[20];
	sprintf(text, "%d", textNum);
	labels[id].setString(text);
}

void BaseInterface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background, states);
	for(auto label: labels)
    	target.draw(label, states);
    for(auto icon: icons)
    	target.draw(icon, states);
    if (curDescription != nullptr){
    	target.draw(*curDescription, states);
    }
}

UnitInterface::UnitInterface(sf::Vector2f pos, sf::Vector2i size, Tileset& tileset):
BaseInterface(pos, size, tileset)
{
	nameLabel = append(0, "            ", "");
	append(1, 740, "Health");
	append(2, 742, "Attack");
	append(3, 768, "Level");
	hpLabel = append(1, "   ", "");
	dmgLabel = append(2, "   ", "");
	lvlLabel = append(3, "   ", "");
}

void UnitInterface::setUnit(Unit *punit){
	setString(nameLabel, punit->name);
	setLabelDescription(nameLabel, punit->description);
	setString(hpLabel, punit->hp);
	setString(dmgLabel, punit->dmg);
	setString(lvlLabel, punit->lvl);
}
