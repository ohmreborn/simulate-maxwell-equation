#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QValue3DAxis>
#include <QWidget>

class simulation_widget {
	public: 
		Q3DSurface *surface;
		QWidget *container;
		QSurface3DSeries *m_series;
		QSurfaceDataArray *data;

		simulation_widget(int wave_size, double y_min, double y_max);
		~simulation_widget();

		void update(float *Ey, int wave_size);
};

