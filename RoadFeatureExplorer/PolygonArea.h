#pragma once

#include "AbstractArea.h"
#include "Polygon2D.h"
#include <vector>
#include <QVector2D>

class PolygonArea : public AbstractArea {
private:
	Loop2D polygon;

public:
	PolygonArea();
	~PolygonArea();

	void clear();
	void addPoint(const QVector2D& pt);
	void moveLastPoint(const QVector2D& pt);

	bool contains(const QVector2D& pt) const;
	QVector2D midPt() const;
	float dx() const;
	float dy() const;
	void translate(float x, float y);
	void resize(const QVector2D& pt);
	bool hitTest(const QVector2D& pt) const;
	bool hitTestResizingPoint(const QVector2D& pt) const;
	std::vector<QVector2D> polyline() const;
};

