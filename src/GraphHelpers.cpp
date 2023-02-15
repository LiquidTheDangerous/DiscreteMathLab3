//
// Created by ASUS on 29.12.2022.
//

#include <iostream>
#include <GraphHelpers.h>
#include <limits>
#include <exception>

const GraphHelpers::Bfunc GraphHelpers::ShimbellMult = [](const int &first, const int &second) -> int {
    if (first == 0 || second == 0) {
        return 0;
    }
    return first + second;
};
const GraphHelpers::Bfunc GraphHelpers::ShimbellPlusMin = [](const int &first, const int &second) -> int {
    if (first == 0) {
        return second;
    }
    if (second == 0) {
        return first;
    }
    return std::min(first, second);
};
const GraphHelpers::Bfunc GraphHelpers::ShimbellPlusMax = [](const int &first, const int &second) -> int {
    if (first == 0) {
        return second;
    }
    if (second == 0) {
        return first;
    }
    return std::max(first, second);
};

GraphHelpers::Matrix
GraphHelpers::matrixMultiplication(const GraphHelpers::Matrix &first, const GraphHelpers::Matrix &second,
                                   const Bfunc &sum,
                                   const Bfunc &produce) noexcept(false) {
    if (first[0].size() != second.size()) {
        throw std::invalid_argument("Invalid arguments");
    }


    auto result = std::vector<std::vector<int>>(first.size(), std::vector<int>(second[0].size(), 0));
    for (int i = 0; i < first.size(); ++i)
        for (int j = 0; j < second[0].size(); ++j) {
            result[i][j] = 0;
            for (int k = 0; k < first[0].size(); ++k) {
                auto mult = produce(first[i][k], second[k][j]);
                result[i][j] = sum(result[i][j], mult);
            }
        }
    return result;
}

void GraphHelpers::matrixMultiplication(const GraphHelpers::Matrix &first, const GraphHelpers::Matrix &second,
                                        GraphHelpers::Matrix &result, const GraphHelpers::Bfunc &sum,
                                        const GraphHelpers::Bfunc &produce) noexcept(false) {
    if (first[0].size() != second.size()) {
        throw std::invalid_argument("Invalid arguments");
    }
    if (result.size() != first.size()) {
        throw std::invalid_argument("Invalid arguments");
    }
    if (result[0].size() != second[0].size()) {
        throw std::invalid_argument("Invalid arguments");
    }

    for (int i = 0; i < first.size(); ++i)
        for (int j = 0; j < second[0].size(); ++j) {
            result[i][j] = 0;
            for (int k = 0; k < first[0].size(); ++k) {
                auto mult = produce(first[i][k], second[k][j]);
                result[i][j] = sum(result[i][j], mult);
            }
        }
}

void GraphHelpers::ShimbellPowMatrix(GraphHelpers::Matrix &in, const int &power) {

    auto cpy1 = in;
    for (int i = 0; i < power - 1; ++i) {
        auto cpy2 = in;
        GraphHelpers::matrixMultiplication(cpy2, cpy1, in, GraphHelpers::ShimbellPlusMin, GraphHelpers::ShimbellMult);
    }
}


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
    if (dijkstraResult.value()[sourceVertexName] == GraphHelpers::infitity) {
        return std::nullopt;
    }
    if (dijkstraResult.value()[destinationVertexName] == GraphHelpers::infitity) {
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
        if (*currentVertex == sourceVertexName) {
            break;
        }
        for (int i = 0; i < verticesCount; ++i) {
            if (matrix[i][j] == 0) {
                continue;
            }
            const auto &edgeWeight = matrix[i][j];
            const auto &currentVertexWeight = dijkstraResult.value()[*currentVertex];
            const auto &nbrVertexWeight = dijkstraResult.value()[vertices[i]];
            if (nbrVertexWeight + edgeWeight == currentVertexWeight) {
                currentVertex = &vertices[i];
                break;
            }
        }
    }


    return std::move(result);
}

std::optional<std::list<std::pair<std::string, std::string>>> GraphHelpers::KruskalMST(const Graph &graph) {
    if (graph.isOriented()) {
        return std::nullopt;
    }
    std::list<std::pair<std::string, std::string>> result;
    auto edges = graph.getUnorientedEdges();
    std::list<std::set<std::string>> components;
    for (const auto &v: graph.getVerticesVect()) {
        std::set<std::string> comp;
        comp.insert(v);
        components.push_back(std::move(comp));
    }
    edges.sort(
            [](const Graph::UnorientedEdge &first, const Graph::UnorientedEdge &second) -> bool {
                return first.w < second.w;
            });
    for (auto &edge: edges) {
        auto &first = edge.firstName;
        auto &second = edge.secondName;

        auto iterFirst = std::find_if(components.begin(), components.end(),
                                      [&first](const std::set<std::string> &comp) -> bool {
                                          return comp.contains(first);
                                      });
        auto iterSecond = std::find_if(components.begin(), components.end(),
                                       [&second](const std::set<std::string> &comp) -> bool {
                                           return comp.contains(second);
                                       });
        if (*iterFirst == *iterSecond) {
            continue;
        }
        (*iterFirst).merge(*iterSecond);
        components.erase(iterSecond);
        result.emplace_back(std::move(first), std::move(second));
    }

    return std::move(result);
}

std::optional<std::list<std::pair<std::string, std::string>>>
GraphHelpers::PrimaMST(const Graph &graph, const std::string &start_vertex) {
    if (graph.isOriented()) {
        return std::nullopt;
    }
    std::list<std::pair<std::string, std::string>> result;
    std::set<std::string> currentVertexSet;
    currentVertexSet.emplace(start_vertex);
    while (true) {
        int min = std::numeric_limits<int>::max();
        const std::string *minNbr = nullptr;
        const std::string *vertexFrom = nullptr;
        for (const auto &vertex: currentVertexSet) {
            const auto &nbrs = graph.getNeighbors(vertex);
            for (const auto &nbr: nbrs) {
                if (currentVertexSet.contains(nbr.first.getName())) {
                    continue;
                }
                if (min > nbr.second) {
                    min = nbr.second;
                    minNbr = &nbr.first.getName();
                    vertexFrom = &vertex;
                }
            }
        }
        if (minNbr == nullptr) {
            break;
        }
        currentVertexSet.emplace(*minNbr);
        result.emplace_back(*vertexFrom, *minNbr);
    }
    return std::move(result);
}

void
GraphHelpers::bronKerbosh(const std::set<std::string>& result,
                          std::set<std::string> candidates,
                          std::set<std::string> x,
                          std::list<std::set<std::string>> &results,
                          const Graph &graph) {
    if (candidates.size() == 0 && x.size() == 0) {
        results.push_back(result);
    }
    auto iter = candidates.begin();
    while (iter != candidates.end() && !candidates.empty()) {
        auto nbrs = graph.getNeighborsNameSet(*iter).value();
        std::set<std::string> new_candidates;
        std::set<std::string> new_result = result;
        std::set<std::string> new_x;
        new_result.insert(*iter);
        std::set_intersection(candidates.begin(), candidates.end(),nbrs.begin(),nbrs.end(),std::inserter(new_candidates,std::end(new_candidates)));
        std::set_intersection(x.begin(), x.end(),nbrs.begin(),nbrs.end(),std::inserter(new_x,std::end(new_x)));
        bronKerbosh(new_result,std::move(new_candidates),std::move(new_x),results,graph);
        if (iter != candidates.end()){
            candidates.erase(iter);
            x.insert(*iter);
            iter = std::begin(candidates);
        }
    }
}

std::list<std::set<std::string>> GraphHelpers::BronKerbrosh(const Graph &graph) {
    auto results = std::list<std::set<std::string>>();
    auto start_result = std::set<std::string>();
    auto start_x = std::set<std::string>();
    auto vertices = graph.getVerticesVect();
    auto start_candidates = std::set<std::string>(vertices.begin(),vertices.end());
    bronKerbosh(start_result,std::move(start_candidates),std::move(start_x),results,graph);
    return results;
}


