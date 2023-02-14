//
// Created by ASUS on 29.12.2022.
//

#ifndef DISCRETEMATHLAB3_GRAPHHELPERS_H
#define DISCRETEMATHLAB3_GRAPHHELPERS_H


#include <set>
#include <Vertex.h>
#include <Graph.h>
#include <optional>
#include <limits>

class GraphHelpers {
public:
    using Matrix = std::vector<std::vector<int>>;
    using Bfunc = std::function<int(int, int)>;
private:

    static constexpr int null = 0;
    static constexpr int removed = 1 << 30;

    static void _dfs(const Graph &graph, std::set<std::string> &visited, const std::string &startVertexName);

    static std::list<std::size_t> findNullColumns(const std::vector<std::vector<int>> &incidentMatrix);

    static void
    removeNullColumns(std::vector<std::vector<int>> &incidentMatrix, const std::list<std::size_t> &nullColumns);

public:
    static const Bfunc ShimbellMult;
    static const Bfunc ShimbellPlusMin;
    static const Bfunc ShimbellPlusMax;

    static Matrix
    matrixMultiplication(const GraphHelpers::Matrix &first, const GraphHelpers::Matrix &second,
                         const Bfunc &sum = std::plus<int>(),
                         const Bfunc &produce = std::multiplies<int>()) noexcept(false);

    static void
    matrixMultiplication(const GraphHelpers::Matrix &first, const GraphHelpers::Matrix &second,
                         Matrix &result,
                         const Bfunc &sum = std::plus<int>(),
                         const Bfunc &produce = std::multiplies<int>()) noexcept(false);

    static void ShimbellPowMatrix(GraphHelpers::Matrix& in,const int& power);

    static constexpr int infitity = std::numeric_limits<int>::max();

    static std::map<std::string, std::map<std::string, bool>> ReachMatrix(const Graph &graph);

    static std::map<std::string, std::map<std::string, bool>> StrongConnectMatrix(const Graph &graph);

    static std::set<std::set<std::string>> BoundComponents(const Graph &graph);

    static std::optional<std::map<std::string, int>> Dijkstra(const Graph &graph, const std::string &startVertexName);

    static std::optional<std::list<std::string>>
    DijkstraPath(const Graph &graph, const std::string &sourceVertexName, const std::string &destinationVertexName);

    static std::optional<std::list<std::pair<std::string, std::string>>> KruskalMST(const Graph &graph);

    static std::optional<std::list<std::pair<std::string, std::string>>>
    PrimaMST(const Graph &graph, const std::string &start_vertex);

    static bool HasCycles(const Graph &graph);

    static std::optional<std::map<int, std::set<std::string>>> TopologicalSort(const Graph &graph);


    bool validateSubMatrix(const Matrix &m, int lenSub, int i_start, int j_start);

    std::tuple<int, int, int> findMaxSubMatrix(const Matrix &m);
};

#endif //DISCRETEMATHLAB3_GRAPHHELPERS_H
