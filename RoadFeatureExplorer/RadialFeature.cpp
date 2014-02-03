#include "RadialFeature.h"

QColor RadialFeature::color() {
	return QColor((int)(fabs(center.x()) + fabs(center.y())) % 128 + 128, 0, 0);
}

/**
 * 領域を表すポリゴンを返却する。
 */
Polygon2D RadialFeature::polygon() {
	return _polygon;
}