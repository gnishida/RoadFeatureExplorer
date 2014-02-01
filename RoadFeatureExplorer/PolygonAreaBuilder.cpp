#include "PolygonAreaBuilder.h"

PolygonAreaBuilder::PolygonAreaBuilder() {
	_selecting = false;
}

void PolygonAreaBuilder::start(const QVector2D& pt) {
	_polyline.clear();
	_polyline.push_back(pt);

	_selecting = true;
}

void PolygonAreaBuilder::addPoint(const QVector2D& pt) {
	_polyline.push_back(pt);
}

void PolygonAreaBuilder::moveLastPoint(const QVector2D& pt) {
	if (_polyline.size() == 0) return;

	_polyline[_polyline.size() - 1] = pt;
}

void PolygonAreaBuilder::end() {
	_selecting = false;
}

bool PolygonAreaBuilder::selected() const {
	return !_selecting && _polyline.size() >= 3;
}

bool PolygonAreaBuilder::selecting() const {
	return _selecting;
}

Loop2D PolygonAreaBuilder::polyline() const {
	return _polyline;
}

PolygonArea PolygonAreaBuilder::polygonArea() const {
	PolygonArea area;

	for (int i = 0; i < _polyline.size(); ++i) {
		area.addPoint(_polyline[i]);
	}

	return area;
}
