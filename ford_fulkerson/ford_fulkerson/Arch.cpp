#include "Arch.h"

Arch::Arch()
{
	m_capacity = 0;
}

Arch::Arch(Node firstNode, Node secondNode) :
	m_firstNode(firstNode),
	m_secondNode(secondNode)
{
	m_capacity = 0;
}

Arch::Arch(Node firstNode, Node secondNode, float capacity)
	: m_firstNode{ firstNode }, m_secondNode(secondNode), m_capacity{ capacity }
{ }


Node Arch::GetFirstNode() const
{
	return this->m_firstNode;
}

Node Arch::GetSecondNode() const
{
	return this->m_secondNode;
}

float Arch::GetCapacity() const
{
	return m_capacity;
}

void Arch::SetCapacity(float c)
{
	m_capacity = c;
}


