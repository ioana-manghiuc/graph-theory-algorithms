#include "Graph.h"

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

void Graph::WriteMatrixInFile(const QString& outputFile)
{
    std::vector<std::vector<int>> matrix(m_nodes.size(), std::vector<int>(m_nodes.size(), 0));

    for (Arch& a : GetArches())
    {
        if (m_isOriented == false)
        {
            matrix[a.GetSecondNode().GetValue()][a.GetFirstNode().GetValue()] = 1;
        }
        matrix[a.GetFirstNode().GetValue()][a.GetSecondNode().GetValue()] = 1;
    }

    QFile file(outputFile);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << matrix.size() << "\n";
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix.size(); j++)
            {
                out << matrix[i][j] << " ";
            }
            out << "\n";
        }
    }
    file.close();
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

bool Graph::HasCycles()
{
    return m_hasCycles;
}

bool Graph::IsTree()
{
    if (m_connectedComponents == 1 && m_hasCycles == false)
    {
        return true;
    }
    return false;
}

std::vector<std::pair<Node, std::list<Node>>> Graph::GetAdjacencyList()
{
    return m_adjacencyList;
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

void Graph::PrintAdjacencyList(const QString& output)
{
    QFile file(output);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << m_adjacencyList.size() << "\n";
        for (auto row : m_adjacencyList)
        {
            out << row.first.GetValue() + 1 << " -> ";
            for (auto node : row.second)
            {
                out << node.GetValue() + 1 << " ";
            }
            out << "\n";
        }
    }
    file.close();
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

Node Graph::GetStartNode()
{
    return m_nodes[0];
}


void Graph::DFS(Node s)
{
    std::ofstream out("pdf.txt");
    std::vector<bool> U(m_nodes.size(), false);
    std::vector<int> predecessors(m_nodes.size(), 0);
    std::vector<int> t1(m_nodes.size(), 0);
    std::vector<int> t2(m_nodes.size(), 0);
    std::vector<int> visited(m_nodes.size(), 0);
    int t = 1;
    t1[s.GetValue()] = t;
    int connectedComponents = 1;

    std::stack<Node> V;

    bool circuitfound = false;
    bool isTree = true;

    V.push(s);
    U[s.GetValue()] = true;
    visited[s.GetValue()] = 1;

    while (!V.empty())
    {
        Node n = V.top();
        std::list<Node> adjacentnodes = GetListOfNode(n);
        bool adjacentVisited = true;
        visited[n.GetValue()] = 1;
        for (const auto& nextNode : adjacentnodes)
        {
            if (!U[nextNode.GetValue()])
            {
                U[nextNode.GetValue()] = true;
                V.push(nextNode);
                predecessors[nextNode.GetValue()] = n.GetValue() + 1;
                t++;
                t1[nextNode.GetValue()] = t;
                adjacentVisited = false;
                // => unvisited adjacent node was found, we check its adjacency list instead
                break;
            }
            if (U[nextNode.GetValue()] && visited[nextNode.GetValue()] == 1)
            {
                out << "Graph has circuits - is not a tree.\n";
                circuitfound = true;
            }
        }
        if (circuitfound)
        {
            return;
        }
        if (adjacentVisited)
        {
            // remains true, which means we returned back to n
            // and remove nextNode
            V.pop();
            t++;
            t2[n.GetValue()] = t;
            visited[n.GetValue()] = 2;

        }
    }

    if (std::find(U.begin(), U.end(), false) != U.end())
    {       
        int index = 0;
        for (int i = 0; i < U.size(); i++)
        {
            if (U[i] == false)
            {
                index = i;
                break;
            }
        }
        if (!m_adjacencyList[index].second.empty())
        {
            if (m_adjacencyList[index].second.front().GetValue() == 0)
            {
                connectedComponents++;
            }
        }
        else if (std::count(predecessors.begin(), predecessors.end(), 0) > 1)
        {
            connectedComponents++;
        }
        else
        {
            connectedComponents++;
        }
    }

    out << "connected components: " << connectedComponents;
    out << "\nt1: ";
    for (auto i : t1)
    {
        out << i << " ";
    }
    out << "\nt2: ";
    for (auto j : t2)
    {
        out << j << " ";
    }
    out << "\npredecessors: ";
    for (auto p : predecessors)
    {
        out << p << " ";
    }
    out << "\nvisited: ";
    for (const auto& p : visited)
    {
        out << p << " ";
    }
    m_visited = visited;
    m_connectedComponents = connectedComponents;
    m_hasCycles = circuitfound;
}

void Graph::TDFS(Node s)
{
    std::ofstream out("pdfcircuit.txt");
    std::vector<bool> U(m_nodes.size(), false);
    std::vector<int> predecessors(m_nodes.size(), 0);
    std::vector<int> t1(m_nodes.size(), 0);
    std::vector<int> t2(m_nodes.size(), 0);
    std::vector<int> visited(m_nodes.size(), 0);
    int t = 1; 
    t1[s.GetValue()] = t;

    std::stack<Node> V;
    std::vector<Node> W;

    bool circuitfound = false;

    V.push(s);
    U[s.GetValue()] = true;
    visited[s.GetValue()] = 1;

    while (W.size() != m_nodes.size() && !circuitfound)
    {
        if (!V.empty())
        {
            Node n = V.top();
            std::list<Node> adjacentnodes = GetListOfNode(n);
            bool adjacentVisited = true;
            visited[n.GetValue()] = 1;
            for (const auto& nextNode : adjacentnodes)
            {
                if (!U[nextNode.GetValue()])
                {
                    U[nextNode.GetValue()] = true;
                    V.push(nextNode);
                    predecessors[nextNode.GetValue()] = n.GetValue() + 1;
                    t++;
                    t1[nextNode.GetValue()] = t;
                    adjacentVisited = false;
                    break;
                }
                if (U[nextNode.GetValue()] && visited[nextNode.GetValue()] == 1)
                {
                    out << "Graph has circuits - cannot be topologically sorted.\n";
                    circuitfound = true;
                    break;
                }
            }
            if (circuitfound)
            {
                break;
            }

            if (adjacentVisited) // remains true, which means we returned back to n and remove nextNode
            {
                V.pop();
                W.push_back(n);
                t++;
                t2[n.GetValue()] = t;
                visited[n.GetValue()] = 2;
            }
        }
        if (V.empty() && std::find(U.begin(), U.end(), false) != U.end())
        {
            int index = 0;
            for (int i = 0; i < U.size(); i++)
            {
                if (U[i] == false)
                {
                    index = i;
                    break;
                }
            }
            V.push(m_adjacencyList[index].first);
            U[m_adjacencyList[index].first.GetValue()] = true;
        }
    }

    if (!circuitfound)
    {
        out << "Graph can be topologically sorted.\n";
    }

    m_t1 = t1;
    m_t2 = t2;
    m_predecessors = predecessors;
    m_hasCycles = circuitfound;
}

void Graph::TopologicalSort(Node s)
{
    std::ofstream out("topologicalsort.txt");
    TDFS(s);
    if (m_hasCycles == true)
    {
        out << "Graph has circuits - cannot be topologically sorted.\n";

    }
    else
    {
        if (!m_t2.empty())
        {
            std::vector<std::pair<Node, int>> nodesAndT2;
            for (size_t i = 0; i < m_nodes.size(); i++)
            {
                nodesAndT2.emplace_back(m_nodes[i], m_t2[i]);
            }

            std::sort(nodesAndT2.begin(), nodesAndT2.end(),
                [](const auto& a, const auto& b) {
                    return a.second > b.second;
                });

            out << "Topological Sort: ";
            for (const auto& pair : nodesAndT2)
            {
                out << pair.first.GetValue() + 1 << " ";
            }
            out << "\n";
            out << "\nt1: ";
            for (const auto& i : m_t1)
            {
                out << i << " ";
            }
            out << "\nt2: ";
            for (const auto& j : m_t2)
            {
                out << j << " ";
            }
            out << "\npredecessors: ";
            for (const auto& p : m_predecessors)
            {
                out << p << " ";
            }
        }
        else
        {
            std::ofstream out("topologicalsort.txt");
            out << "Graph is empty.\n";
        }
    }
}

bool Graph::IsOriented()
{
    return m_isOriented;
}

std::vector<Arch> Graph::GetSortedArches()
{
    for (int i = 0; i < m_nodes.size(); i++)
    {
        if (m_predecessors[i] != 0)
        {
            Arch a(m_nodes[m_predecessors[i] - 1], m_nodes[i]);
            m_sortedGraphArches.push_back(a);
        }
    }
    return m_sortedGraphArches;
}

int Graph::GetConnectedComponents()
{
    return m_connectedComponents;
}

void Graph::ShowSortedArches()
{
    std::ofstream out("sortedarches.txt");
    for (auto a : m_sortedGraphArches)
    {
        out << "(" << a.GetFirstNode().GetValue() + 1 << " ," << a.GetSecondNode().GetValue() + 1 << ")\n";
    }
}

void Graph::ClearGraph()
{
    m_nodes.clear();
    m_arches.clear();
    m_predecessors.clear();
    m_adjacencyList.clear();
    m_t1.clear();
    m_t2.clear();
    m_sortedGraphArches.clear();
}

void Graph::FindRootOfGraph()
{
    m_root.SetValue(-13);
    for (Node n : m_nodes)
    {
        DFS(n);
        if (std::all_of(m_visited.begin(), m_visited.end(), [](int val) { return val == 2; }))
        {
            m_root = n;
            break;
        }
    }
    std::ofstream out("root.txt");
    if (m_root.GetValue() != -13)
    {
        out << "Graph Root: " << m_root.GetValue() + 1;
        DFS(m_root);
    }
    else
    {
        out << "Graph doesn't have a root.\n";
    }
}

Node Graph::GetRoot()
{
    return m_root;
}

Node Graph::GetNextUnvisitedNode()
{
    for (const Node& node : m_nodes)
    {
        if (!m_visited[node.GetValue()])
        {
            return node;
        }
    }
    return Node();
}




