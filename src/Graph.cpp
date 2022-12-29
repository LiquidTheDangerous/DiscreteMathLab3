//
// Created by ASUS on 29.12.2022.
//

#include <Graph.h>

std::list<std::pair<Vertex,std::list<Graph::edge>>> Graph::getVertices() const {
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

std::list<Graph::edge> Graph::getNeighbors(const std::string &vertexName) const {
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
    this->vertices.emplace(Vertex(vertexName),std::list<Graph::edge>());
    return true;
}

bool Graph::addVertex(const Graph::VertexT &vertex) {
    return this->addVertex(vertex.getName());
}
