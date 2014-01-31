#pragma once

#include "AbstractArea.h"
#include <vector>
#include <QVector2D>

class PolygonArea : public AbstractArea {
private:
	std::vector<QVector2D> polygon;

public:
	PolygonArea(void);
	~PolygonArea(void);

	void clear();
	void addPoint(const QVector2D& pt);
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

