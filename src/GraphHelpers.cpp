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
