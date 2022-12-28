//
// Created by ASUS on 29.12.2022.
//

#ifndef DISCRETEMATHLAB3_VERTEX_H
#define DISCRETEMATHLAB3_VERTEX_H

#include <string>
#include <list>
#include <functional>

class Vertex {
public:
    using edge = std::pair<Vertex *, int>;

    explicit Vertex(const std::string &name);

    explicit Vertex(std::string &&name) noexcept;

    [[nodiscard]] const std::string &getName() const;

    bool attachNeighbor(Vertex *vertex, int length = 1);

    bool detachNeighborByName(const std::string &vertexName);

    [[nodiscard]] const std::list<edge> &getNeighbors() const;

    [[nodiscard]] std::size_t getNeighborsCount() const;

    [[nodiscard]] bool containsNeighbor(const std::string &name) const;

    [[maybe_unused]] [[nodiscard]] bool containsNeighbor(const Vertex &vertex) const;

    bool operator==(const Vertex &vertex) const;

    bool operator!=(const Vertex &vertex) const;

    bool operator<(const Vertex &vertex) const;

    bool operator<=(const Vertex &vertex) const;

    bool operator>(const Vertex &vertex) const;

    bool operator>=(const Vertex &vertex) const;

    friend class std::hash<Vertex>;

private:
    std::string name;
    std::list<edge> neighbors;

};

template<>
struct std::hash<Vertex> {
    std::size_t operator()(Vertex const &vertex) const noexcept {
        return std::hash<std::string>{}(vertex.name);;
    }
};


#endif //DISCRETEMATHLAB3_VERTEX_H
