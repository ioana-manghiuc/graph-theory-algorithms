#pragma once
#include "Node.h"
#include "limits.h"

class Arch
{
public:
	Arch();
	Arch(Node firstNode, Node secondNode);

	Node& GetFirstNode();
	Node& GetSecondNode();
	float GetCost();

	void SetCost(float c);

private:
	Node m_firstNode;
	Node m_secondNode;
	float m_cost;
};