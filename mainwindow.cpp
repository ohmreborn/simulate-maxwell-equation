#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QTimer>

#include <iostream>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QVBoxLayout *layout = new QVBoxLayout(ui->simulation_widget);
	layout->addWidget(sim_widget.container);
	layout->setContentsMargins(0, 0, 0, 0);
	ui->simulation_widget->setLayout(layout);

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

