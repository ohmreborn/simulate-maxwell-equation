#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mysim/mysim.h"
#include "simulation/source_wave/source_wave.h"

#include <QVBoxLayout>
#include <QTimer>

#include <iostream>
#include <cmath>
#include <cstdint>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	real_signal = new simulation(100);
	noise = new simulation(100);
	real_signal->add(&simple_source);
	real_signal->add(&noise_source);
	noise->add(&noise_source);

	uint64_t window_size = 32;
	world = new Mysim(real_signal, noise, window_size);

	sim_widget = new simulation_widget(
			ui->simulation_widget, 
			real_signal->wave_size, 
			real_signal->x_min, real_signal->x_max,
			real_signal->y_min, real_signal->y_max,
			real_signal->z_min, real_signal->z_max);
	QVBoxLayout *simulation_layout = new QVBoxLayout(ui->simulation_widget);
	simulation_layout->addWidget(sim_widget);
	ui->simulation_widget->setLayout(simulation_layout);

	time_widget = new myplot(ui->time_widget,
			real_signal->wave_size,
			real_signal->x_min, real_signal->x_max,
			real_signal->y_min, 10
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
	delete world;
}

void MainWindow::updateData()
{
	world->update();
	sim_widget->update(world->real_signal->Ey, world->real_signal->wave_size);
	time_widget->update(world->total_error);
}

