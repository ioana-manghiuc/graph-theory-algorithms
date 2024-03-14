#include "Node.h"

Node::Node()
{
	m_value = -1;
}

Node::Node(int index) : m_index{ index }
{
}

Node::Node(QPoint q): m_coordinate{q}
{
}

void Node::SetValue(int v)
{
	this->m_value = v;
}

void Node::SetCoordinate(QPoint c)
{
	this->m_coordinate = c;
}

void Node::SetX(int x)
{
	this->m_coordinate.setX(x);
}

void Node::SetY(int y)
{
	this->m_coordinate.setY(y);
}

void Node::SetCoordinate(int x, int y)
{
	this->m_coordinate.setX(x);
	this->m_coordinate.setY(y);
}

int Node::GetValue() const
{
	return m_value;
}

QPoint Node::GetCoordinate()
{
	return this->m_coordinate;
}

int Node::GetX()
{
	return this->m_coordinate.x();
}

int Node::GetY()
{
	return this->m_coordinate.y();
}

bool Node::IsEqualTo(const Node& n)
{
	return this->m_value == n.GetValue();
}
