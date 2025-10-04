#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "simulation/source_wave/source_wave.h"
#include "simulation/simulation.h"

#include <QVBoxLayout>
#include <QTimer>

#include <iostream>
#include <cmath>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	myworld = new simulation(100);
	myworld->add(&simple_source);
	myworld->add(&noise_source);

	sim_widget = new simulation_widget(
			ui->simulation_widget, 
			myworld->wave_size, 
			myworld->x_min, myworld->x_max,
			myworld->y_min, myworld->y_max,
			myworld->z_min, myworld->z_max);
	QVBoxLayout *simulation_layout = new QVBoxLayout(ui->simulation_widget);
	simulation_layout->addWidget(sim_widget);
	ui->simulation_widget->setLayout(simulation_layout);

	time_widget = new myplot(ui->time_widget,
			myworld->wave_size,
			myworld->x_min, myworld->x_max,
			myworld->y_min, myworld->y_max
			);
	QVBoxLayout *layout = new QVBoxLayout(ui->time_widget);
	layout->addWidget(time_widget);
	ui->time_widget->setLayout(layout);


	// Timer for animation
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &MainWindow::updateData);
	m_timer->start(50); 
}

MainWindow::~MainWindow()
{
    delete ui;
	delete myworld;
}

void MainWindow::updateData()
{
	myworld->update();
	sim_widget->update(myworld->Ey, myworld->wave_size);
	time_widget->update(myworld->Ey);
}

