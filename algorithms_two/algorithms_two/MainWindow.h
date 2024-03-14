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
#include <QMap>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    QPointF CalculateIntersectionPoint(QPointF center, QPointF point, qreal radius);
    void DrawMinimumCostArches(QColor archColor);

protected:
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void paintEvent(QPaintEvent* e);

private slots:
    void onPrimAlgorithmClicked();
    void onKruskalAlgorithmClicked();

private:
    Ui::MainWindowClass ui;
    Graph m_graph;
    Node m_firstNode;

    QMainWindow* m_primAlgorithmOutput;
    QPushButton* m_primAlgorithm;

    QMainWindow* m_KruskalWindow;
    QPushButton* m_KruskalAlgorithm;

    QPixmap canvas;
};

