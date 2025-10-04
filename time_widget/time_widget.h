#ifndef TIME_WIDGET
#define TIME_WIDGET

#include <QWidget>
#include <QLineSeries>

class myplot : public QWidget 
{
	Q_OBJECT

	public:
		explicit myplot(QWidget *parent, 
				int wave_size,
				float x_min, float x_max,
				float y_min, float y_max
				);
		void update(float *Ey);
		~myplot();
		double phase = 0.1;

	private:
		QLineSeries *series;
};

#endif
