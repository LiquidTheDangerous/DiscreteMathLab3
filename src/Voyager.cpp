//
// Created by ASUS on 01.06.2023.
//

#include <Voyager.h>
#include <limits>

Voyager::Voyager(const std::vector<std::vector<int>> &g) {
    this->adj_matrix = g;
    this->record = std::numeric_limits<int>::max();
}

int Voyager::getRecord() const {
    return this->record;
}

void Voyager::eval(PathFinder &p) {
    if (p.allVisited()) {
        if (!this->adj_matrix[p.currentVertex()][p.startVertex()]) {
            return;
        }

        std::vector<int> path_vect(p.pathList().begin(), p.pathList().end());

        int len = 0;
        int front_vert = path_vect.front();
        int back_vert = path_vect.back();
        len += this->adj_matrix[p.startVertex()][front_vert];
        len += this->adj_matrix[back_vert][p.startVertex()];
        for (int i = 0; i < path_vect.size() - 1; ++i) {
            len += this->adj_matrix[path_vect[i]][path_vect[i + 1]];
        };

        if (this->record > len) {
            this->shortest = path_vect;
            this->record = len;
        }

        return;
    };

    int p_vert(p.currentVertex());
    std::vector<int> nbrs_ind = this->getNbrs(p_vert);

    for (int &i: nbrs_ind) {
        if (!p.wasIn(i)) {
            PathFinder p_cpy(p);
            p_cpy.next(i);
            eval(p_cpy);
        }

    }
}

void Voyager::solveTask(const int &start_vert_ind) {
    this->start = start_vert_ind;
	PathFinder p(start_vert_ind, this->adj_matrix.size());
	this->eval(p);
}

std::vector<int> Voyager::getShortest() {
	std::vector<int> res(this->shortest.size() + 2);
	for (int i = 0; i < this->shortest.size(); ++i)
	{
		res[i + 1] = this->shortest[i];
	};
	res[0] = this->start;
	res[res.size() - 1] = this->start;
	return res;
}

std::vector<int> Voyager::getNbrs(const int &ind) {
	std::list<int> nbrs;

	for (int j = 0; j < this->adj_matrix.size(); ++j)
	{
		if (this->adj_matrix[ind][j])
		{
			nbrs.push_back(j);
		}
	}
	return {nbrs.begin(),nbrs.end()};
}
