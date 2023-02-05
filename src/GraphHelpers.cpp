//
// Created by ASUS on 29.12.2022.
//

#include <iostream>
#include <GraphHelpers.h>

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
    for (auto& v1 : vertices){
        if (!uncoveredVertices.contains(v1))
            continue;
        auto component = std::set<std::string>();
        for (auto& v2 : vertices){
            if (boundMatrix[v1][v2]){
                uncoveredVertices.erase(v2);
                component.insert(v2);
            }
        }
        if (!component.empty()){
            result.insert(component);
        }
    }
    return result;
}

std::optional<std::map<int, std::set<std::string>>> GraphHelpers::TopologicalSort(const Graph &graph) {
    using ReturnType = std::optional<std::map<int,std::set<std::string>>>;
    if (graph.getVerticesCount() == 0){
        return std::nullopt;
    }
    if (HasCycles(graph)){
        return std::nullopt;
    }
    auto result = std::map<int, std::set<std::string>>();
    auto incidentMatrix = graph.getIncidentMatrix();
    auto& matrix = incidentMatrix.first;
    auto& map = incidentMatrix.second;
    auto c = 0;

    while (true){
        auto nullColumns = findNullColumns(matrix);
        if (nullColumns.empty()){
            break;
        }
        auto vertexSet = std::set<std::string>();
        for (auto&& col : nullColumns){
            vertexSet.emplace(map[col]);
        }
        result[c++] = std::move(vertexSet);
        removeNullColumns(matrix,nullColumns);

    }
    return ReturnType{std::move(result)};
}

bool GraphHelpers::HasCycles(const Graph &graph) {
    auto components = BoundComponents(graph);
    auto vCount = graph.getVerticesCount();
    return  components.size() != vCount;
}

std::list<std::size_t> GraphHelpers::findNullColumns(const std::vector<std::vector<int>> &incidentMatrix) {
    auto result = std::list<std::size_t>();
    for (int j = 0; j < incidentMatrix.size(); ++j){
        bool flag = true;
        for (int i = 0; i < incidentMatrix.size() && flag; ++i){
            flag &= incidentMatrix[i][j]==0;
        }
        if (flag){
            result.emplace_back(j);
        }
    }
    return result;
}

void GraphHelpers::removeNullColumns(std::vector<std::vector<int>> &incidentMatrix,
                                     const std::list<std::size_t> &nullColumns) {
    for (auto&& j : nullColumns){
        for (auto && i : incidentMatrix){
            i[j] = removed;
        }
    }
    for (auto&& i : nullColumns){
        for (std::size_t j = 0; j < incidentMatrix.size();++j){
            if (incidentMatrix[i][j] != removed){
                incidentMatrix[i][j] = null;
            }
        }
    }
}
