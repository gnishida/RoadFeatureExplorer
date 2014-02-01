#pragma once

#include "RoadGraph.h"
#include "Renderable.h"
#include "BBox.h"
#include "Polygon2D.h"

class RoadGraphRenderer {
public:
	unsigned int dispList;

public:
	RoadGraphRenderer();

	void render(std::vector<RenderablePtr>& renderables);
	void renderOne(RenderablePtr renderable);

	void renderArea(const AbstractArea& area, GLenum lineType, float height);
	void renderDenseArea(const AbstractArea& area, float height);
	void renderPoint(const QVector2D& pt, float height);
	void renderPolyline(std::vector<QVector2D>& polyline, GLenum lineType, float height);

	//void tessellate(const Loop2D& polygon);
	void renderConcave(const Loop2D& polygon, const QColor& color, float height);
};

