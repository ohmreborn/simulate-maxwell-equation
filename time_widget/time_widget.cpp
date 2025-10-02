#include "time_widget.h"

AnimatedPlotWidget::AnimatedPlotWidget(QWidget *parent)
	: QWidget(parent), time(0.0), maxPoints(200)
{
	dataPoints.resize(maxPoints);
	for (int i=0; i<maxPoints; ++i) {
		double newValue = qSin(time + 0.1*i) + 0.3 * qCos(3 * (time + 0.1*i));
		dataPoints[i] = QPointF(time + 0.1*i, newValue);
	}

	minX = 0;
	maxX = 0.1 * maxPoints;
	minY = -1.5;
	maxY = 1.5;

	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &AnimatedPlotWidget::updateAnimation);
	timer->start(16);
}

void AnimatedPlotWidget::updateAnimation()
{
	for (int i=0;i<maxPoints;i++){
		double newValue = qSin(time + 0.1*i) + 0.3 * qCos(3 * (time + 0.1*i));
		dataPoints[i].setY(newValue);
	}
	time += 0.1;
	update();
}

void AnimatedPlotWidget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// Background with gradient
	QLinearGradient gradient(0, 0, 0, height());
	gradient.setColorAt(0, QColor(240, 240, 250));
	gradient.setColorAt(1, QColor(220, 230, 240));
	painter.fillRect(rect(), gradient);

	// Define margins and plot area
	int margin = 50;
	QRect plotArea = rect().adjusted(margin, margin, -margin, -margin);

	// Draw grid background
	painter.setPen(QPen(QColor(200, 200, 200), 1, Qt::DotLine));
	for (int i = 0; i <= 10; ++i) {
		int y = plotArea.top() + (plotArea.height() * i / 10);
		painter.drawLine(plotArea.left(), y, plotArea.right(), y);
	}
	for (int i = 0; i <= 10; ++i) {
		int x = plotArea.left() + (plotArea.width() * i / 10);
		painter.drawLine(x, plotArea.top(), x, plotArea.bottom());
	}

	// Draw axes
	painter.setPen(QPen(Qt::black, 2));
	painter.drawLine(plotArea.bottomLeft(), plotArea.bottomRight());
	painter.drawLine(plotArea.bottomLeft(), plotArea.topLeft());

	if (dataPoints.isEmpty()) return;

	// Map data points to screen coordinates
	auto mapToScreen = [&](const QPointF& dataPoint) -> QPointF {
		double x = plotArea.left() + 
			(dataPoint.x() - minX) / (maxX - minX) * plotArea.width();
		double y = plotArea.bottom() - 
			(dataPoint.y() - minY) / (maxY - minY) * plotArea.height();
		return QPointF(x, y);
	};

	// Draw the animated line with gradient effect
	QPen linePen(Qt::blue, 3);
	linePen.setCapStyle(Qt::RoundCap);
	linePen.setJoinStyle(Qt::RoundJoin);

	for (int i = 0; i < dataPoints.size() - 1; ++i) {
		QPointF p1 = mapToScreen(dataPoints[i]);
		QPointF p2 = mapToScreen(dataPoints[i + 1]);

		// Fade effect: older points are more transparent
		double alpha = (double)i / dataPoints.size();
		QColor lineColor = QColor::fromRgbF(0.2, 0.4, 1.0, alpha);
		linePen.setColor(lineColor);
		painter.setPen(linePen);

		painter.drawLine(p1, p2);
	}

	// Draw the current point (head of the line)
	if (!dataPoints.isEmpty()) {
		QPointF currentPoint = mapToScreen(dataPoints.last());
		painter.setPen(QPen(Qt::red, 2));
		painter.setBrush(Qt::red);
		painter.drawEllipse(currentPoint, 5, 5);
	}

	// Draw axis labels
	painter.setPen(Qt::black);
	QFont font = painter.font();
	font.setPointSize(10);
	painter.setFont(font);

	painter.drawText(plotArea.bottomLeft() - QPoint(30, -5), 
			QString::number(minY, 'f', 1));
	painter.drawText(plotArea.topLeft() - QPoint(30, 0), 
			QString::number(maxY, 'f', 1));
	painter.drawText(plotArea.bottomRight() + QPoint(-30, 20), 
			QString("Time: %1").arg(time, 0, 'f', 1));
}
