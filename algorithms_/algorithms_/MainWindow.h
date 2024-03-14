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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void drawSortedGraph();
    QPointF calculateIntersectionPoint(QPointF center, QPointF point, qreal radius);
    void showMessage(const QString& message);

protected:
    virtual void mouseReleaseEvent(QMouseEvent* e);
    void paintOrientedArch(QPainter& paint, QPointF start, QPointF end);
    virtual void paintEvent(QPaintEvent* e);

private slots:
    void on_orientedGraph_clicked();
    void onPrintSortedGraphClicked();
    void onClearButtonClicked();
    void onFindButtonClicked();
    

private:
    Ui::MainWindowClass ui;
    Graph m_graph;
    Node m_firstNode;

    QMainWindow* sortedGraph;
    QPixmap sortedGraphCanvas;
    QMainWindow* errors;

    QMainWindow* root;

    QPushButton* m_sortedGraph;
    QPushButton* m_clearWindow;
    QPushButton* m_findRoot;

    QTextEdit* m_text;
    QTextEdit* m_message;
};
