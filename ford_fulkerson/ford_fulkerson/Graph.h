#pragma once
#include "Node.h"
#include "Arch.h"
#include <vector>
#include <QFile>
#include <QTextStream>
#include <QMouseEvent>
#include <QDebug>
#include <stack>
#include <fstream>
#include <list>
#include <set>
#include <random>

class Graph
{
public:
    Graph();
    void SetIsOriented(bool o);

    void AddNode(QPoint p);
    void AddArch(Arch a);
    void AddArch(Node firstNode, Node secondNode);
    void WriteInCapacityMatrix();

    std::vector<Node>& GetNodes();
    std::vector<Arch>& GetArches();
    std::vector<std::pair<Node, std::list<Node>>> GetAdjacencyList();
    std::list<Node> GetListOfNode(Node n);
    std::vector<std::vector<float>> GetMatrixOfCapacity();
    Arch GetArchOfNodes(Node n1, Node n2, Graph G);
    std::vector<Arch> GetMinimumCostArches();
    Node GetNodeOfValue(int value);

    bool IsMultigraph(Node n1, Node n2);
    bool NodeExists(QMouseEvent* e);

    void AddToAdjacencyList();

    bool ArchExists(const Arch& arch, const std::vector<Arch>& arches);
    bool RoadExists(Graph& G, const Node& start, const Node& end, std::vector<int>& predecessors);
    void GeneratePaths(const Node& u, const Node& d, std::vector<bool>& visited,
        std::vector<Arch>& currentPath, std::vector<Arch>& path, Graph& G, float& residual);
    void GeneratePath(const Node& start, const Node& finish, Graph& G, std::vector<Arch>& path, float& residual);
    void RemoveArch(Graph& residualGraph, const Node& firstNode, const Node& secondNode);
    void AddOrUpdateArch(Graph& residualGraph, Arch& arch);
    float FordFulkerson(const Node& start, const Node& finish);


private:
    std::vector<Node> m_nodes;
    std::vector<Arch> m_arches;
    std::vector<std::vector<float>> m_matrixOfCapacities;
    std::vector<std::pair<Node, std::list<Node>>> m_adjacencyList;
    std::vector<int> m_predecessors;
    std::vector<Arch> m_minimumCostArches;
    bool m_isOriented;
    bool m_hasCycles;
    int m_connectedComponents;
    std::vector<int> m_visited;
};


