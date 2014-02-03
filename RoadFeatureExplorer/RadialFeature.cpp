#include "RadialFeature.h"

QColor RadialFeature::color() {
	return QColor((int)(center.x() + center.y()) % 255, 0, 0);
}

/**
 * 領域を表すポリゴンを返却する。
 */
Polygon2D RadialFeature::polygon() {
	return _polygon;
}