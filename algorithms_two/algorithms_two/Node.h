#pragma once
#include <QPainter>

class Node
{
public:
    Node();
    Node(int index);
    Node(QPoint q);

    void SetValue(int v);
    void SetCoordinate(QPoint c);
    void SetX(int x);
    void SetY(int y);
    void SetCoordinate(int x, int y);

    int GetValue() const;
    QPointF GetCoordinate();
    int GetX();
    int GetY();

    bool IsEqualTo(const Node& n);

    bool operator==(const Node& other) const;
    bool operator<(const Node& other) const;    

private:
    int m_index;
    int m_value;
    QPoint m_coordinate;
};
