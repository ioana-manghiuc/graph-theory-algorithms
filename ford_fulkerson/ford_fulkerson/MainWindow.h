#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include "Graph.h"
#include <QLabel>
#include <iostream>
#include "math.h"
#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QFile>
#include <QTextStream>
#include <QPushButton>
#include <QPalette>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QMap>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    QPointF CalculateIntersectionPoint(QPointF center, QPointF point, qreal radius);
    void PaintOrientedArch(QPainter& paint, Node first, Node second, float capacity, const QColor& color);

protected:
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void paintEvent(QPaintEvent* e);

private slots:
    void onIsOrientedClicked();
    void onFordFulkersonClicked();

private:
    Ui::MainWindowClass ui;
    Graph m_graph;
    Node m_firstNode;

    QMainWindow* m_fordFulkersonOutput;
    QPushButton* m_fordFulkerson;
    QCheckBox* m_isOriented;
    QPixmap canvas;
};
