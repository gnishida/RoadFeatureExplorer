#include "PolygonAreaBuilder.h"

PolygonAreaBuilder::PolygonAreaBuilder(void)
{
}

PolygonAreaBuilder::~PolygonAreaBuilder(void)
{
}

void PolygonAreaBuilder::init() {
	area.clear();
}

void PolygonAreaBuilder::addPoint(const QVector2D& pt) {
	area.addPoint(pt);
}

/**
 * 一番最後の点を動かす。
 * 通常、マウスなどで領域を選択する場合、マウスの移動に伴って、一番最後の点を動かしたい場合に、このメソッドを使用する。
 */
void PolygonAreaBuilder::movePoint(const QVector2D& pt) {
	if (area.size() == 0) return;

	area.p
}
