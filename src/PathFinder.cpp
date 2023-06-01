//
// Created by ASUS on 01.06.2023.
//

#include <PathFinder.h>
#include <algorithm>

PathFinder::PathFinder(const int &start_vert, const int &amount) : visited_verts(amount, false) {
    this->current_vertex = start_vert;
    this->start_vertex = start_vert;
    this->visited_verts[this->start_vertex] = true;
}

PathFinder::PathFinder(const PathFinder &p) {
    this->start_vertex = p.start_vertex;
    this->current_vertex = p.current_vertex;
    this->path = p.path;
    this->visited_verts = p.visited_verts;
}

int &PathFinder::startVertex() {
    return this->start_vertex;
}

int &PathFinder::currentVertex() {
    return this->current_vertex;
}

bool PathFinder::allVisited() const {
//    return std::all_of(this->visited_verts.begin(), this->visited_verts.end(),[](const bool& i){return i;});
    for (const bool &i: this->visited_verts) {
        if (!i) {
            return false;
        }
    }
    return true;
}

bool PathFinder::wasIn(const int &v) const {
    return this->visited_verts[v];
}

void PathFinder::next(const int &v) {
    if (v >= this->visited_verts.size()) {
        return;
    }
    this->current_vertex = v;
    this->path.push_back(v);
    this->visited_verts[v] = true;
}

std::list<int> &PathFinder::pathList() {
    return this->path;
}