#include <iostream>
#include <Application.h>
#include <GraphHelpers.h>

int main() {

    auto app = Application(800,600,"title");
    app.run();
//    std::vector<std::vector<int>> m = {{0,0,3,1},{0,0,0,0},{0,2,0,3},{0,0,0,0}};
//    GraphHelpers::ShimbellPowMatrix(m,2);
//    for (auto& i : m){
//        for (auto &j : i){
//            std::cout << j << ' ';
//        }
//        std::cout << std::endl;
//    }
    return 0;
}
