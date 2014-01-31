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
	return polygon.contains(pt);
}

QVector2D PolygonArea::midPt() const {
	QVector2D minCorner, maxCorner;
	Polygon2D::getLoopAABB(polygon, minCorner, maxCorner);

	return (minCorner + maxCorner) * 0.5f;
}

float PolygonArea::dx() const {
	QVector2D minCorner, maxCorner;
	QVector2D size = Polygon2D::getLoopAABB(polygon, minCorner, maxCorner);

	return size.x();
}

float PolygonArea::dy() const {
	QVector2D minCorner, maxCorner;
	QVector2D size = Polygon2D::getLoopAABB(polygon, minCorner, maxCorner);

	return size.y();
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
