#include "simulation_widget.h"
#include <QVBoxLayout>


simulation_widget::simulation_widget(
		QWidget *parent,
		int wave_size, 
		float x_min, float x_max,
		float y_min, float y_max,
		float z_min, float z_max
		){
	surface = new Q3DSurface();
	container = QWidget::createWindowContainer(surface);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(container);
	this->setLayout(layout);

	// Create surface series
	m_series = new QSurface3DSeries();
	surface->addSeries(m_series);

	data = new QSurfaceDataArray;
	data->reserve(wave_size);
	for (int i = 0; i < wave_size; i++) {
		QSurfaceDataRow* row = new QSurfaceDataRow;
		row->reserve(wave_size);
		for (int j = 0;j < wave_size; j++) {
			row->append(QSurfaceDataItem(QVector3D(float(j),
							0, 
							float(i))));

		}
		data->append(row);
	}

	m_series->dataProxy()->resetArray(data);

	surface->axisX()->setTitle("X-Axis");
	surface->axisY()->setTitle("Y-Axis");
	surface->axisZ()->setTitle("Z-Axis");
	surface->axisX()->setTitleVisible(true);
	surface->axisZ()->setTitleVisible(true);
	surface->axisY()->setTitleVisible(true);
	surface->axisX()->setRange(x_min, x_max);
	surface->axisY()->setRange(y_min, y_max);
	surface->axisZ()->setRange(z_min, z_max);

	// Enable selection and set camera
	surface->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow | QAbstract3DGraph::SelectionSlice);
	surface->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetIsometricLeft);


	surface->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow | QAbstract3DGraph::SelectionSlice);
	surface->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetIsometricLeft);

}

simulation_widget::~simulation_widget(){
	delete surface;
}

void simulation_widget::update(float *Ey, int wave_size){

	for (int z = 0;z < data->size();++z) {
		QSurfaceDataRow *row = data->at(z);
		for (int x = 0; x < row->size(); ++x) {
			QSurfaceDataItem &item = (*row)[x];
			QVector3D pos = item.position();
			int index = z + x*wave_size;
			pos.setY(Ey[index]);
			item.setPosition(pos);
		}
	}
	m_series->dataProxy()->resetArray(data);
}

