#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

#include "simulation.h"
#include "simulation_widget/simulation_widget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(QWidget *parent = nullptr);
		~MainWindow();

		public slots:
			void updateData();

	private:
		Ui::MainWindow *ui;
		QTimer *m_timer;
		world myworld;
		simulation_widget sim_widget = simulation_widget(myworld.wave_size, myworld.y_min, myworld.y_max);
};
#endif // MAINWINDOW_H
