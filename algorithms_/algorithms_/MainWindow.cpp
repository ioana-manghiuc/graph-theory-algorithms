#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->setFixedHeight(467);
    this->setFixedWidth(613);

    setWindowTitle("draw graph");
    QColor customColor("#RRGGBB");

    m_sortedGraph = new QPushButton("generate sorted graph", this);
    m_sortedGraph->setGeometry(10, 47, 207, 30);
    m_sortedGraph->setStyleSheet("background-color: #f7edf2;  color: #800000");
    connect(m_sortedGraph, &QPushButton::clicked, this, &MainWindow::onPrintSortedGraphClicked);

    m_clearWindow = new QPushButton("CLEAR SCREEN", this);
    m_clearWindow->setGeometry(460, 17, 143, 30);
    m_clearWindow->setStyleSheet("background-color: #fcf0e8; color: #e65c00");
    connect(m_clearWindow, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);

    m_findRoot = new QPushButton("find root", this);
    m_findRoot->setGeometry(10, 84, 207, 30);
    m_findRoot->setStyleSheet("background-color: #f7edf2;  color: #800000");
    connect(m_findRoot, &QPushButton::clicked, this, &MainWindow::onFindButtonClicked);

}

MainWindow::~MainWindow()
{}

void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::RightButton)
    {
        if (!m_graph.NodeExists(e))
        {
            m_graph.AddNode(e->pos());
            update();
            m_graph.WriteMatrixInFile("adjacencymatrix.txt");
        }
        else
        {
            qDebug() << "\nNode Overlapping Check - a node already exists!\n";
        }
    }
    else if (e->button() == Qt::LeftButton)
    {
        std::vector<Node> nodes = m_graph.GetNodes();
        for (Node& n : nodes)
        {
            if (fabs(e->pos().x() - n.GetX()) < 15 && fabs(e->pos().y() - n.GetY()) < 15)
            {
                if (m_firstNode.GetValue() == -1)
                {
                    m_firstNode = n; 
                }
                else
                {
                    if (!m_graph.IsMultigraph(m_firstNode, n))
                    {
                        m_graph.AddArch(Arch(m_firstNode, n));
                        m_firstNode.SetValue(-1);
                        update();
                        m_graph.WriteMatrixInFile("adjacencymatrix.txt");
                    }
                    else
                    {
                        qDebug() << "\nMultigraph Check - an arch already exists in that direction\n";
                    }
                }
                update();
                break;
            }
        }
    }
}

QPointF MainWindow::calculateIntersectionPoint(QPointF center, QPointF point, qreal radius)
{
    qreal dx = point.x() - center.x();
    qreal dy = point.y() - center.y();
    qreal distance = sqrt(dx * dx + dy * dy);

    if (distance > 0)
    {
        qreal scale = radius / distance;
        return center + QPointF(dx * scale, dy * scale);
    }
    return center;
}

void MainWindow::paintOrientedArch(QPainter& paint, QPointF start, QPointF end)
{
    paint.setRenderHint(QPainter::Antialiasing, true);

    qreal arrowSize = 11; // size of head
    paint.setPen(Qt::darkRed);
    paint.setBrush(QColor("#ff751a")); // fills arrow

    QPointF adjustedStart = calculateIntersectionPoint(start, end, 15);
    QPointF adjustedEnd = calculateIntersectionPoint(end, start, 15);

    QLineF line(adjustedEnd, adjustedStart);
    double angle = std::atan2(-line.dy(), line.dx());
    QPointF arrow_p1 = line.p1() + QPoint(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
    QPointF arrow_p2 = line.p1() + QPoint(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);
    QPolygonF arrowHead;
    arrowHead << line.p1() << arrow_p1 << arrow_p2;

    paint.drawLine(line);
    paint.drawPolygon(arrowHead);
}

void MainWindow::drawSortedGraph()
{
    QPainter p(&sortedGraphCanvas);
    p.setFont(QFont("Cascadia Code SemiBold", 11));
    p.setRenderHints(QPainter::TextAntialiasing, true);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);

    std::vector<Node> noduri = m_graph.GetNodes();
    for (Node& n : noduri)
    {
        QRect r(n.GetX() - 15, n.GetY() - 15, 30, 30);
        QPen pen;
        pen.setColor(Qt::darkRed);
        pen.setWidth(2);
        p.setPen(pen);
        p.drawEllipse(r);

        QString s;
        s.setNum(n.GetValue() + 1);
        p.drawText(r, Qt::AlignCenter, s);
    }

    std::vector<Arch> arce = m_graph.GetSortedArches();
    for (Arch& a : arce)
    {
        QPointF p1(a.GetFirstNode().GetX(), a.GetFirstNode().GetY());
        QPointF p2(a.GetSecondNode().GetX(), a.GetSecondNode().GetY());
        paintOrientedArch(p, p1, p2);
    }

    if (m_firstNode.GetValue() >= 0)
    {
        QRect r(m_firstNode.GetX() - 15, m_firstNode.GetY() - 15, 30, 30);
        QPen pen;
        pen.setColor(QColor("#ff751a"));
        pen.setWidth(3);
        p.setPen(pen);
        p.drawEllipse(r);
        QString s;
        s.setNum(m_firstNode.GetValue() + 1);
        p.drawText(r, Qt::AlignCenter, s);
    }
}

void MainWindow::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    std::vector<Node> noduri = m_graph.GetNodes();

    if (m_sortedGraph->isChecked())
    {
        drawSortedGraph();
    }
    else
    {
        for (Node& n : noduri)
        {
            QRect r(n.GetX() - 15, n.GetY() - 15, 30, 30);
            QPen pen;
            pen.setColor(Qt::darkRed);
            pen.setWidth(2);
            p.setPen(pen);
            p.drawEllipse(r);

            QString s;
            s.setNum(n.GetValue() + 1);
            p.drawText(r, Qt::AlignCenter, s);
        }

        std::vector<Arch> arce = m_graph.GetArches();
        if (!arce.empty())
        {
            for (Arch& a : arce)
            {
                if (ui.orientedGraph->isChecked())
                {
                    QPointF p1(a.GetFirstNode().GetX(), a.GetFirstNode().GetY());
                    QPointF p2(a.GetSecondNode().GetX(), a.GetSecondNode().GetY());
                    paintOrientedArch(p, p1, p2);
                }
                else
                {
                    QPointF p3(a.GetFirstNode().GetX(), a.GetFirstNode().GetY());
                    QPointF p4(a.GetSecondNode().GetX(), a.GetSecondNode().GetY());
                    QPointF adjustedStart = calculateIntersectionPoint(p3, p4, 15);
                    QPointF adjustedEnd = calculateIntersectionPoint(p4, p3, 15);
                    p.drawLine(adjustedStart, adjustedEnd);
                }
            }
        }
        else
        {
            qDebug() << "Sorting didn't take place.\n";
        }

        if (m_firstNode.GetValue() >= 0)
        {
            QRect r(m_firstNode.GetX() - 15, m_firstNode.GetY() - 15, 30, 30);
            QPen pen;
            pen.setColor(QColor("#b30059")); pen.setWidth(3);
            p.setPen(pen);
            p.drawEllipse(r);
            QString s;
            s.setNum(m_firstNode.GetValue() + 1); p.drawText(r, Qt::AlignCenter, s);
        }
    }
}


void MainWindow::on_orientedGraph_clicked()
{
    m_graph.SetIsOriented(true);
}

void MainWindow::onPrintSortedGraphClicked()
{
    m_graph.AddToAdjacencyList();
    m_graph.PrintAdjacencyList("adjacencylist.txt");

    if (m_graph.IsOriented())
    {
        m_graph.TopologicalSort(m_graph.GetStartNode());
    }
    else
    {
        qDebug() << "Graph must be oriented in order to be topologically sorted.\n";
    }

    sortedGraph = new QMainWindow;
    sortedGraph->setFixedHeight(467);
    sortedGraph->setFixedWidth(613);
    sortedGraph->setWindowTitle("topological sort");

    sortedGraphCanvas = QPixmap(sortedGraph->size());
    sortedGraphCanvas.fill(Qt::white);

    QLabel* sortedGraphLabel = new QLabel(sortedGraph);
    sortedGraphLabel->setGeometry(0, 0, 613, 467);
    sortedGraphLabel->setScaledContents(false);
    sortedGraphLabel->setPixmap(sortedGraphCanvas.scaledToWidth(sortedGraphLabel->width(), Qt::SmoothTransformation));

    QWidget* sortedGraphWidget = new QWidget(sortedGraph);
    sortedGraphWidget->setGeometry(sortedGraph->geometry());
    sortedGraph->setCentralWidget(sortedGraphWidget);
    sortedGraph->show();
    if (m_graph.HasCycles() == false)
    {
        drawSortedGraph();
        sortedGraphLabel->setPixmap(sortedGraphCanvas);
    }
    else
    {
        qDebug() << "Graph has cycles.\n";
    }
}

void MainWindow::onClearButtonClicked()
{
    m_graph.ClearGraph();
    update();
}

void MainWindow::onFindButtonClicked()
{
    root = new QMainWindow;
    root->setFixedHeight(133);
    root->setFixedWidth(413);
    root->setWindowTitle("root of graph");

    m_text = new QTextEdit(root);
    m_text->setFont(QFont("Cascadia Code SemiBold", 11));
    root->setCentralWidget(m_text);

    m_graph.AddToAdjacencyList();
    m_graph.PrintAdjacencyList("adjacencylist.txt");
    if (m_graph.IsOriented())
    {
        m_graph.FindRootOfGraph();
    }
    else
    {
        qDebug() << "Graph must be oriented in order to be a tree.\n";
    }

    if (m_graph.IsTree())
    {
        const QString m = "The graph is a tree.\n";
        showMessage(m);
        if (m_graph.GetRoot().GetValue() != -13)
        {
            const QString message = "Root of Graph is: " + QString::number(m_graph.GetRoot().GetValue() + 1);
            showMessage(message);
            m_text->show();
        }
        else
        {
            showMessage("Graph does not have a root.\n");
            m_text->show();
        }
    }
    else
    {
        showMessage("Graph is not a tree.\n");
        m_text->show();
    }

    root->show();
}

void MainWindow::showMessage(const QString& message)
{
    if (m_text)
    {
        m_text->append(message);
    }
    qDebug() << message;
}
