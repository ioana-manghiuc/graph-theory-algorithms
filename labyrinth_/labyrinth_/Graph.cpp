#include "Graph.h"

Graph::Graph()
{

}

std::vector<Node>& Graph::GetNodes()
{
    return m_nodes;
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


int Graph::GetSize()
{
    return this->m_matrixSize;
}

std::vector<std::vector<int>>& Graph::GetMatrix()
{
    return this->m_matrix;
}

void Graph::ReadMatrixFromFile()
{
    QFile fis("input_matrix.txt");
    if (fis.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream input(&fis);
        input >> m_matrixSize;

        m_matrix.resize(m_matrixSize + 2, std::vector<int>(m_matrixSize + 2, 0));

        for (int i = 1; i <= m_matrixSize; i++)
        {
            for (int j = 1; j <= m_matrixSize; j++)
            {
                input >> m_matrix[i][j];
            }
        }

        for (int i = 0; i <= m_matrixSize + 1; i++)
        {
            m_matrix[i][0] = 0;
            m_matrix[0][i] = 0;
            m_matrix[m_matrixSize + 1][i] = 0;
            m_matrix[i][m_matrixSize + 1] = 0;
        }
    }
    else
    {
        std::cout << "ERROR - file couldn't be opened.\n";
    }
}

void Graph::CreateGraphFromMatrix()
{
    for (int i = 1; i <= m_matrixSize; i++)
    {
        for (int j = 1; j <= m_matrixSize; j++)
        {
            if (m_matrix[i][j] == 0)
            {
                continue;
            }

            Node n;
            n.SetX((double)j);
            n.SetY((double)i);
            n.SetColour(m_matrix[i][j]);
            n.SetValue(m_nodes.size() + 1);
            m_nodes.push_back(n);
            if (m_matrix[i - 1][j] != 0) // arches on columns
            {
                Node currentNode;
                for (Node& node : m_nodes)
                {
                    if (node.GetCoordinate().y() == (double)i - 1 && 
                        node.GetCoordinate().x() == (double)j)
                    {
                        currentNode = node;
                        break;
                    }
                }
                Arch arch(n, currentNode);
                m_arches.push_back(arch);
            }

            if (m_matrix[i][j - 1] != 0) // arches on rows
            {
                Node currentNode;
                for (Node& node : m_nodes)
                {
                    if (node.GetCoordinate().y() == (double)i && 
                        node.GetCoordinate().x() == (double)j - 1)
                    {
                        currentNode = node;
                        break;
                    }
                }
                Arch arch(n, currentNode);
                m_arches.push_back(arch);
            }
        }
    }
}

Node Graph::FindStartingNode()
{
    Node start;
    for (Node& node : m_nodes)
    {
        if (node.GetColour() == 3)
        {
            start = node;
        }
    }
    return start;
}

void Graph::BFS(Node s)
{
    std::vector<int> length(m_nodes.size(), 0);
    std::vector<bool> U(m_nodes.size(), false);
    std::vector<int> predecessors(m_nodes.size(), 0);

    std::queue<Node> V;
    V.push(s);
    U[s.GetValue() - 1] = true;
    while (!V.empty())
    {
        Node n = V.front();
        V.pop();

        for (Arch& a : m_arches)
        {
            if (a.GetFirstNode().GetValue() == n.GetValue()
                && !U[a.GetSecondNode().GetValue() - 1])
            {
                U[a.GetSecondNode().GetValue() - 1] = true;
                V.push(a.GetSecondNode());
                predecessors[a.GetSecondNode().GetValue() - 1] = n.GetValue();
                length[a.GetSecondNode().GetValue() - 1] = length[n.GetValue() - 1] + 1;
            }

            if (a.GetSecondNode().GetValue() == n.GetValue()
                && !U[a.GetFirstNode().GetValue() - 1])
            {
                U[a.GetFirstNode().GetValue() - 1] = true;
                V.push(a.GetFirstNode());
                predecessors[a.GetFirstNode().GetValue() - 1] = n.GetValue();
                length[a.GetFirstNode().GetValue() - 1] = length[n.GetValue() - 1] + 1;
            }
        }
    }
    m_predecessors = predecessors;
}

void Graph::FindExitNodes()
{
    for (Node& node : m_nodes)
    {
        if (node.GetColour() == 2)
        {
            m_exitNodes.push_back(node);
        }
    }
}

void Graph::Road(Node n)
{
    while (m_predecessors[n.GetValue() - 1] != 0)
    {
        int i = (int)n.GetCoordinate().y();
        int j = (int)n.GetCoordinate().x();
        if (m_matrix[i][j] == 1)
        {
            m_matrix[i][j] = 7;
        }
        Arch a(n, m_nodes[m_predecessors[n.GetValue() - 1] - 1]);
        m_exitRoads.push_back(a);

        Node current = m_nodes[m_predecessors[n.GetValue() - 1] - 1];
        n = current;
    }
}

void Graph::AllExitPaths()
{
    for (Node& node : m_exitNodes)
    {
        Road(node);
    }
}

std::vector<Arch>& Graph::GetExitRoads()
{
    return m_exitRoads;
}
