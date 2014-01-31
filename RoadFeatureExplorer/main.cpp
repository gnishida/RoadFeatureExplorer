#include "MainWindow.h"
#include <QtGui/QApplication>
#include "ConvexHull.h"
#include "Util.h"

int main(int argc, char *argv[]) {
	// ConvexHull test
	ConvexHull ch;
	ch.addPoint(QVector2D(0, 0));
	ch.addPoint(QVector2D(10, 0));
	ch.addPoint(QVector2D(10, 10));

	std::vector<QVector2D> hull;
	ch.convexHull(hull);
	for (int i = 0; i < hull.size(); i++) {
		std::cout << hull[i].x() << "," << hull[i].y() << std::endl;
	}

	QVector2D size;
	QMatrix4x4 transMat;
	Util::getOBB(hull, size, transMat);
	std::cout << size.x() << "," <<size.y() << std::endl;



	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
