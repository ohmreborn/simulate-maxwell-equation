#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>

#include "simulation.h"
#include "simulation_widget/simulation_widget.h"
#include "time_widget/time_widget.h"

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
		simulation_widget* sim_widget;
		myplot *time_widget;
};
#endif // MAINWINDOW_H
