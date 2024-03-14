#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->setFixedHeight(467);
    this->setFixedWidth(613);

    setWindowTitle("draw graph");

    m_primAlgorithm = new QPushButton("prim algorithm", this);
    m_primAlgorithm->setGeometry(20, 27, 157, 30);
    m_primAlgorithm->setStyleSheet("background-color: #e6e6ff;  color: #660066");
    connect(m_primAlgorithm, &QPushButton::clicked, this, &MainWindow::onPrimAlgorithmClicked);

    m_KruskalAlgorithm = new QPushButton("kruskal algorithm", this);
    m_KruskalAlgorithm->setGeometry(20, 73, 189, 30);
    m_KruskalAlgorithm->setStyleSheet("background-color: #e6e6ff;  color: #660066");
    connect(m_KruskalAlgorithm, &QPushButton::clicked, this, &MainWindow::onKruskalAlgorithmClicked);
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
            m_graph.WriteInCostMatrix();
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
                        m_graph.WriteInCostMatrix();
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

void MainWindow::DrawMinimumCostArches(QColor archColor)
{
    QPainter p(&canvas);
    p.setFont(QFont("Cascadia Code SemiBold", 11));
    p.setRenderHints(QPainter::TextAntialiasing, true);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);

    std::vector<Node> noduri = m_graph.GetNodes();
    for (Node& n : noduri)
    {
        QRect r(n.GetX() - 17, n.GetY() - 17, 34, 34);
        QPen pen;
        pen.setColor(QColor("#660066"));
        pen.setWidth(2);
        p.setPen(pen);
        p.drawEllipse(r);

        QString s;
        s.setNum(n.GetValue() + 1);
        p.drawText(r, Qt::AlignCenter, s);
    }

    std::vector<Arch> arches = m_graph.GetMinimumCostArches();

    for (int i = 0; i < arches.size(); i++)
    {
        QPointF p3(arches[i].GetFirstNode().GetX(), arches[i].GetFirstNode().GetY());
        QPointF p4(arches[i].GetSecondNode().GetX(), arches[i].GetSecondNode().GetY());
        QPointF adjustedStart = CalculateIntersectionPoint(p3, p4, 17);
        QPointF adjustedEnd = CalculateIntersectionPoint(p4, p3, 17);
        QLineF line(adjustedStart, adjustedEnd);

        QPen archPen;
        archPen.setColor(archColor);
        archPen.setWidth(2);
        p.setPen(archPen);
        p.drawLine(line);
    }
}

void MainWindow::paintEvent(QPaintEvent* e)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing, true);
	QPen pen;
	pen.setColor(QColor("#660066")); pen.setWidth(3);
	p.setPen(pen);
	std::vector<Node> noduri = m_graph.GetNodes();

	for (Node& n : noduri)
	{
		QRect r(n.GetX() - 17, n.GetY() - 17, 34, 34);
		QPen pen;
		pen.setColor(QColor("#660066"));
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
		{
			for (Arch& a : arce)
			{
				QPointF p3(a.GetFirstNode().GetX(), a.GetFirstNode().GetY());
				QPointF p4(a.GetSecondNode().GetX(), a.GetSecondNode().GetY());
				QPointF adjustedStart = CalculateIntersectionPoint(p3, p4, 17);
				QPointF adjustedEnd = CalculateIntersectionPoint(p4, p3, 17);
				QLineF line(adjustedStart, adjustedEnd);

				QPen archPen;
				archPen.setColor(QColor("#660066"));
				archPen.setWidth(2);
				p.setPen(archPen);
				p.drawLine(line);

				QPointF middle((p3.x() + p4.x()) / 2, (p3.y() + p4.y()) / 2);
				QString s; s.setNum(a.GetCost());
				QPen pen;
				pen.setColor(QColor("#bf4080"));
				p.setPen(pen);
				p.drawText(middle, s);
			}
		}
	}

	if (m_firstNode.GetValue() >= 0)
	{
		QRect r(m_firstNode.GetX() - 17, m_firstNode.GetY() - 17, 34, 34);
		QPen pen;
		pen.setColor(QColor("#cc99ff")); pen.setWidth(3);
		p.setPen(pen);
		p.drawEllipse(r);
		QString s;
		s.setNum(m_firstNode.GetValue() + 1); p.drawText(r, Qt::AlignCenter, s);
	}

}

void MainWindow::onPrimAlgorithmClicked()
{
    m_graph.AddToAdjacencyList();
    m_graph.PrimAlgorithm();

    m_primAlgorithmOutput = new QMainWindow;
    m_primAlgorithmOutput->setFixedHeight(467);
    m_primAlgorithmOutput->setFixedWidth(613);
    m_primAlgorithmOutput->setWindowTitle("prim algorithm output");

    canvas = QPixmap(m_primAlgorithmOutput->size());
    canvas.fill(Qt::white);

    QLabel* primoutputlabel = new QLabel(m_primAlgorithmOutput);
    primoutputlabel->setGeometry(0, 0, 613, 467);
    primoutputlabel->setScaledContents(false);
    primoutputlabel->setPixmap(canvas.scaledToWidth(primoutputlabel->width(), Qt::SmoothTransformation));

    QWidget* primwidget = new QWidget(m_primAlgorithmOutput);
    primwidget->setGeometry(m_primAlgorithmOutput->geometry());
    m_primAlgorithmOutput->setCentralWidget(primwidget);
    m_primAlgorithmOutput->show();

    DrawMinimumCostArches(QColor("#cc99ff"));
    primoutputlabel->setPixmap(canvas);
    
    update();
}

void MainWindow::onKruskalAlgorithmClicked()
{
    m_graph.AddToAdjacencyList();
    m_graph.KruskalAlgorithm();

    m_KruskalWindow = new QMainWindow;
    m_KruskalWindow->setFixedHeight(467);
    m_KruskalWindow->setFixedWidth(613);
    m_KruskalWindow->setWindowTitle("kruskal algorithm output");

    canvas = QPixmap(m_KruskalWindow->size());
    canvas.fill(Qt::white);

    QLabel* kruskalLabel = new QLabel(m_KruskalWindow);
    kruskalLabel->setGeometry(0, 0, 613, 467);
    kruskalLabel->setScaledContents(false);
    kruskalLabel->setPixmap(canvas.scaledToWidth(kruskalLabel->width(), Qt::SmoothTransformation));

    QWidget* kruskalwidget = new QWidget(m_KruskalWindow);
    kruskalwidget->setGeometry(m_KruskalWindow->geometry());
    m_KruskalWindow->setCentralWidget(kruskalwidget);
    m_KruskalWindow->show();

    DrawMinimumCostArches(QColor("#bf4080"));
    kruskalLabel->setPixmap(canvas);

    update();
}