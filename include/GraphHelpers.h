//
// Created by ASUS on 29.12.2022.
//

#ifndef DISCRETEMATHLAB3_GRAPHHELPERS_H
#define DISCRETEMATHLAB3_GRAPHHELPERS_H


#include <set>
#include <Vertex.h>
#include <Graph.h>

class GraphHelpers {
public:
    static void _dfs(const Graph& graph, std::set<std::string>& visited, const std::string &startVertexName);
public:
    static std::map<std::string,std::map<std::string,bool>> ReachMatrix(const Graph& graph);
    static std::map<std::string,std::map<std::string,bool>> StrongConnectMatrix(const Graph& graph);
};


#endif //DISCRETEMATHLAB3_GRAPHHELPERS_H
