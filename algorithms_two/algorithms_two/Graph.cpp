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

bool Graph::NodeExists(QMouseEvent* e)
{
    for (Node n : m_nodes)
    {
        if (fabs(e->pos().x() - n.GetX()) < 30 && fabs(e->pos().y() - n.GetY()) < 30)
            return true;
    }
    return false;
}

Arch Graph::GetArchOfNodes(Node n1, Node n2)
{
    for (Arch a : m_arches)
    {
        if ((a.GetFirstNode().GetValue() == n1.GetValue() && a.GetSecondNode().GetValue() == n2.GetValue())
            || (a.GetSecondNode().GetValue() == n1.GetValue() && a.GetFirstNode().GetValue() == n2.GetValue()))
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

void Graph::WriteInCostMatrix()
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

    std::vector<float> costs = { 35,20,30,18,14,18,16,13,12,1,6,31,33 };
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
                matrix[j][i] = costs[k];
                k++;
            }
        }
    }

    m_matrixOfCosts = matrix;

    for (Arch& a : m_arches)
    {
        a.SetCost(m_matrixOfCosts[a.GetFirstNode().GetValue()][a.GetSecondNode().GetValue()]);
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

std::vector<std::vector<float>> Graph::GetMatrixOfCosts()
{
    return m_matrixOfCosts;
}

std::vector<Arch> Graph::GetMinimumCostArches()
{
    return m_minimumCostArches;
}

void Graph::PrimAlgorithm()
{
	std::vector<float> costs(m_nodes.size(), std::numeric_limits<float>::max());
	costs[0] = 0;
	std::set<Node> N;
	std::set<Node> N_hat(m_nodes.begin(), m_nodes.end());
	std::vector<Arch> e(m_nodes.size());

	Node x = m_nodes[0];

	while (!N_hat.empty())
	{
		std::list<Node> adjacentNodes = GetListOfNode(x);

		N.insert(x);
		N_hat.erase(x);

		float minimum = std::numeric_limits<float>::max();
		Node nextNodeCandidate;
        bool validCandidateFound = false;

		for (const auto& nextNode : adjacentNodes)
		{
			if (N_hat.find(nextNode) != N_hat.end())
			{
				Arch a = GetArchOfNodes(x, nextNode);
				if (costs[nextNode.GetValue()] > a.GetCost())
				{
					costs[nextNode.GetValue()] = a.GetCost();
					e[nextNode.GetValue()] = a;
					if (minimum > a.GetCost())
					{
						minimum = a.GetCost();
						nextNodeCandidate = nextNode;
                        validCandidateFound = true;
					}
				}
			}
		}
        if (!validCandidateFound)
        {
            if (!N_hat.empty())
            {
                x = *N_hat.begin(); 
            }
            else
            {
                break;
            }
        }
        else
        {         
            x = nextNodeCandidate;
        }
	}
    m_minimumCostArches = e;
}

int Graph::FindUltimateParent(std::vector<int>& parent, int node)
{
    if (parent[node] != node)
    {
        parent[node] = FindUltimateParent(parent, parent[node]);
    }
    return parent[node];
}

void Graph::Union(std::vector<int>& parent, int u, int v)
{
    parent[u] = v;
}

void Graph::KruskalAlgorithm()
{
    std::vector<Arch> MST;
    std::vector<int> parent(m_nodes.size());

    std::sort(m_arches.begin(), m_arches.end(),
        [](Arch a, Arch b)
        {
            return a.GetCost() < b.GetCost();
        });

    for (int i = 0; i < m_nodes.size(); i++)
    {
        parent[i] = i;
    }

    for (Arch a : m_arches)
    {
        int p1 = FindUltimateParent(parent, a.GetFirstNode().GetValue());
        int p2 = FindUltimateParent(parent, a.GetSecondNode().GetValue());
    
        if (p1 != p2)
        {
            MST.push_back(a);
            Union(parent, p1, p2);
        }
    }
    m_minimumCostArches = MST;
}
