//
// Created by ASUS on 01.06.2023.
//

#ifndef DISCRETEMATHLAB3_VOYAGER_H
#define DISCRETEMATHLAB3_VOYAGER_H

#include <vector>
#include <PathFinder.h>

class Voyager {
private:
    std::vector<std::vector<int>> adj_matrix;
    std::vector<int> shortest;
    int record;
    int start;

    void eval(PathFinder &p);


    std::vector<int> getNbrs(const int &ind);

public:
    Voyager(const std::vector<std::vector<int>> &g);

    int getRecord() const;

    void solveTask(const int &start_vert_ind);

    std::vector<int> getShortest();
};


#endif //DISCRETEMATHLAB3_VOYAGER_H
