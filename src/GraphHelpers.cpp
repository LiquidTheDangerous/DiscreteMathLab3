//
// Created by ASUS on 29.12.2022.
//

#include <iostream>
#include <GraphHelpers.h>
#include <limits>

void GraphHelpers::_dfs(const Graph &graph, std::set<std::string> &visited, const std::string &startVertexName) {
    auto iter = visited.insert(startVertexName);
    if (iter.second) {
//        std::cout << *iter.first;
        auto nbrs = graph.getNeighbors(startVertexName);
        for (auto &nbr: nbrs) {
            _dfs(graph, visited, nbr.first.getName());
        }
    }
}

std::map<std::string, std::map<std::string, bool>> GraphHelpers::ReachMatrix(const Graph &graph) {
    auto vertices = graph.getVerticesName();
    auto result = std::map<std::string, std::map<std::string, bool>>();
    for (auto &v: vertices) {
        auto reachable = std::set<std::string>();
        _dfs(graph, reachable, v);
        auto row = std::map<std::string, bool>();
        for (auto &v_reach: vertices) {
            if (reachable.contains(v_reach)) {
                row[v_reach] = true;
            } else {
                row[v_reach] = false;
            }
        }
        result[v] = std::move(row);
    }
    return result;
}

std::map<std::string, std::map<std::string, bool>> GraphHelpers::StrongConnectMatrix(const Graph &graph) {
    auto reachMatrix = ReachMatrix(graph);
    auto vertices = graph.getVerticesName();

    auto result = std::map<std::string, std::map<std::string, bool>>();

    for (auto &v1: vertices) {
        for (auto &v2: vertices) {
            result[v1][v2] = reachMatrix[v1][v2] && reachMatrix[v2][v1];
        }
    }

    return result;
}

std::set<std::set<std::string>> GraphHelpers::BoundComponents(const Graph &graph) {
    auto uncoveredVertices = graph.getVerticesName();
    auto vertices = graph.getVerticesName();
    auto result = std::set<std::set<std::string>>();
    auto boundMatrix = StrongConnectMatrix(graph);
    for (auto &v1: vertices) {
        if (!uncoveredVertices.contains(v1))
            continue;
        auto component = std::set<std::string>();
        for (auto &v2: vertices) {
            if (boundMatrix[v1][v2]) {
                uncoveredVertices.erase(v2);
                component.insert(v2);
            }
        }
        if (!component.empty()) {
            result.insert(component);
        }
    }
    return result;
}

std::optional<std::map<int, std::set<std::string>>> GraphHelpers::TopologicalSort(const Graph &graph) {
    using ReturnType = std::optional<std::map<int, std::set<std::string>>>;
    if (graph.getVerticesCount() == 0) {
        return std::nullopt;
    }
    if (HasCycles(graph)) {
        return std::nullopt;
    }
    auto result = std::map<int, std::set<std::string>>();
    auto incidentMatrix = graph.getIncidentMatrix();
    auto &matrix = incidentMatrix.first;
    auto &map = incidentMatrix.second;
    auto c = 0;

    while (true) {
        auto nullColumns = findNullColumns(matrix);
        if (nullColumns.empty()) {
            break;
        }
        auto vertexSet = std::set<std::string>();
        for (auto &&col: nullColumns) {
            vertexSet.emplace(map[col]);
        }
        result[c++] = std::move(vertexSet);
        removeNullColumns(matrix, nullColumns);

    }
    return ReturnType{std::move(result)};
}

bool GraphHelpers::HasCycles(const Graph &graph) {
    auto components = BoundComponents(graph);
    auto vCount = graph.getVerticesCount();
    return components.size() != vCount;
}

std::list<std::size_t> GraphHelpers::findNullColumns(const std::vector<std::vector<int>> &incidentMatrix) {
    auto result = std::list<std::size_t>();
    for (int j = 0; j < incidentMatrix.size(); ++j) {
        bool flag = true;
        for (int i = 0; i < incidentMatrix.size() && flag; ++i) {
            flag &= incidentMatrix[i][j] == 0;
        }
        if (flag) {
            result.emplace_back(j);
        }
    }
    return result;
}

void GraphHelpers::removeNullColumns(std::vector<std::vector<int>> &incidentMatrix,
                                     const std::list<std::size_t> &nullColumns) {
    for (auto &&j: nullColumns) {
        for (auto &&i: incidentMatrix) {
            i[j] = removed;
        }
    }
    for (auto &&i: nullColumns) {
        for (std::size_t j = 0; j < incidentMatrix.size(); ++j) {
            if (incidentMatrix[i][j] != removed) {
                incidentMatrix[i][j] = null;
            }
        }
    }
}

std::optional<std::map<std::string, int>>
GraphHelpers::Dijkstra(const Graph &graph, const std::string &startVertexName) {
    if (!graph.contains(startVertexName)) {
        return std::nullopt;
    }
    std::map<std::string, int> result;
    auto vertices = graph.getVerticesVect();
    auto quoted = std::vector<bool>(graph.getVerticesCount(), false);
    auto vertexCost = std::vector<int>(graph.getVerticesCount(), GraphHelpers::infitity);
    auto verticesIndices = graph.getVerticesIndices();
    vertexCost[verticesIndices[startVertexName]] = 0;
    const auto *currentVertex = &startVertexName;

    for (;;) {

        //mark current vertex
        quoted[verticesIndices[*currentVertex]] = true;

        for (const auto &neighbor: graph.getNeighbors(*currentVertex)) {

            //Edge weight
            const auto &w = neighbor.second;
            const auto &currentVertexCost = vertexCost[verticesIndices[*currentVertex]];
            auto &neighborVertexCost = vertexCost[verticesIndices[neighbor.first.getName()]];

            //Try update vertex cost
            if (w + currentVertexCost < neighborVertexCost) {
                neighborVertexCost = w + currentVertexCost;
            }
        }

        //find next current vertex
        auto minCost = GraphHelpers::infitity;
        auto index = -1;
        int c = 0;
        for (const auto &cost: vertexCost) {
            if (!quoted[c] && cost < minCost) {
                minCost = cost;
                index = c;
            }
            ++c;
        }

        //if index not found then break
        if (index == -1) {
            break;
        }

        currentVertex = &vertices[index];

    }

    int c = 0;
    for (const auto &vertex: vertices) {

        result[vertex] = vertexCost[c];
        ++c;
    }

    return std::move(result);
}

std::optional<std::list<std::string>>
GraphHelpers::DijkstraPath(const Graph &graph, const std::string &sourceVertexName,
                           const std::string &destinationVertexName) {
    if (!graph.contains(sourceVertexName)) {
        return std::nullopt;
    }
    if (!graph.contains(destinationVertexName)) {
        return std::nullopt;
    }
    auto dijkstraResult = GraphHelpers::Dijkstra(graph, sourceVertexName);
    if (!dijkstraResult) {
        return std::nullopt;
    }
    if (dijkstraResult.value()[sourceVertexName] == GraphHelpers::infitity){
        return std::nullopt;
    }
    if (dijkstraResult.value()[destinationVertexName] == GraphHelpers::infitity){
        return std::nullopt;
    }
    auto result = std::list<std::string>();
    auto io = graph.getIncidentMatrix();
    auto &matrix = io.first;
    auto &vertices = io.second;
    auto indices = graph.getVerticesIndices();
    auto *currentVertex = &destinationVertexName;

    const auto verticesCount = graph.getVerticesCount();
    for (;;) {
        std::size_t j = indices[*currentVertex];
        result.push_front(*currentVertex);
        if (*currentVertex == sourceVertexName){
            break;
        }
        for (int i = 0; i < verticesCount; ++i) {
            if (matrix[i][j] == 0) {
                continue;
            }
            const auto& edgeWeight = matrix[i][j];
            const auto& currentVertexWeight = dijkstraResult.value()[*currentVertex];
            const auto& nbrVertexWeight = dijkstraResult.value()[vertices[i]];
            if (nbrVertexWeight +edgeWeight == currentVertexWeight){
                currentVertex = &vertices[i];
                break;
            }
        }
    }


    return std::move(result);
}
