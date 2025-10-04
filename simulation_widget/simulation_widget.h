#ifndef SIMULATION_WIDGET
#define SIMULATION_WIDGET

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QValue3DAxis>
#include <QWidget>

class simulation_widget:public QWidget {
	public: 
		Q3DSurface *surface;
		QWidget *container;
		QSurface3DSeries *m_series;
		QSurfaceDataArray *data;

		explicit simulation_widget(
				QWidget *parent,
				int wave_size, 
				float x_min, float x_max,
				float y_min, float y_max,
				float z_min, float z_max
				);
		~simulation_widget();

		void update(float *Ey, int wave_size);
};

#endif
