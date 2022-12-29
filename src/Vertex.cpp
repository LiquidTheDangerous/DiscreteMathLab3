//
// Created by ASUS on 29.12.2022.
//


#include <Vertex.h>

Vertex::Vertex(const std::string &name) : name(name) {
}

Vertex::Vertex(std::string &&name) noexcept(true) {
    this->name = std::move(name);
}

const std::string &Vertex::getName() const {
    return this->name;
}

bool Vertex::operator==(const Vertex &vertex) const {
    return this->name == vertex.name;
}

bool Vertex::operator!=(const Vertex &vertex) const {
    return this->name != vertex.name;
}

bool Vertex::operator<(const Vertex &vertex) const {
    return this->name < vertex.name;
}

bool Vertex::operator<=(const Vertex &vertex) const {
    return this->name <= vertex.name;
}

bool Vertex::operator>(const Vertex &vertex) const {
    return this->name > vertex.name;
}

bool Vertex::operator>=(const Vertex &vertex) const {
    return this->name >= vertex.name;
}

//bool Vertex::containsNeighbor(const std::string &vertexName) const {
//    auto find_iterator = std::find_if(this->neighbors.begin(), this->neighbors.end(),
//                                      [&vertexName](const edge &edge) -> bool {
//                                          return vertexName == edge.first->name;
//                                      });
//    return find_iterator != this->neighbors.end();
//}
//
//[[maybe_unused]] bool Vertex::containsNeighbor(const Vertex &vertex) const {
//    auto find_iterator = std::find_if(this->neighbors.begin(), this->neighbors.end(),
//                                      [&vertex](const edge &edge) -> bool {
//                                          return vertex.name == edge.first->name;
//                                      });
//    return find_iterator != this->neighbors.end();
//}
//
//const std::list<Vertex::edge> &Vertex::getNeighbors() const {
//    return neighbors;
//}

