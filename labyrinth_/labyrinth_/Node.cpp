#include "Node.h"

Node::Node()
{
}
Node::Node(int value) : m_value(value)
{}

Node::Node(QPoint c) : m_coordinate(c)
{ }

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

int Node::GetX()
{
    return this->m_coordinate.x();
}

int Node::GetY()
{
    return this->m_coordinate.y();
}

int Node::GetValue()
{
    return this->m_value;
}

int Node::GetColour()
{
    return this->m_colour;
}

void Node::SetColour(int c)
{
    this->m_colour = c;
}

QPoint Node::GetCoordinate()
{
    return this->m_coordinate;
}

bool Node::IsEqualTo(Node& n)
{
    return this->m_value == n.GetValue();
}