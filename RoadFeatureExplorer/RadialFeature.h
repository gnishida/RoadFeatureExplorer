#pragma once

#include <QVector2D>

class RadialFeature {
public:
	int group_id;
	QVector2D center;

public:
	RadialFeature(int group_id) : group_id(group_id) {}
	~RadialFeature() {}
};

