#pragma once

#include "PolygonArea.h"

class PolygonAreaBuilder {
public:
	PolygonArea area;

public:
	PolygonAreaBuilder();
	~PolygonAreaBuilder();

	void init();
	void addPoint(const QVector2D& pt);
	void movePoint(const QVector2D& pt);
};

