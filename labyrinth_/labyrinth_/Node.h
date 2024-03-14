#pragma once
#ifndef NODE_H
#define NODE_H
#include <QPainter>

class Node
{
public:
    Node();
    Node(int value);
    Node(QPoint c);
    void SetValue(int v);
    void SetCoordinate(QPoint c);
    void SetX(int x);
    void SetY(int y);
    void SetCoordinate(int x, int y);
    int GetValue();
    QPoint GetCoordinate();
    int GetX();
    int GetY();
    int GetColour();
    void SetColour(int c);
    bool IsEqualTo(Node& n);

private:
    int m_value;
    QPoint m_coordinate;
    int m_colour;
};

#endif // NODE_H