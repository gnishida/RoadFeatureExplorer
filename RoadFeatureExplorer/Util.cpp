﻿#include "Util.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define SQR(x)		((x) * (x))

const float Util::MTC_FLOAT_TOL = 1e-6f;

/**
 * Return the sistance from segment ab to point c.
 * If the 
 */
float Util::pointSegmentDistanceXY(const QVector3D &a, const QVector3D &b, const QVector3D &c, bool segmentOnly) {
	float dist;		

	float r_numerator = (c.x()-a.x())*(b.x()-a.x()) + (c.y()-a.y())*(b.y()-a.y());
	float r_denomenator = (b.x()-a.x())*(b.x()-a.x()) + (b.y()-a.y())*(b.y()-a.y());
	float r = r_numerator / r_denomenator;

	if (segmentOnly && (r < 0 || r > 1)) {
		float dist1 = SQR(c.x() - a.x()) + SQR(c.y() - a.y());
		float dist2 = SQR(c.x() - b.x()) + SQR(c.y() - b.y());
		if (dist1 < dist2) {	
			return sqrt(dist1);
		} else {
			return sqrt(dist2);
		}
	} else {
		return abs((a.y()-c.y())*(b.x()-a.x())-(a.x()-c.x())*(b.y()-a.y())) / sqrt(r_denomenator);
	}
}

/**
 * Project latitude/longitude coordinate to world coordinate.
 * Mercator projection cannot be used for this purpose, becuase
 * it deforms the area especially in the high latitude regions.
 * Hubeny's theorum should be used for this purpose, but not yet implemented yet.
 *
 * @param lat		latitude
 * @param lon		longitude
 * @param centerLat	latitude of the center of the map
 * @param centerLon	longitude of the center of the map
 * @return			the world coordinate (Z coordinate is dummy.)
 */
QVector2D Util::projLatLonToMeter(const QVector2D &latLon, const QVector2D &centerLatLon) {
	QVector2D result;

	float y = latLon.y() / 180.0f * M_PI;
	float dx = (latLon.x() - centerLatLon.x()) / 180.0f * M_PI;
	float dy = (latLon.y() - centerLatLon.y()) / 180.0f * M_PI;

	float radius = 6378137.0f;;

	result.setX(radius * cos(y) * dx);
	result.setY(radius * dy);

	return  result; 
}

/*
QVector3D Util::projLatLonToMeter(const QVector3D &latLon, const QVector3D &centerLatLon) {
	QVector3D result;

	float y = latLon.y() / 180.0f * M_PI;
	float dx = (latLon.x() - centerLatLon.x()) / 180.0f * M_PI;
	float dy = (latLon.y() - centerLatLon.y()) / 180.0f * M_PI;

	float radius = 6378137.0f;;

	result.setX(radius * cos(y) * dx);
	result.setY(radius * dy);
	result.setZ(0.0f);

	return  result; 
}
*/

/**
 * Computes the intersection between two line segments on the XY plane.
 * Segments must intersect within their extents for the intersection to be valid. z coordinate is ignored.
 *
 * @param a one end of the first line
 * @param b another end of the first line
 * @param c one end of the second line
 * @param d another end of the second line
 * @param tab
 * @param tcd
 * @param segmentOnly
 * @param intPoint	the intersection
 * @return true if two lines intersect / false otherwise
 **/
bool Util::segmentSegmentIntersectXY(const QVector2D& a, const QVector2D& b, const QVector2D& c, const QVector2D& d, float *tab, float *tcd, bool segmentOnly, QVector2D& intPoint) {
	QVector2D u = b - a;
	QVector2D v = d - c;

	if (u.lengthSquared() < MTC_FLOAT_TOL || v.lengthSquared() < MTC_FLOAT_TOL) {
		return false;
	}

	float numer = v.x()*(c.y()-a.y()) + v.y()*(a.x()-c.x());
	float denom = u.y()*v.x() - u.x()*v.y();

	if (denom == 0.0f)  {
		// they are parallel
		*tab = 0.0f;
		*tcd = 0.0f;
		return false;
	}

	float t0 = numer / denom;

	QVector2D ipt = a + t0*u;
	QVector2D tmp = ipt - c;
	float t1;
	if (QVector2D::dotProduct(tmp, v) > 0.0f){
		t1 = tmp.length() / v.length();
	}
	else {
		t1 = -1.0f * tmp.length() / v.length();
	}

	//Check if intersection is within segments
	if(segmentOnly && !( (t0 >= MTC_FLOAT_TOL) && (t0 <= 1.0f-MTC_FLOAT_TOL) && (t1 >= MTC_FLOAT_TOL) && (t1 <= 1.0f-MTC_FLOAT_TOL) ) ){
		return false;
	}

	*tab = t0;
	*tcd = t1;
	QVector2D dirVec = b-a;

	intPoint = a+(*tab)*dirVec;

	return true;
}

/**
 * Compute the distance between the edge A-B and the edge C-D. Store the coordinate of the closest point in closestPtInAB.
 */
float Util::pointSegmentDistanceXY(const QVector2D& a, const QVector2D& b, const QVector2D& c, QVector2D& closestPtInAB) {
	float dist;		

	float r_numerator = (c.x()-a.x())*(b.x()-a.x()) + (c.y()-a.y())*(b.y()-a.y());
	float r_denomenator = (b.x()-a.x())*(b.x()-a.x()) + (b.y()-a.y())*(b.y()-a.y());
	float r = r_numerator / r_denomenator;
	//
	float px = a.x() + r*(b.x()-a.x());
	float py = a.y() + r*(b.y()-a.y());
	//    
	float s =  ((a.y()-c.y())*(b.x()-a.x())-(a.x()-c.x())*(b.y()-a.y()) ) / r_denomenator;

	float distanceLine = fabs(s)*sqrt(r_denomenator);

	closestPtInAB.setX(px);
	closestPtInAB.setY(py);

	if ((r >= 0) && (r <= 1)) {
		dist = distanceLine;
	} else {
		float dist1 = (c.x()-a.x())*(c.x()-a.x()) + (c.y()-a.y())*(c.y()-a.y());
		float dist2 = (c.x()-b.x())*(c.x()-b.x()) + (c.y()-b.y())*(c.y()-b.y());
		if (dist1 < dist2) {	
			dist = sqrt(dist1);
		} else {
			dist = sqrt(dist2);
		}
	}

	return abs(dist);
}

float Util::rad2deg(float rad) {
	return ((rad*180.0f) / M_PI);
}

/**
 * 角度を正規化し、[-PI , PI]の範囲にする。
 */
float Util::normalizeAngle(float angle) {
	// まずは、正の値に変換する
	if (angle < 0.0f) {
		angle += ((int)(fabs(angle) / M_PI / 2.0f) + 1) * M_PI * 2;
	}

	// 次に、[0, PI * 2]の範囲にする
	angle -= (int)(angle / M_PI / 2.0f) * M_PI * 2;

	// 最後に、[-PI, PI]の範囲にする
	//if (angle > M_PI) angle = M_PI * 2.0f - angle;
	if (angle > M_PI) angle = angle - M_PI * 2.0f;		// fix this bug on 12/17

	return angle;
}

/**
 * Compute the difference in angle that is normalized in the range of [0, PI].
 */
float Util::diffAngle(const QVector2D& dir1, const QVector2D& dir2, bool absolute) {
	float ang1 = atan2f(dir1.y(), dir1.x());
	float ang2 = atan2f(dir2.y(), dir2.x());

	if (absolute) {
		return fabs(normalizeAngle(ang1 - ang2));
	} else {
		return normalizeAngle(ang1 - ang2);
	}
}

/**
 * Compute the difference in angle that is normalized in the range of [0, PI].
 */
float Util::diffAngle(float angle1, float angle2, bool absolute) {
	if (absolute) {
		return fabs(normalizeAngle(angle1 - angle2));
	} else {
		return normalizeAngle(angle1 - angle2);
	}
}

QVector2D Util::getAABB(const std::vector<QVector2D>& ring, QVector2D& minCorner, QVector2D& maxCorner) {
	maxCorner = QVector2D(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	minCorner = QVector2D(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

	QVector2D curPt;

	for (int i = 0; i < ring.size(); ++i) {
		if (ring.at(i).x() > maxCorner.x()) {
			maxCorner.setX(ring[i].x());
		}
		if (ring.at(i).y() > maxCorner.y()) {
			maxCorner.setY(ring[i].y());
		}

		if (ring.at(i).x() < minCorner.x()) {
			minCorner.setX(ring[i].x());
		}
		if (ring.at(i).y() < minCorner.y()) {
			minCorner.setY(ring[i].y());
		}
	}

	return maxCorner - minCorner;
}

void Util::getOBB(std::vector<QVector2D>& ring, QVector2D& size, QMatrix4x4& transMat) {
	float alpha = 0.0f;			
	float deltaAlpha = 0.05 * M_PI;
	float bestAlpha;

	QMatrix4x4 rotMat;
	QVector2D minPt, maxPt;
	QVector2D origMidPt;
	QVector2D boxSz;
	QVector2D bestBoxSz;
	float curArea;
	float minArea = FLT_MAX;

	std::vector<QVector2D> ring_temp = ring;

	getAABB(ring_temp, minPt, maxPt);
	origMidPt = 0.5f * (minPt + maxPt);

	int cSz = ring.size();
	QVector2D difVec;
	for (int i = 0; i < ring.size(); ++i) {
		difVec = (ring.at((i+1) % cSz) - ring.at(i)).normalized();
		alpha = atan2(difVec.x(), difVec.y());
		rotMat.setToIdentity();				
		rotMat.rotate(Util::rad2deg(alpha), 0.0f, 0.0f, 1.0f);				

		transformRing(ring, ring_temp, rotMat);
		
		boxSz = getAABB(ring_temp, minPt, maxPt);
		curArea = boxSz.x() * boxSz.y();
		if (curArea < minArea) {
			minArea = curArea;
			bestAlpha = alpha;
			bestBoxSz = boxSz;
		}
	}

	transMat.setToIdentity();											
	transMat.rotate(Util::rad2deg(bestAlpha), 0.0f, 0.0f, 1.0f);
	transMat.setRow(3, QVector4D(origMidPt.x(), origMidPt.y(), 0.0f, 1.0f));		

	size = bestBoxSz;
}

void Util::transformRing(const std::vector<QVector2D>& srcRing, std::vector<QVector2D>& dstRing, QMatrix4x4& transformMat) {
	dstRing = srcRing;
	for (int i = 0; i < srcRing.size(); ++i) {
		QVector3D pt = transformMat * QVector3D(srcRing[i].x(), srcRing[i].y(), 0.0f);
		dstRing[i].setX(pt.x());
		dstRing[i].setY(pt.y());
	}
}

