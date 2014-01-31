#include "PolygonArea.h"
#include "Util.h"

PolygonArea::PolygonArea() {
}

PolygonArea::~PolygonArea() {
}

void PolygonArea::clear() {
	polygon.clear();
}

void PolygonArea::addPoint(const QVector2D& pt) {
	polygon.push_back(pt);
}

bool PolygonArea::contains(const QVector2D& pt) const {
	int size = polygon.size();

	for (int i = 0; i < size; ++i) {
		QVector2D vec1 = polygon[(i + 1) % size] - polygon[i];
		QVector2D vec2 = pt - polygon[i];
		if (vec1.x() * vec2.y() - vec1.y() * vec2.x() > 0) return false;
	}

	return true;
}

QVector2D PolygonArea::midPt() const {
	QVector2D minCorner, maxCorner;
	Util::getAABB(polygon, minCorner, maxCorner);

	return (minCorner + maxCorner) * 0.5f;
}

float PolygonArea::dx() const {
	QVector2D minCorner, maxCorner;
	Util::getAABB(polygon, minCorner, maxCorner);

	return maxCorner.x() - minCorner.x();
}

float PolygonArea::dy() const {
	QVector2D minCorner, maxCorner;
	Util::getAABB(polygon, minCorner, maxCorner);

	return maxCorner.y() - minCorner.y();
}

void PolygonArea::translate(float x, float y) {
	for (int i = 0; i < polygon.size(); ++i) {
		polygon[i].setX(polygon[i].x());
		polygon[i].setY(polygon[i].y());
	}
}

void PolygonArea::resize(const QVector2D& pt) {
}

bool PolygonArea::hitTest(const QVector2D& pt) const {
	return contains(pt);
}

bool PolygonArea::hitTestResizingPoint(const QVector2D& pt) const {
	return false;
}

std::vector<QVector2D> PolygonArea::polyline() const {
	return polygon;
}
