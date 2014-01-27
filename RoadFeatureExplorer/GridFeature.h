#pragma once

#include <QVector2D>
#include <opencv/cv.h>
#include <opencv/highgui.h>

class GridFeature {
public:
	int group_id;
	float angle1;		// 横方向の角度（第一象限）
	float angle2;		// 縦方向の角度（第二象限）
	cv::Mat length1;	// 横方向の長さのヒストグラム
	cv::Mat length2;	// 縦方向の長さのヒストグラム

	int count;
	QVector2D tempDir1;
	QVector2D tempDir2;

public:
	GridFeature(int group_id) : group_id(group_id) {}
	~GridFeature() {}

	void setAngle(float angle);
	void addDirection(const QVector2D& dir, float threshold);
	void computeDirection();
	bool isClose(const QVector2D& dir, float threshold);
};

