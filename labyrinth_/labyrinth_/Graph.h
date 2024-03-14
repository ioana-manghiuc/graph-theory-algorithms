#pragma once
#ifndef GRAPH_H
#define GRAPH_H
#include "Node.h"
#include "Arch.h"
#include <vector>
#include <QVector>
#include <queue>
#include <QFile>
#include <QTextStream>
#include <QMouseEvent>
#include <iostream>
#include <fstream>

class Graph
{
public:
    Graph();
    std::vector<Node>& GetNodes();
    std::vector<Arch>& GetArches();
    void AddNode(QPoint p);
    void AddArch(Arch a);
    void AddArch(Node firstNode, Node secondNode);
    bool IsMultigraph(Node n1, Node n2);
    bool NodeExists(QMouseEvent* e);
    int GetSize();
    std::vector<std::vector<int>>& GetMatrix();
    void ReadMatrixFromFile();
    void CreateGraphFromMatrix();
    void BFS(Node s);
    Node FindStartingNode();
    void FindExitNodes();
    void Road(Node n);
    void AllExitPaths();
    std::vector<Arch>& GetExitRoads();

private:
    
    std::vector<Node> m_nodes;
    std::vector<Arch> m_arches;
    std::vector<std::vector<int>> m_matrix;
    int m_matrixSize;
    std::vector<int> m_predecessors;
    std::vector<Node> m_exitNodes;
    std::vector<Arch> m_exitRoads;
};
#endif // GRAPH_H