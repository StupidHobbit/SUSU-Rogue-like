#ifndef PATH_FINDING
#define PATH_FINDING

#include <vector>
#include <stack>
#include <queue>
#include <SFML/System/Vector2.hpp>

bool check(int x, int y, int w, int h);
bool find_path(std::vector< std::vector<char>> &Map, sf::Vector2i start, sf::Vector2i place, std::stack <sf::Vector2i> &path);

#endif 
