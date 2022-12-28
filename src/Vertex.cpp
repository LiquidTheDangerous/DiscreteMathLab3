//
// Created by ASUS on 29.12.2022.
//


#include <Vertex.h>

Vertex::Vertex(const std::string &name) : name(name) {
}

Vertex::Vertex(std::string &&name) noexcept {
    this->name = std::move(name);
}

const std::string &Vertex::getName() const {
    return this->name;
}

bool Vertex::attachNeighbor(Vertex *vertex, int length) {
    auto find_iterator = std::find_if(this->neighbors.begin(), this->neighbors.end(),
                                      [&vertex](const edge &edge) -> bool {
                                          return vertex->name == edge.first->name;
                                      });
    if (find_iterator != this->neighbors.end()) {
        return false;
    }

    this->neighbors.emplace_back(vertex, length);

    return true;
}

bool Vertex::detachNeighborByName(const std::string &vertexName) {
    auto find_iterator = std::find_if(this->neighbors.begin(), this->neighbors.end(),
                                      [&vertexName](const edge &edge) -> bool {
                                          return vertexName == edge.first->name;
                                      });
    if (find_iterator == this->neighbors.end()) {
        return false;
    }
    this->neighbors.erase(find_iterator);
    return true;
}

std::size_t Vertex::getNeighborsCount() const {
    return this->neighbors.size();
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

bool Vertex::containsNeighbor(const std::string &vertexName) const {
    auto find_iterator = std::find_if(this->neighbors.begin(), this->neighbors.end(),
                                      [&vertexName](const edge &edge) -> bool {
                                          return vertexName == edge.first->name;
                                      });
    return find_iterator != this->neighbors.end();
}

[[maybe_unused]] bool Vertex::containsNeighbor(const Vertex &vertex) const {
    auto find_iterator = std::find_if(this->neighbors.begin(), this->neighbors.end(),
                                      [&vertex](const edge &edge) -> bool {
                                          return vertex.name == edge.first->name;
                                      });
    return find_iterator != this->neighbors.end();
}

const std::list<Vertex::edge> &Vertex::getNeighbors() const {
    return neighbors;
}

