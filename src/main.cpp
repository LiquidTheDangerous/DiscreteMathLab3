#include <iostream>
#include <Graph.h>
#include <GraphHelpers.h>
int main() {
    auto graph = Graph();

    graph.addVertex("1");
    graph.addVertex("2");
    graph.addVertex("3");
    graph.addVertex("4");
    graph.connect("1","2");
    graph.connect("1","3");
    graph.connect("3","2");
    graph.connect("3","4");
    graph.connect("4","3");

    auto matrix = GraphHelpers::ReachMatrix(graph);
    auto vertices = graph.getVerticesName();
    std::cout << "  ";
    for (auto& v : vertices){
        std::cout << v << ' ';
    }
    std::cout << std::endl;
    for (auto& v1 : vertices){
        std::cout << v1 << ' ';
        for (auto& v2 : vertices){
            std::cout << matrix[v1][v2] << ' ';
        }
        std::cout << std::endl;
    }

//    auto
}
