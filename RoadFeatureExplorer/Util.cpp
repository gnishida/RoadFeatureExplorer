#include "Util.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define SQR(x)		((x) * (x))

const float Util::MTC_FLOAT_TOL = 1e-6f;

/**
 * Return the sistance from segment ab to point c.
 * If the 
 */
float Util::pointSegmentDistanceXY(const QVector3D &a, const QVector3D &b, const QVector3D &c) {
	float dist;		

	float r_numerator = (c.x()-a.x())*(b.x()-a.x()) + (c.y()-a.y())*(b.y()-a.y());
	float r_denomenator = (b.x()-a.x())*(b.x()-a.x()) + (b.y()-a.y())*(b.y()-a.y());
	float r = r_numerator / r_denomenator;

	if (r >= 0 && r <= 1) {
		return abs((a.y()-c.y())*(b.x()-a.x())-(a.x()-c.x())*(b.y()-a.y())) / sqrt(r_denomenator);
	} else {
		float dist1 = SQR(c.x() - a.x()) + SQR(c.y() - a.y());
		float dist2 = SQR(c.x() - b.x()) + SQR(c.y() - b.y());
		if (dist1 < dist2) {	
			return sqrt(dist1);
		} else {
			return sqrt(dist2);
		}
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
