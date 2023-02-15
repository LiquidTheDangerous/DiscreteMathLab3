//
// Created by ASUS on 29.12.2022.
//

#include <Graph.h>
#include <iostream>

std::list<std::pair<Vertex, std::list<Graph::edge>>> Graph::getVertices() const {
    return {this->vertices.begin(), this->vertices.end()};
}

bool Graph::contains(const std::string &vertexName) const {
    return this->vertices.find(VertexT(vertexName)) != this->vertices.end();
}

bool Graph::contains(const Vertex &vertex) const {
    return this->vertices.contains(vertex);
}

bool Graph::connect(const std::string &sourceName, const std::string &destinationName, int len) {
    auto sourceIt = this->vertices.find(VertexT(sourceName));
    if (sourceIt == this->vertices.end())
        return false;

    auto destinationIt = this->vertices.find(VertexT(destinationName));
    if (destinationIt == this->vertices.end())
        return false;

    sourceIt->second.emplace_back(destinationIt->first, len);

    return true;
}

const std::list<Graph::edge> &Graph::getNeighbors(const std::string &vertexName) const {
    auto sourceIt = this->vertices.find(VertexT(vertexName));
    if (sourceIt == this->vertices.end())
        throw std::runtime_error("Vertex not found");
    return sourceIt->second;
}

bool Graph::disconnect(const std::string &sourceName, const std::string &destinationName) {
    auto sourceIt = this->vertices.find(VertexT(sourceName));
    if (sourceIt == this->vertices.end())
        return false;

    auto destinationIt = this->vertices.find(VertexT(destinationName));
    if (destinationIt == this->vertices.end())
        return false;

    auto it = std::find_if(sourceIt->second.begin(), sourceIt->second.end(),
                           [&destinationIt](const Graph::edge &e) -> bool {
                               return destinationIt->first.getName() == e.first.getName();
                           });
    if (it == sourceIt->second.end())
        return false;

    sourceIt->second.erase(it);

    return true;
}

bool Graph::addVertex(const std::string &vertexName) {
    if (this->contains(vertexName))
        return false;
    this->vertices.emplace(Vertex(vertexName), std::list<Graph::edge>());
    return true;
}

bool Graph::addVertex(const Graph::VertexT &vertex) {
    return this->addVertex(vertex.getName());
}

std::set<std::string> Graph::getVerticesName() const {
    std::set<std::string> result;
    for (auto &v: this->vertices) {
        result.insert(v.first.getName());
    }
    return result;
}

void Graph::removeVertex(const std::string &vertexName) {
    for (auto &e: this->vertices) {
        auto iter = std::remove_if(e.second.begin(), e.second.end(), [&vertexName](Graph::edge &item) -> bool {
            return item.first.getName() == vertexName;
        });
        e.second.erase(iter, e.second.end());
    }

    auto count = std::erase_if(this->vertices, [&vertexName](const auto &item) -> bool {
        auto const &[key, val] = item;
        return key.getName() == vertexName;
    });
}

std::size_t Graph::getVerticesCount() const {
    return this->vertices.size();
}

std::pair<std::vector<std::vector<int>>, std::vector<std::string>> Graph::getIncidentMatrix() const {
    auto map = getVerticesVect();
    std::vector<std::vector<int>> matrixResult = _getIncidentMatrix();
    return {std::move(matrixResult), std::move(map)};
}

std::vector<std::vector<int>> Graph::_getIncidentMatrix() const {
    int i = 0;
    auto matrixResult = std::vector<std::vector<int>>(getVerticesCount(),
                                                      std::vector<int>(getVerticesCount(), 0));

    for (auto &&v1: vertices) {
        int j = 0;
        for (auto &&v2: vertices) {
            const auto iter = std::find_if(v1.second.begin(), v1.second.end(),
                                           [&v2](const edge &e) -> bool {
                                               return e.first.getName() == v2.first.getName();
                                           });

            if (iter != v1.second.end()) {
                matrixResult[i][j] = iter->second;
            } else {
                matrixResult[i][j] = 0;
            }
            ++j;
        }
        ++i;
    }
    return matrixResult;
}

std::vector<std::string> Graph::getVerticesVect() const {
    auto vector = std::vector<std::string>(this->getVerticesCount());
    std::size_t c = 0;
    for (auto &&v: vertices) {
        vector[c++] = (v.first.getName());
    }
    return vector;
}

bool Graph::setW(const std::string &sourceName, const std::string &destinationName, int w) {
    auto sourceIt = this->vertices.find(VertexT(sourceName));
    if (sourceIt == this->vertices.end())
        return false;

    auto destinationIt = this->vertices.find(VertexT(destinationName));
    if (destinationIt == this->vertices.end())
        return false;

    auto iter = std::find_if(sourceIt->second.begin(), sourceIt->second.end(),
                             [destinationName](const Graph::edge &e) -> bool {
                                 return e.first.getName() == destinationName;
                             });
    if (iter == sourceIt->second.end()) {
        this->connect(sourceName, destinationName, w);
    } else {
        iter->second = w;
    }

    return true;
}

std::optional<int> Graph::getW(const std::string &sourceName, const std::string &destinationName) const {
    auto sourceIt = this->vertices.find(VertexT(sourceName));
    if (sourceIt == this->vertices.end())
        return std::nullopt;
    auto destinationIt = this->vertices.find(VertexT(destinationName));
    if (destinationIt == this->vertices.end())
        return std::nullopt;
    auto iter = std::find_if(sourceIt->second.begin(), sourceIt->second.end(),
                             [destinationName](const Graph::edge &e) -> bool {
                                 return e.first.getName() == destinationName;
                             });
    if (iter == sourceIt->second.end()) {
        return std::nullopt;
    }
    return std::make_optional<int>(iter->second);
}

std::map<std::string, std::size_t> Graph::getVerticesIndices() const {
    std::map<std::string, std::size_t> result;
    std::size_t c{0};
    for (auto &v: this->vertices) {
        result[v.first.getName()] = c++;
    }
    return result;
}

bool Graph::isOriented() const {

    for (auto &&v1: vertices) {
        for (auto &&v2: vertices) {
            const auto iter1 = std::find_if(v1.second.begin(), v1.second.end(),
                                            [&v2](const edge &e) -> bool {
                                                return e.first.getName() == v2.first.getName();
                                            });
            const auto iter2 = std::find_if(v2.second.begin(), v2.second.end(),
                                            [&v1](const edge &e) -> bool {
                                                return e.first.getName() == v1.first.getName();
                                            });

            if (iter1 != v1.second.end()) {
                if (iter2 != v2.second.end()) {
                    if (iter2->second != iter1->second) {
                        return true;
                    }
                } else {
                    return true;
                }
            }
        }
    }
    return false;
}

std::list<Graph::UnorientedEdge> Graph::getUnorientedEdges() const {
    std::list<Graph::UnorientedEdge> result;

    for (auto &&v1: vertices) {
        for (auto &&v2: vertices) {
            const auto iter1 = std::find_if(v1.second.begin(), v1.second.end(),
                                            [&v2](const edge &e) -> bool {
                                                return e.first.getName() == v2.first.getName();
                                            });
            const auto iter2 = std::find_if(v2.second.begin(), v2.second.end(),
                                            [&v1](const edge &e) -> bool {
                                                return e.first.getName() == v1.first.getName();
                                            });

            if (iter1 != v1.second.end()) {
                if (iter2 != v2.second.end()) {
                    if (iter2->second == iter1->second) {
                        result.emplace_back(iter1->first.getName(), iter2->first.getName(), iter1->second);
                    }
                }
            }
        }
    }


    return result;
}

std::optional<std::set<std::string>> Graph::getNeighborsNameSet(const std::string &vertexName) const {
    std::set<std::string> result;
    auto iter = this->vertices.find(Vertex(vertexName));
    if (iter != this->vertices.end()){
        for (auto& nbr : iter->second){
            result.insert(nbr.first.getName());
        }
        return std::move(result);
    }

    return std::nullopt;
}

Graph::UnorientedEdge::UnorientedEdge(const std::string &first, const std::string &second, int w) :
        firstName(first),
        secondName(second),
        w(w) {
}

Graph::UnorientedEdge::UnorientedEdge(std::string &&first, std::string &&second, int w) :
        firstName(std::move(first)),
        secondName(std::move(second)),
        w(w) {
}
