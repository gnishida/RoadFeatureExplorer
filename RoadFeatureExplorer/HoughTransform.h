#pragma once

#include "Polygon2D.h"
#include "BBox.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>

class HoughTransform {
private:
	Polygon2D area;
	float scale;
	BBox bbox;
	cv::Mat htSpace;

public:
	HoughTransform(const Polygon2D& area, float scale);
	~HoughTransform() {}

	void line(const QVector2D& v1, const QVector2D& v2);
	QVector2D maxPoint() const;
};

