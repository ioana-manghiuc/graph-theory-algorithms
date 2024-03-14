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
    void WriteInCostMatrix();

    std::vector<Node>& GetNodes();
    std::vector<Arch>& GetArches();
    std::vector<std::pair<Node, std::list<Node>>> GetAdjacencyList();
    std::list<Node> GetListOfNode(Node n);
    std::vector<std::vector<float>> GetMatrixOfCosts();
    Arch GetArchOfNodes(Node n1, Node n2);
    std::vector<Arch> GetMinimumCostArches();

    bool IsMultigraph(Node n1, Node n2);
    bool NodeExists(QMouseEvent* e);

    void AddToAdjacencyList();

    void PrimAlgorithm();
    int FindUltimateParent(std::vector<int>& parent, int node);
    void Union(std::vector<int>& parent, int u, int v);
    void KruskalAlgorithm();
    
private:
    std::vector<Node> m_nodes;
    std::vector<Arch> m_arches;
    std::vector<std::vector<float>> m_matrixOfCosts;
    std::vector<std::pair<Node, std::list<Node>>> m_adjacencyList;
    std::vector<Arch> m_minimumCostArches;
    bool m_isOriented;
};

