#include "Arch.h"

Arch::Arch()
{
}

Arch::Arch(Node firstNode, Node secondNode): 
	m_firstNode(firstNode),
	m_secondNode(secondNode)
{ }

Node& Arch::GetFirstNode()
{
	return this->m_firstNode;
}

Node& Arch::GetSecondNode()
{
	return this->m_secondNode;
}
