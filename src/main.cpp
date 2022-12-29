#include <iostream>
#include <Graph.h>
#include <GraphHelpers.h>

int main() {
//    auto graph = Graph();
//    graph.addVertex("1");
//    graph.addVertex("2");
//    graph.addVertex("3");
//    graph.addVertex("4");
//    graph.connect("1","2");
//    graph.connect("1","3");
//    graph.connect("3","2");
//    graph.connect("3","4");
//    graph.connect("4","3");
//
//    auto matrix = GraphHelpers::StrongConnectMatrix(graph);
//    auto vertices = graph.getVerticesName();
//    std::cout << "  ";
//    for (auto& v : vertices){
//        std::cout << v << ' ';
//    }
//    std::cout << std::endl;
//    for (auto& v1 : vertices){
//        std::cout << v1 << ' ';
//        for (auto& v2 : vertices){
//            std::cout << matrix[v1][v2] << ' ';
//        }
//        std::cout << std::endl;
//    }

    auto graph = Graph();
    graph.addVertex("1");
    graph.addVertex("2");
    graph.addVertex("3");
    graph.addVertex("4");
    graph.addVertex("5");
    graph.addVertex("6");
    graph.addVertex("7");
    graph.addVertex("8");

    graph.connect("1", "2");
    graph.connect("2", "3");
    graph.connect("3", "1");
    graph.connect("3", "4");
    graph.connect("2", "4");
    graph.connect("4", "5");
    graph.connect("5", "4");
    graph.connect("6", "8");
    graph.connect("8", "7");
    graph.connect("7", "8");
    graph.connect("8", "6");
    graph.connect("6", "5");
    graph.connect("7", "5");


    auto matrix = GraphHelpers::StrongConnectMatrix(graph);
    auto vertices = graph.getVerticesName();
    std::cout << "  ";
    for (auto &v: vertices) {
        std::cout << v << ' ';
    }
    std::cout << std::endl;
    for (auto &v1: vertices) {
        std::cout << v1 << ' ';
        for (auto &v2: vertices) {
            std::cout << matrix[v1][v2] << ' ';
        }
        std::cout << std::endl;
    }
}
