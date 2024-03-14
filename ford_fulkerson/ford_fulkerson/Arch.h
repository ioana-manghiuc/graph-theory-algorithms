#pragma once
#include "Node.h"
#include "limits.h"

class Arch
{
public:
	Arch();
	Arch(Node firstNode, Node secondNode);
	Arch(Node firstNode, Node secondNode, float capacity);

	Node GetFirstNode() const;
	Node GetSecondNode() const;
	float GetCapacity() const;

	void SetCapacity(float c);

	bool operator==(const Arch& other) const {
		return (
			(m_firstNode == other.GetFirstNode()) &&
			(m_secondNode == other.GetSecondNode()) &&
			(m_capacity == other.GetCapacity())
			);
	}

private:
	Node m_firstNode;
	Node m_secondNode;
	float m_capacity;
};

