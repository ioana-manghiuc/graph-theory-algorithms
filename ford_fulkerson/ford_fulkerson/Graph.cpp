#include "Graph.h"
#include <queue>

Graph::Graph()
{
    m_isOriented = false;
}

void Graph::SetIsOriented(bool o)
{
    this->m_isOriented = o;
}

std::vector<Node>& Graph::GetNodes()
{
    return this->m_nodes;
}

std::vector<Arch>& Graph::GetArches()
{
    return this->m_arches;
}

void Graph::AddNode(QPoint p)
{
    Node n;
    n.SetCoordinate(p);
    n.SetValue(m_nodes.size());
    m_nodes.push_back(n);
}

void Graph::AddArch(Arch a)
{
    m_arches.push_back(a);
}

void Graph::AddArch(Node firstNode, Node secondNode)
{
    Arch a(firstNode, secondNode);
    m_arches.push_back(a);
}

bool Graph::IsMultigraph(Node n1, Node n2)
{
    for (Arch& arc : m_arches)
    {
        if ((arc.GetFirstNode().IsEqualTo(n1) && arc.GetSecondNode().IsEqualTo(n2)))
        {
            //checks if an arch already exists in that direction
            return true;
        }
    }
    return false;
}

bool Graph::NodeExists(QMouseEvent* e)
{
    for (Node n : m_nodes)
    {
        if (fabs(e->pos().x() - n.GetX()) < 30 && fabs(e->pos().y() - n.GetY()) < 30)
            return true;
    }
    return false;
}

Arch Graph::GetArchOfNodes(Node n1, Node n2, Graph G)
{
    for (Arch a : G.m_arches)
    {
        if ((a.GetFirstNode().GetValue() == n1.GetValue() && a.GetSecondNode().GetValue() == n2.GetValue()))
        {
            return a;
        }
    }
    return Arch();
}

std::list<Node> Graph::GetListOfNode(Node n)
{
    auto it = std::find_if(m_adjacencyList.begin(), m_adjacencyList.end(), [n](auto pair)
        {
            return pair.first.GetValue() == n.GetValue();
        });

    if (it != m_adjacencyList.end())
    {
        return it->second;
    }
    else
    {
        return std::list<Node>();
    }
}

void Graph::WriteInCapacityMatrix()
{
    std::vector<std::vector<float>> matrix(m_nodes.size(), std::vector<float>(m_nodes.size(), 0));
    std::vector<Arch> arce = this->GetArches();
    for (Arch& a : arce)
    {
        if (m_isOriented == false)
        {
            matrix[a.GetSecondNode().GetValue()][a.GetFirstNode().GetValue()] = 1;
        }
        matrix[a.GetFirstNode().GetValue()][a.GetSecondNode().GetValue()] = 1;
    }

   // std::vector<float> costs = { 10,7,3,5,7,7,9,14,4,12,5, 9, 13, 17 };
    std::vector<float> costs = { 10,9,5,4,6,7,2,3,5,7,8,8,3, 17 };
    //std::vector<float> costs = { 16,13,10,12,4,14,9,20,7,4,5, 9, 13, 17 };
    //std::vector<float> costs = { 35,20,30,18,14,18,16,13,12,1,6,31,33 };
    //std::vector<float> costs = { 24,20,19,17,14,17,18,18,13,1,6,31,33 };
    //std::vector<float> costs = { 17,24,19,18,14,19,16,15,17,1,6,31,33 };
    //std::vector<float> costs = { 18,15,19,5,3,2,8,13,12,1,6,31,33 };
    int k = 0;

    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix.size(); j++)
        {
            if (matrix[i][j] == 1)
            {
                matrix[i][j] = costs[k];
                k++;
            }
        }
    }

    m_matrixOfCapacities = matrix;

    for (Arch& a : m_arches)
    {
        a.SetCapacity(m_matrixOfCapacities[a.GetFirstNode().GetValue()][a.GetSecondNode().GetValue()]);
    }
}

void Graph::AddToAdjacencyList()
{
    for (Node n : m_nodes)
    {
        std::list<Node> adjacentnodes;
        for (Arch a : m_arches)
        {
            if (m_isOriented == false)
            {
                if (n.GetValue() == a.GetFirstNode().GetValue())
                {
                    adjacentnodes.push_back(a.GetSecondNode());
                }
                else if (n.GetValue() == a.GetSecondNode().GetValue())
                {
                    adjacentnodes.push_back(a.GetFirstNode());
                }
            }
            else if (m_isOriented == true && n.GetValue() == a.GetFirstNode().GetValue()
                && n.GetValue() != a.GetSecondNode().GetValue())
            {
                adjacentnodes.push_back(a.GetSecondNode());

            }
        }
        std::pair<Node, std::list<Node>> row(n, adjacentnodes);
        m_adjacencyList.push_back(row);
    }
}

std::vector<std::pair<Node, std::list<Node>>> Graph::GetAdjacencyList()
{
    return m_adjacencyList;
}

std::vector<std::vector<float>> Graph::GetMatrixOfCapacity()
{
    return  m_matrixOfCapacities;
}

std::vector<Arch> Graph::GetMinimumCostArches()
{
    return m_minimumCostArches;
}

Node Graph::GetNodeOfValue(int value)
{
    auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [value](const Node& node) {
        return node.GetValue() == value;
        });

    if (it != m_nodes.end())
    {
        return *it;
    }
    else
    {
        return Node();
    }
}

bool Graph::ArchExists(const Arch& arch, const std::vector<Arch>& arches)
{
    for (const Arch& existingArch : arches)
    {
        if (existingArch == arch)
        {
            return true;
        }
    }
    return false;
}

bool Graph::RoadExists(Graph& G, const Node& start, const Node& end, std::vector<int>& predecessors)
{
    std::vector<bool> visited(m_nodes.size(), false);
    predecessors.resize(m_nodes.size(), 0);

    std::queue<Node> V;
    V.push(start);
    visited[start.GetValue()] = true;
    predecessors[start.GetValue()] = -1;

    while (!V.empty())
    {
        Node n = V.front();
        V.pop();

        for (auto nextNode : GetListOfNode(n))
        {
            Arch a = GetArchOfNodes(n, nextNode, G);
            if (!visited[nextNode.GetValue()] && ArchExists(a, G.GetArches()))
            {
                if (nextNode == end)
                {
                    predecessors[nextNode.GetValue()] = n.GetValue();
                    return true;
                }

                V.push(nextNode);
                predecessors[nextNode.GetValue()] = n.GetValue();
                visited[nextNode.GetValue()] = true;
            }
        }
    }
    m_predecessors.swap(predecessors);
    return false;
}


void Graph::GeneratePaths(const Node& u, const Node& d, std::vector<bool>& visited,
    std::vector<Arch>& currentPath, std::vector<Arch>& path, Graph& G, float& residual)
{
    visited[u.GetValue()] = true;

    if (u == d)
    {
        residual = std::numeric_limits<float>::max();

        for (Arch& road : currentPath)
        {
            residual = std::min(residual, road.GetCapacity());
        }
        path = currentPath;
    }
    else
    {
        for (Node& neighbor : G.GetListOfNode(u))
        {
            Arch currentArch = GetArchOfNodes(u, neighbor, G);
            if (!visited[neighbor.GetValue()] && currentArch.GetCapacity() > 0)
            {
                if (!ArchExists(currentArch, currentPath))
                {
                    currentPath.push_back(currentArch);
                    GeneratePaths(neighbor, d, visited, currentPath, path, G, residual);
                    currentPath.pop_back();
                }
            }
        }
    }

    visited[u.GetValue()] = false;
}

void Graph::GeneratePath(const Node& start, const Node& finish, Graph& G, std::vector<Arch>& path, float& residual)
{
    std::vector<bool> visited(m_nodes.size(), false);
    std::vector<Arch> currentPath;
    GeneratePaths(start, finish, visited, currentPath, path, G, residual);
}

void Graph::RemoveArch(Graph& residualGraph, const Node& firstNode, const Node& secondNode)
{
    residualGraph.m_arches.erase(std::remove_if(residualGraph.m_arches.begin(), residualGraph.m_arches.end(),
        [&](const Arch& arch) {
            return (arch.GetFirstNode() == firstNode && arch.GetSecondNode() == secondNode);
        }), residualGraph.m_arches.end());
}

void Graph::AddOrUpdateArch(Graph& residualGraph, Arch& arch)
{
    auto it = std::find_if(residualGraph.m_arches.begin(), residualGraph.m_arches.end(),
        [&](const Arch& existingArch) {
            return (existingArch.GetFirstNode() == arch.GetFirstNode() && existingArch.GetSecondNode() == arch.GetSecondNode());
        });

    if (it != residualGraph.m_arches.end())
    {
        it->SetCapacity(arch.GetCapacity());
    }
    else
    {
        residualGraph.m_arches.push_back(arch);
    }
}

float Graph::FordFulkerson(const Node& start, const Node& finish)
{
    float flow = 0;
    Graph residualGraph = *this;
    std::vector<Arch> previousRoad;

    while (RoadExists(residualGraph, start, finish, m_predecessors))
    {
        std::vector<Arch> road;
        float residual;
        GeneratePath(start, finish, residualGraph, road, residual);

        if (road.empty() || road == previousRoad)
        {
            continue;
        }

        flow += residual;
        previousRoad = road;

        for (Arch& arch : road)
        {
            if (residual == arch.GetCapacity())
            {
                Arch reversedArch(arch.GetSecondNode(), arch.GetFirstNode(), residual);
                residualGraph.AddOrUpdateArch(residualGraph, reversedArch);
                residualGraph.RemoveArch(residualGraph, arch.GetFirstNode(), arch.GetSecondNode());
            }
            else
            {
                arch.SetCapacity(arch.GetCapacity() - residual);
                Arch reverseArch = residualGraph.GetArchOfNodes(arch.GetSecondNode(), arch.GetFirstNode(), residualGraph);

                if (reverseArch == Arch())
                {
                    reverseArch = Arch(arch.GetSecondNode(), arch.GetFirstNode(), residual);
                    residualGraph.AddOrUpdateArch(residualGraph, reverseArch);
                }
                else
                {
                    reverseArch.SetCapacity(reverseArch.GetCapacity() + residual);
                }

                if (arch.GetCapacity() == 0)
                {
                    residualGraph.RemoveArch(residualGraph, arch.GetFirstNode(), arch.GetSecondNode());
                }
            }
        }
    }
    return flow;
}