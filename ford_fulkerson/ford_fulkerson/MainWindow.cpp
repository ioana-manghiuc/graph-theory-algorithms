#include "MainWindow.h"
#include <QPainterPath>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->setFixedHeight(513);
    this->setFixedWidth(756);
    setFont(QFont("Cascadia Code SemiBold", 11));
    setWindowTitle("draw graph");

    m_isOriented = new QCheckBox("directed graph", this);
    m_isOriented->setGeometry(20, 27, 157, 30);
    m_isOriented->setStyleSheet("color:#005580");
    connect(m_isOriented, &QCheckBox::clicked, this, &MainWindow::onIsOrientedClicked);

    m_fordFulkerson = new QPushButton("ford-fulkerson", this);
    m_fordFulkerson->setGeometry(20, 67, 157, 30);
    m_fordFulkerson->setStyleSheet("background-color: #b3cccc;  color: #005580");
    connect(m_fordFulkerson, &QPushButton::clicked, this, &MainWindow::onFordFulkersonClicked);
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
            m_graph.WriteInCapacityMatrix();
            update();
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
                        Arch a(m_firstNode, n);
                        m_graph.AddArch(a);
                        m_firstNode.SetValue(-1);
                        update();
                        m_graph.WriteInCapacityMatrix();
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

QPointF MainWindow::CalculateIntersectionPoint(QPointF center, QPointF point, qreal radius)
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

void MainWindow::PaintOrientedArch(QPainter& paint, Node first, Node second, float capacity, const QColor& color)
{
    paint.setPen(QPen(color, 2));
    paint.setRenderHint(QPainter::Antialiasing, true);
    qreal arrowSize = 11; // size of head

    QPainterPath path;
    QPointF start(first.GetX(), first.GetY());
    QPointF end(second.GetX(), second.GetY());
    QPointF adjustedStart = CalculateIntersectionPoint(start, end, 17);
    QPointF adjustedEnd = CalculateIntersectionPoint(end, start, 17);

    bool direction = first.GetValue() < second.GetValue();

    if (direction)
    {
        QPointF controlPoint = QPointF((adjustedStart.x() + adjustedEnd.x()) / 2 + 50, (adjustedStart.y() + adjustedEnd.y()) / 2);
        path.moveTo(adjustedStart);
        path.quadTo(controlPoint, adjustedEnd);
    }
    else
    {
        QPointF controlPoint = QPointF((adjustedStart.x() + adjustedEnd.x()) / 2 - 50, (adjustedStart.y() + adjustedEnd.y()) / 2);
        path.moveTo(adjustedEnd);
        path.quadTo(controlPoint, adjustedStart);
    }
    paint.drawPath(path);

    QLineF line(adjustedEnd, adjustedStart);
    double angle = std::atan2(-line.dy(), line.dx());
    QPointF arrow_p1 = line.p1() + QPoint(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
    QPointF arrow_p2 = line.p1() + QPoint(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);
    QPolygonF arrowHead;
    arrowHead << line.p1() << arrow_p1 << arrow_p2;

    paint.setBrush(QBrush(QColor("#ecc6d8")));
    paint.drawPolygon(arrowHead);
    paint.setBrush(QBrush(QColor(Qt::transparent)));

    QPointF middle = path.pointAtPercent(0.5);
    QString scap; scap.setNum(capacity);
    QPen pen;
    pen.setColor(QColor("#ac39ac"));
    paint.setPen(pen);
    paint.drawText(middle, scap);
}

void MainWindow::onIsOrientedClicked()
{
    m_graph.SetIsOriented(true);
}

void MainWindow::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    pen.setColor(QColor("#005580")); pen.setWidth(3);
    p.setPen(pen);
    std::vector<Node> noduri = m_graph.GetNodes();

    for (Node& n : noduri)
    {
        QRect r(n.GetX() - 17, n.GetY() - 17, 34, 34);
        QPen pen;
        pen.setColor(QColor("#005580"));
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
			if (m_isOriented->isChecked())
			{
				PaintOrientedArch(p, a.GetFirstNode(), a.GetSecondNode(), a.GetCapacity(), QColor("#005580"));
			}
			else
			{
				QPointF p3(a.GetFirstNode().GetX(), a.GetFirstNode().GetY());
				QPointF p4(a.GetSecondNode().GetX(), a.GetSecondNode().GetY());
				QPointF adjustedStart = CalculateIntersectionPoint(p3, p4, 17);
				QPointF adjustedEnd = CalculateIntersectionPoint(p4, p3, 17);
				QLineF line(adjustedStart, adjustedEnd);

				QPen archPen;
				archPen.setColor(QColor("#005580"));
				archPen.setWidth(2);
				p.setPen(archPen);
				p.drawLine(line);

				QPointF middle((p3.x() + p4.x()) / 2, (p3.y() + p4.y()) / 2);
				QString s; s.setNum(a.GetCapacity());
				QPen pen;
				pen.setColor(QColor("#ac39ac"));
				p.setPen(pen);
				p.drawText(middle, s);
			}
		}
	}

    if (m_firstNode.GetValue() >= 0)
    {
        QRect r(m_firstNode.GetX() - 17, m_firstNode.GetY() - 17, 34, 34);
        QPen pen;
        pen.setColor(QColor("#999900")); pen.setWidth(3);
        p.setPen(pen);
        p.drawEllipse(r);
        QString s;
        s.setNum(m_firstNode.GetValue() + 1); p.drawText(r, Qt::AlignCenter, s);
    }
}

void MainWindow::onFordFulkersonClicked()
{
    m_graph.AddToAdjacencyList();
    Node start = m_graph.GetNodes()[0];
    Node finish = m_graph.GetNodes()[m_graph.GetNodes().size() - 1];
    float maxFlow = m_graph.FordFulkerson(start, finish);
    QLabel* flow = new QLabel("MAX FLOW: ", this);
    flow->setGeometry(613, 27, 110, 30);
    flow->setStyleSheet("color: #005580");
    if (maxFlow != 0)
    {
        QString text = "MAX FLOW: " + QString::number(maxFlow);
        flow->setText(text);
        flow->show();
    }
    qDebug() << "MAX FLOW: " << m_graph.FordFulkerson(start, finish);
}
