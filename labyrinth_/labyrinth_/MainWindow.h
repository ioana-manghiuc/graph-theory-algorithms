#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Graph.h"
#include <QMainWindow>
#include <QPaintEvent>
#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    virtual void paintEvent(QPaintEvent* e);

private slots:
    void on_drawLabyrinth_clicked();

    void on_createRoads_clicked();

    void on_drawGraph_clicked();

    void on_checkBox_clicked();

private:
    Graph m_graph;
    Ui::MainWindowClass* ui;
};

#endif // MAINWINDOW_H