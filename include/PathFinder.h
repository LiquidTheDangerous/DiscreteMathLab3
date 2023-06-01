//
// Created by ASUS on 01.06.2023.
//

#ifndef DISCRETEMATHLAB3_PATHFINDER_H
#define DISCRETEMATHLAB3_PATHFINDER_H
#include <list>
#include <vector>
class PathFinder
{
private:
	int start_vertex;
	int current_vertex;
	std::list<int> path;
	std::vector<bool> visited_verts;

public:
	PathFinder(const int& start_vert, const int& amount);
	PathFinder(const PathFinder& p);

	int& startVertex();
	int& currentVertex();
	bool allVisited() const;
	bool wasIn(const int& v) const;
	void next(const int& v);
	std::list<int>& pathList();
};

#endif //DISCRETEMATHLAB3_PATHFINDER_H
