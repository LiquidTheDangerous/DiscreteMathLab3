//
// Created by ASUS on 29.12.2022.
//

#ifndef DISCRETEMATHLAB3_GRAPH_H
#define DISCRETEMATHLAB3_GRAPH_H

#include <Vertex.h>
#include <memory>
#include <map>
#include <set>
#include <optional>
class Graph {

public:
    using VertexT = Vertex;
    using edge = std::pair<VertexT, int>;
    struct UnorientedEdge{
        UnorientedEdge() = default;
        UnorientedEdge(const std::string& first, const std::string& second, int w);
        UnorientedEdge(std::string&& first, std::string&& second, int w);
        std::string firstName;
        std::string secondName;
        int w;
    };
    explicit Graph() = default;

    bool addVertex(const std::string &vertexName);

    bool addVertex(const VertexT &vertex);

    void removeVertex(const std::string& vertexName);

    [[nodiscard]] std::list<std::pair<Vertex,std::list<Graph::edge>>>  getVertices() const;

    [[nodiscard]] bool contains(const std::string &vertexName) const;

    [[nodiscard]] bool contains(const VertexT &vertex) const;

    bool isOriented() const;

    std::list<Graph::UnorientedEdge> getUnorientedEdges() const;

    bool connect(const std::string &sourceName, const std::string &destinationName, int len = 1);

    bool setW(const std::string& sourceName, const std::string& destinationName, int w);

    std::optional<int> getW(const std::string& sourceName, const std::string& destinationName) const;

    bool disconnect(const std::string &sourceName, const std::string &destinationName);

    [[nodiscard]] const std::list<Graph::edge> & getNeighbors(const std::string &vertexName) const;

    [[nodiscard]] std::set<std::string> getVerticesName() const;

    [[nodiscard]] std::size_t getVerticesCount() const;

    [[nodiscard]] std::pair<std::vector<std::vector<int>>, std::vector<std::string>> getIncidentMatrix() const;

    [[nodiscard]] std::vector<std::string> getVerticesVect() const;

    [[nodiscard]] std::optional<std::set<std::string>> getNeighborsNameSet(const std::string& vertexName) const;

    [[nodiscard]] std::map<std::string, size_t> getVerticesIndices() const;
    [[nodiscard]] std::vector<std::vector<int>> _getIncidentMatrix() const;
private:
    std::map<VertexT, std::list<edge>> vertices;

};


#endif //DISCRETEMATHLAB3_GRAPH_H
