#include "GridFeature.h"
#include "Util.h"

#ifndef M_PI
#define M_PI	3.141592653
#endif

/**
 * グリッド方向の近似値を使って、グリッドの角度を仮にセットする。
 */
void GridFeature::setAngle(float angle) {
	angle1 = angle;
	angle2 = angle + M_PI * 0.5f;

	count = 0;
	tempDir1 = QVector2D(0, 0);
	tempDir2 = QVector2D(0, 0);
}

/**
 * エッジ方向を追加しながら、グリッド方向の精度を上げる。
 * ただし、当該エッジの方向が、仮の方向から閾値よりも大きく外れている場合は、そのエッジの方向は無視する。
 */
void GridFeature::addDirection(const QVector2D& dir, float threshold) {
	QVector2D d = dir.normalized();

	float a1 = atan2f(d.y(), d.x());
	float a2 = atan2f(-d.y(), -d.x());

	float diff1 = Util::diffAngle(angle1, a1);
	float diff2 = Util::diffAngle(angle1, a2);
	float diff3 = Util::diffAngle(angle2, a1);
	float diff4 = Util::diffAngle(angle2, a2);
	float min_diff = std::min(std::min(diff1, diff2), std::min(diff3, diff4));

	// エッジの方向が、仮の方向から閾値よりも大きく外れている場合は、そのエッジの方向は無視する。
	if (min_diff > threshold) return;

	if (diff1 == min_diff) {
		tempDir1 += d;
		tempDir2 += QVector2D(-d.y(), d.x());
	} else if (diff2 == min_diff) {
		tempDir1 -= d;
		tempDir2 -= QVector2D(-d.y(), d.x());
	} else if (diff3 == min_diff) {
		tempDir1 += QVector2D(d.y(), -d.x());
		tempDir2 += d;
	} else {
		tempDir1 -= QVector2D(d.y(), -d.x());
		tempDir2 -= d;
	}

	count++;
}

/**
 * エッジの追加が終わったら、この関数を呼んで、グリッド方向をより正確な値に更新する。
 */
void GridFeature::computeDirection() {
	tempDir1 /= (float)count;
	tempDir2 /= (float)count;

	angle1 = atan2f(tempDir1.y(), tempDir1.x());
	angle2 = atan2f(tempDir2.y(), tempDir2.x());

	count = 0;
	tempDir1 = QVector2D(0, 0);
	tempDir2 = QVector2D(0, 0);
}

/**
 * 与えられたエッジの方向が、グリッド方向に近いかどうかチェックする。
 */
bool GridFeature::isClose(const QVector2D& dir, float threshold) {
	float a1 = atan2f(dir.y(), dir.x());
	float a2 = atan2f(-dir.y(), -dir.x());

	if (Util::diffAngle(angle1, a1) < threshold || Util::diffAngle(angle1, a2) < threshold || Util::diffAngle(angle2, a1) < threshold || Util::diffAngle(angle2, a2) < threshold) return true;
	else return false;
}
