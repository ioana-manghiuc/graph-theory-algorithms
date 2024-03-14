#pragma once
#ifndef ARCH_H
#define ARCH_H
#include "Node.h"

class Arch
{
public:
    Arch();
    Arch(Node firstNode, Node secondNode);
    Node& GetFirstNode();
    Node& GetSecondNode();

private:
    Node m_firstNode, m_secondNode;
};

#endif // ARCH_H