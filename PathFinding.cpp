#include "PathFinding.h"
#include<cmath>
#include<iostream>

inline bool check(sf::Vector2i &a, int w, int h){
	return a.x >= 0 && a.y >= 0 && a.x < w && a.y < h;
}

inline int get_del(sf::Vector2i &a, sf::Vector2i &b){
    return std::abs(a.x-b.x)+std::abs(a.y-b.y);
}

struct Point{
    int F;
    sf::Vector2i v;
    Point(const int &F, const sf::Vector2i &v):F(F), v(v){}
    friend inline bool operator<(const Point &a, const Point &b){
        return a.F>=b.F;
    }
};

bool find_path(std::vector< std::vector<char>> &map, sf::Vector2i &start, sf::Vector2i &place, std::stack <sf::Vector2i> &path) {
	sf::Vector2i del[] = {sf::Vector2i(0, 1), sf::Vector2i(0, -1), sf::Vector2i(1, 0), sf::Vector2i(-1, 0)};
	const int INF = 1e9;
	int w = map[0].size(), h = map.size();
	std::vector<std::vector<int>> d (h, std::vector<int>(w, INF));
	std::vector<std::vector<sf::Vector2i>> p (h, std::vector<sf::Vector2i>(w));
	d[start.y][start.x] = 0;
	std::priority_queue < Point> q;
	q.push (Point(get_del(start, place), start));
	bool flag = false;
	while (!q.empty()) {
        if (flag)
            break;
		sf::Vector2i v = q.top().v;
		int cur_d = q.top().F-get_del(v, place);
		q.pop();
		if (cur_d > d[v.y][v.x])  continue;
		for(int i=0;i<4;i++){
            sf::Vector2i to=v+del[i];
            if(check(to, w, h) && !map[to.y][to.x] && d[v.y][v.x]+1<d[to.y][to.x]){
                d[to.y][to.x]=d[v.y][v.x]+1;
                p[to.y][to.x]=v;
                q.push(Point(d[to.y][to.x]+get_del(to, place), to));
                if (to == place) {
					flag = true;
					break;
				}
            }
		}
	}
	if (!flag)
        return false;
    while (!path.empty())
        path.pop();
    sf::Vector2i cback=place;
	while (cback != start) {
		for (int i = 0; i < 4; i++) {
			sf::Vector2i to = cback+del[i];
            if (check(to, w, h) && d[cback.y][cback.x] == d[to.y][to.x] + 1) {
				path.push(cback);
				cback = to;
				break;
			}
		}
	}
	return true;
}

bool astar_find_path(Location *location, sf::Vector2i &start, sf::Vector2i &place, std::stack <sf::Vector2i> &path) {
	sf::Vector2i del[] = {sf::Vector2i(0, 1), sf::Vector2i(0, -1), sf::Vector2i(1, 0), sf::Vector2i(-1, 0)};
	const int INF = 1e9;
	int w = location->map[0].size(), h = location->map.size();
	std::vector<std::vector<int>> d (h, std::vector<int>(w, INF));
	std::vector<std::vector<sf::Vector2i>> p (h, std::vector<sf::Vector2i>(w));
	d[start.y][start.x] = 0;
	std::priority_queue < Point> q;
	q.push (Point(get_del(start, place), start));
	bool flag = false;
	while (!q.empty()) {
        if (flag)
            break;
		sf::Vector2i v = q.top().v;
		int cur_d = q.top().F-get_del(v, place);
		q.pop();
		if (cur_d > d[v.y][v.x])  continue;
		for(int i=0;i<4;i++){
            sf::Vector2i to=v+del[i];
            if(check(to, w, h) && !location->map[to.y][to.x] && (location->unitsMap[to.y][to.x]==NULL || to==place || get_del(to, start)>=6) && d[v.y][v.x]+1<d[to.y][to.x]){
                d[to.y][to.x]=d[v.y][v.x]+1;
                p[to.y][to.x]=v;
                q.push(Point(d[to.y][to.x]+get_del(to, place), to));
                if (to == place) {
					flag = true;
					break;
				}
            }
		}
	}
	if (!flag)
        return false;
    while (!path.empty())
        path.pop();
    sf::Vector2i cback=place;
	while (cback != start) {
		for (int i = 0; i < 4; i++) {
			sf::Vector2i to = cback+del[i];
            if (check(to, w, h) && d[cback.y][cback.x] == d[to.y][to.x] + 1) {
				path.push(cback);
				cback = to;
				break;
			}
		}
	}
	return true;
}
/*
bool stup_find_path(std::vector< std::vector<char>> &map, sf::Vector2i &start, sf::Vector2i &place, std::stack <sf::Vector2i> &path) {

	sf::Vector2i del[] = {sf::Vector2i(0, 1), sf::Vector2i(0, -1), sf::Vector2i(1, 0), sf::Vector2i(-1, 0)};
	const int INF = 1e9;
	int w = map[0].size(), h = map.size();
	std::vector<std::vector<int>> d (h, std::vector<int>(w, INF));
	std::vector<std::vector<sf::Vector2i>> p (h, std::vector<sf::Vector2i>(w));
	d[start.y][start.x] = 0;
	std::priority_queue < Point> q;
	q.push (Point(get_del(start, place), start));
	bool flag = false;
	while (!q.empty()) {
        if (flag)
            break;
		sf::Vector2i v = q.top().v;
		int cur_d = q.top().F-get_del(v, place);
		q.pop();
		if (cur_d > d[v.y][v.x])  continue;
		for(int i=0;i<4;i++){
            sf::Vector2i to=v+del[i];
            if(check(to, w, h) && map[to.y][to.x]!=1 && d[v.y][v.x]+1<d[to.y][to.x]){
                d[to.y][to.x]=d[v.y][v.x]+1;
                p[to.y][to.x]=v;
                q.push(Point(d[to.y][to.x]+get_del(to, place), to));
                if (to == place) {
					flag = true;
					break;
				}
            }
		}
	}
	if (!flag)
        return false;
    while (!path.empty())
        path.pop();
    sf::Vector2i cback=place;
	while (cback != start) {
		for (int i = 0; i < 4; i++) {
			sf::Vector2i to = cback+del[i];
            if (check(to, w, h) && d[cback.y][cback.x] == d[to.y][to.x] + 1) {
				path.push(cback);
				cback = to;
				break;
			}
		}
	}
	return true;
}*/
