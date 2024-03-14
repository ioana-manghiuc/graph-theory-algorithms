#include "MainWindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui->setupUi(this);
    m_graph.ReadMatrixFromFile();
    m_graph.CreateGraphFromMatrix();
    m_graph.BFS(m_graph.FindStartingNode());
    update();
    m_graph.FindExitNodes();
    m_graph.AllExitPaths();
}

MainWindow::~MainWindow()
{}

void MainWindow::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true); //smoothes the shapes when drawn
    std::vector<Node> nodes = m_graph.GetNodes();
    std::vector<std::vector<int>> matrix = m_graph.GetMatrix();
    std::vector<Arch> arches = m_graph.GetArches();
    std::vector<Arch> roads = m_graph.GetExitRoads();

    if (ui->drawLabyrinth->isChecked())
    {
        int sizeOfGraph = m_graph.GetSize();

        p.setPen(QPen(Qt::black, 3));
        int length = 47;
        int width = 47;

        for (int i = 1; i < sizeOfGraph + 1; i++)
        {
            for (int j = 1; j < sizeOfGraph + 1; j++)
            {
                p.drawRect(j * width, i * length, width, length);
                if (matrix[i][j] == 0)
                {
                    p.fillRect(j * width, i * length, width, length, Qt::black);
                }

                if (matrix[i][j] == 1)
                {
                    p.fillRect(j * width, i * length, width, length, Qt::white);
                }

                if (matrix[i][j] == 2)
                {
                    p.fillRect(j * width, i * length, width, length, Qt::darkRed);
                }

                if (matrix[i][j] == 3)
                {
                    p.fillRect(j * width, i * length, width, length, Qt::darkCyan);
                }
            }
        }
        update();
        if (ui->createRoads->isChecked())
        {
            int sizeOfGraph = m_graph.GetSize();
            int length = 47;
            int width = 47;

            for (int i = 1; i < sizeOfGraph + 1; i++)
            {
                for (int j = 1; j < sizeOfGraph + 1; j++)
                {
                    if (matrix[i][j] == 7)
                    {
                        p.fillRect(j * width, i * length, width, length, Qt::green);
                        p.setPen(QPen(Qt::black, 2));
                        p.drawRect(j * width, i * length, width, length);
                    }
                }
            }
            update();
        }
    }

    if (ui->drawGraph->isChecked())
    {
        for (Node& n : nodes)
        {
            p.setPen(QPen(Qt::black,2));
            QRect r(n.GetX() * 51, n.GetY() * 51, 26, 26);
            if (n.GetColour() == 1)
            {
                p.setBrush(Qt::white);
            }
            else if (n.GetColour() == 2)
            {
                p.setBrush(Qt::darkRed);
            }
            else if (n.GetColour() == 3)
            {
                p.setBrush(Qt::darkCyan);
            }

            p.drawRect(r);
            QString s; s.setNum(n.GetValue());
            p.drawText(r, Qt::AlignCenter, s);
        }
        update();

        for (Arch& a : arches)
        {
            QRect r1(a.GetFirstNode().GetX() * 51, a.GetFirstNode().GetY() * 51, 26, 26);
            QRect r2(a.GetSecondNode().GetX() * 51, a.GetSecondNode().GetY() * 51, 26, 26);
            QPoint center1 = r1.center();
            QPoint center2 = r2.center();
            QPoint start = center1 + 0.6 * (center2 - center1);
            QPoint end = center2 + 0.6 * (center1 - center2);
            p.drawLine(start, end);
        }

        if (ui->checkBox->isChecked())
        {
            for (Arch& a : roads)
            {
                QRect r1(a.GetFirstNode().GetX() * 51, a.GetFirstNode().GetY() * 51, 26, 26);
                QRect r2(a.GetSecondNode().GetX() * 51, a.GetSecondNode().GetY() * 51, 26, 26);
                QPoint center1 = r1.center();
                QPoint center2 = r2.center();
                QPoint start = center1 + 0.6 * (center2 - center1);
                QPoint end = center2 + 0.6 * (center1 - center2);
                QLine line = QLine(start, end);
                p.setPen(QPen(Qt::green, 3));
                p.drawLine(line);

            }
        }
    }
}

void MainWindow::on_drawLabyrinth_clicked() {}

void MainWindow::on_createRoads_clicked() {}

void MainWindow::on_drawGraph_clicked() {}

void MainWindow::on_checkBox_clicked() {}

