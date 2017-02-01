#include "PathFinding.h"

bool check(int x, int y, int w, int h){
	return x >= 0 && y >= 0 && x < w && y < h; 
}

bool find_path(std::vector< std::vector<char>> &map, sf::Vector2i start, sf::Vector2i place, std::stack <sf::Vector2i> &path) {
	int dx[] = { 0, 1, 0, -1 };
	int dy[] = { 1, 0, -1, 0 };
	bool flag = false;
	int w = map[0].size(), h = map.size();
	std::vector<std::vector<int>> mark(h, std::vector<int>(w, (int) 1e9));
	mark[start.y][start.x] = 0;
	std::queue<sf::Vector2i> q;
	q.push(start);
	while (!q.empty()) {
		if (flag) {
			break;
		}
		sf::Vector2i v = q.front();
		q.pop();
		for (int i = 0; i < 4; i++) {
			sf::Vector2i to = sf::Vector2i(v.x + dx[i], v.y + dy[i]);
			if (check(to.x, to.y, w, h) && map[to.y][to.x] != 1 && mark[v.y][v.x] + 1 < mark[to.y][to.x]) {
				mark[to.y][to.x] = mark[v.y][v.x] + 1;
				q.push(to);
				if (to == place) {
					flag = true;
					break;
				}
			}
		}
	}
	if (!flag) {
		return false;
	}
        while (!path.empty()) {
		path.pop();
	}
	while (place != start) {
		for (int i = 0; i < 4; i++) {
			sf::Vector2i to = sf::Vector2i(place.x + dx[i], place.y + dy[i]);
			if (check(to.x, to.y, w, h) && mark[place.y][place.x] == mark[to.y][to.x] + 1) {
				path.push(place);
				place = to;
				break;
			}
		}
	}
	return true;
}
