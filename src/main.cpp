#include <iostream>
#include <Application.h>
#include <GraphHelpers.h>

int main() {

//    Graph g;
//    g.addVertex("2");
//    g.addVertex("3");
//    g.addVertex("5");
//    g.addVertex("7");
//    g.addVertex("8");
//    g.addVertex("9");
//    g.addVertex("10");
//    g.addVertex("11");
//
//    g.connect("3","8");
//    g.connect("3","10");
//    g.connect("5","11");
//    g.connect("7","11");
//    g.connect("7","8");
//    g.connect("8","9");
//    g.connect("11","2");
//    g.connect("11","9");
//    g.connect("11","10");
//    auto sort = GraphHelpers::TopologicalSort(g);
//    if (sort) {
//        std::cout << "Sort: " << std::endl;
//        auto& map = *sort;
//        for (auto &&order: map) {
//            std::cout << order.first << "-> ";
//            for (auto &&v: order.second) {
//                std::cout << v << ' ';
//            }
//            std::cout << std::endl;
//        }
//    }
//    else{
//        std::cout << "Couldn't sort" << std::endl;
//    }
    auto app = Application(800,600,"title");
    app.run();

    return 0;
}
