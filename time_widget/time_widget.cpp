#include "time_widget.h"

#include <QChartView>
#include <QChart>
#include <QValueAxis>
#include <QVBoxLayout>

#include <cmath>

myplot::myplot(QWidget *parent, 
		int wave_size,
		float x_min, float x_max,
		float y_min, float y_max
		)
    : QWidget(parent)
{
    series = new QLineSeries(this);

	for (int i=0;i<wave_size;i++){
		series->append(i, 0);
	}
	
    // Create chart and add series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple Line Chart Example");

    // Create axes
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("X Axis");
    axisX->setRange(x_min, x_max);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Y Axis");
    axisY->setRange(y_min, y_max);

    // Attach axes to the series
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Create chart view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    this->setLayout(layout);
}

void myplot::update(float total_error){
	QVector<QPointF> points = series->pointsVector();
	for (int i = points.size()-1; i > 0; i--) {
		points[i].setY(points[i-1].y());
	}
	points[0].setY(total_error);
	series->replace(points);  // Single update to chart
}

myplot::~myplot(){
}

