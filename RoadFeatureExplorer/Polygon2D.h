/*********************************************************************
This file is part of QtUrban.

    QtUrban is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    QtUrban is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QtUrban.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#pragma once

#include <vector>
#include <QVector2D>
#include <QVector3D>
#include "float.h"
#define _USE_MATH_DEFINES
#include <math.h>

class Loop2D : public std::vector<QVector2D> {
public:
	void close();
	void tessellate(std::vector<Loop2D>& trapezoids) const;
	bool contains(const QVector2D& pt) const;
	void findEdge(float x, float y, int& v1, int& v2, float& s) const;
	static Loop2D createRectangle(float width, float height);
};

class Polygon2D {
protected:
	/** Vector containing 3D points of polygon contour */
	Loop2D contour;

	QVector2D normalVec;
	QVector2D centroid;

	bool isNormalVecValid;
	bool isCentroidValid;

public:
	Polygon2D();
	~Polygon2D();

	/**
	 * Copy constructor.
	 */
	inline Polygon2D::Polygon2D(const Polygon2D &ref) {	
		contour = ref.contour;
		normalVec = ref.normalVec;
		centroid = ref.centroid;
	}

	/**
	 * Assignment operator.
	 */
	inline Polygon2D& operator=(const Polygon2D &ref) {				
		contour = ref.contour;
		normalVec = ref.normalVec;
		centroid = ref.centroid;
		return (*this);
	}

	/** Acessor to point at index idx */
	inline QVector2D& operator[](const int idx) {	
		return contour[idx];
	}

	/** getter/setter for contour */
	Loop2D& getContour() { return contour; }
	void setContour(const Loop2D& contour);

	void clear();
	void push_back(const QVector2D& point);
	int size() const;

	/** Get normal vector */
	//QVector3D& getNormalVector();

	/** Get center of vertices */
	QVector2D& getCentroid();

	/*inline float getMeanZValue() const {
		float zVal = 0.0f;
		if(this->contour.size() > 0){
			for(size_t i=0; i<contour.size(); ++i){
				zVal += (contour[i].z());
			}
			return (zVal/((float)contour.size()));
		} else {
			return zVal;
		}
	}*/

	/** Render polygon */
	//void renderContour();
	//void render();
	//void renderNonConvex(bool reComputeNormal = true, float nx = 0.0f, float ny = 0.0f, float nz = 1.0f);

	/** Is self intersecting */
	//bool isSelfIntersecting();
	//static bool isSelfIntersecting(const Loop2D& contour);

	/** Only works for polygons with no holes in them */
	//bool splitMeWithPolyline(const std::vector<QVector2D>& pline, Loop2D &pgon1, Loop2D &pgon2);
	//bool splitMeWithPolyline2(const std::vector<QVector2D>& pline, Loop2D &pgon1, Loop2D &pgon2);

	/** Only works for polygons with no holes in them */
	//float computeInset(float offsetDistance, Loop2D &pgonInset, bool computeArea = true);
	//float computeInset(std::vector<float> offsetDistances, Loop2D &pgonInset, bool computeArea = true);
	//float computeArea(bool parallelToXY = false);
	bool reorientFace(bool onlyCheck = false);

	//static QVector3D getLoopNormalVector(const Loop2D &pin);
	static bool reorientFace(Loop2D& pface, bool onlyCheck = false);
	//static int cleanLoop(const Loop2D& pin, Loop2D &pout, float threshold);
	static void transformLoop(const Loop2D& pin, Loop2D& pout, QMatrix4x4& transformMat);
	//static float computeLoopArea(const Loop2D& pin, bool parallelToXY = false);
	//static void sampleTriangularLoopInterior(const Loop2D& pin, std::vector<QVector3D>& pts, float density);
	static QVector2D getLoopAABB(const Loop2D& pin, QVector2D& minCorner, QVector2D& maxCorner);
	static void getLoopOBB(const Loop2D& pin, QVector2D& size, QMatrix4x4& xformMat);
	//void getMyOBB(QVector3D& size, QMatrix4x4& xformMat);
	//static void extrudePolygon(const Loop2D& basePgon, float height, std::vector<Polygon2D>& pgonExtrusion);

	/** Shortest distance from a point to a polygon */
	static float distanceXYToPoint(const Loop2D& pin, const QVector3D& pt);

	/** minimum distance from a loop to another loop (this considers the contour only) */
	static float distanceXYfromContourAVerticesToContourB(const Loop2D& pA, const Loop2D& pB);
};

