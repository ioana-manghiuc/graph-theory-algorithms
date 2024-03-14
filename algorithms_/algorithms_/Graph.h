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

class Graph
{
public:
    Graph();
    void SetIsOriented(bool o);

    void AddNode(QPoint p);
    void AddArch(Arch a);
    void AddArch(Node firstNode, Node secondNode);

    std::vector<Node>& GetNodes();
    std::vector<Arch>& GetArches();
    Node GetStartNode();
    std::vector<std::pair<Node, std::list<Node>>> GetAdjacencyList();
    std::list<Node> GetListOfNode(Node n);
    Node GetRoot();
    Node GetNextUnvisitedNode();
    std::vector<Arch> GetSortedArches();
    int GetConnectedComponents();

    bool IsMultigraph(Node n1, Node n2);
    void WriteMatrixInFile(const QString& outputFile);
    bool NodeExists(QMouseEvent* e);
    bool HasCycles();
    bool IsTree();

    void AddToAdjacencyList();
    void PrintAdjacencyList(const QString& output);

    void DFS(Node s);
    void TDFS(Node s);
    void TopologicalSort(Node s);
    bool IsOriented();
    void ShowSortedArches();
    void ClearGraph();
    void FindRootOfGraph();


private:
    std::vector<Node> m_nodes;
    std::vector<Arch> m_arches;
    bool m_isOriented;
    std::vector<int> m_predecessors;
    std::vector<std::pair<Node, std::list<Node>>> m_adjacencyList;
    std::vector<int> m_t1;
    std::vector<int> m_t2;
    std::vector<Arch> m_sortedGraphArches;
    std::vector<int> m_visited;
    Node m_root;
    bool m_hasCycles;
    int m_connectedComponents;
};

