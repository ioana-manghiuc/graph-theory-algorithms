#pragma once
#include "Node.h"

class Arch
{
public:
	Arch();
	Arch(Node firstNode, Node secondNode);
	Node& GetFirstNode();
	Node& GetSecondNode();

private:
	Node m_firstNode;
	Node m_secondNode;
};

