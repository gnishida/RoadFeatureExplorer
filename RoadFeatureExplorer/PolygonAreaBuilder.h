#pragma once

#include <Polygon2D.h>
#include "common.h"

class PolygonAreaBuilder {
private:
	bool _selecting;
	Polyline2D _polyline;

public:
	PolygonAreaBuilder();
	~PolygonAreaBuilder() {}

	void start(const QVector2D& pt);
	void addPoint(const QVector2D& pt);
	void moveLastPoint(const QVector2D& pt);
	void end();

	bool selected() const;
	bool selecting() const;
	Polygon2D polygon() const;
};

