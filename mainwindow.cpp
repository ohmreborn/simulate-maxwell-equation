#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QTimer>

#include <iostream>
#include <cmath>

#include "./time_widget/time_widget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QVBoxLayout *simulation_widget_layout = new QVBoxLayout(ui->simulation_widget);
	simulation_widget_layout->addWidget(sim_widget.container);
	ui->simulation_widget->setLayout(simulation_widget_layout);

	AnimatedPlotWidget *plot = new AnimatedPlotWidget(ui->time_widget);
	QVBoxLayout *layout = new QVBoxLayout(ui->time_widget);
	layout->addWidget(plot);
	ui->time_widget->setLayout(layout);

	// Timer for animation
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &MainWindow::updateData);
	m_timer->start(16); // ~60 FPS
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateData()
{
	myworld.update();
	sim_widget.update(myworld.Ey, myworld.wave_size);

}

