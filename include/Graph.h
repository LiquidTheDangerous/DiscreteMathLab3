//
// Created by ASUS on 29.12.2022.
//

#ifndef DISCRETEMATHLAB3_GRAPH_H
#define DISCRETEMATHLAB3_GRAPH_H
#include <unordered_set>
#include <Vertex.h>
class Graph {
public:
    explicit Graph(const std::size_t& verticesCount);

private:
    std::unordered_set<Vertex> vertices;
};


#endif //DISCRETEMATHLAB3_GRAPH_H
