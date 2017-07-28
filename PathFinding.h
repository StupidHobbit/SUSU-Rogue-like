#ifndef PATH_FINDING
#define PATH_FINDING

#include <vector>
#include <stack>
#include <queue>
#include <SFML/System/Vector2.hpp>
#include "Location.h"

inline bool check(sf::Vector2i &a, int w, int h);

bool find_path(std::vector< std::vector<char>> &map, sf::Vector2i &start, sf::Vector2i &place, std::stack <sf::Vector2i> &path);

bool astar_find_path(Location *location, sf::Vector2i &start, sf::Vector2i &place, std::stack <sf::Vector2i> &path);

#endif
