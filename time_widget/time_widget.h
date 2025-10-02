#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QVector>
#include <QPointF>
#include <QtMath>

class AnimatedPlotWidget : public QWidget
{
	Q_OBJECT
	public:
		explicit AnimatedPlotWidget(QWidget *parent = nullptr);

		public slots:
			void updateAnimation();

	protected:
		void paintEvent(QPaintEvent *event) override;

	private:
		QTimer *timer;
		QVector<QPointF> dataPoints;
		double time;
		int maxPoints;

		double minX, maxX, minY, maxY;
};
