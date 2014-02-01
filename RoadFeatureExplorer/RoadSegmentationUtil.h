#pragma once

#include "RoadGraph.h"
#include "AbstractArea.h"
#include "GridFeature.h"
#include "RadialFeature.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>

class RoadSegmentationUtil {
protected:
	RoadSegmentationUtil() {}
	~RoadSegmentationUtil() {}

public:
	static void detectGrid(RoadGraph& roads, const Polygon2D& area, int roadType, std::vector<GridFeature>& gridFeatures, int maxIteration, float numBins, float minTotalLength, float minMaxBinRatio, float angleThreshold, float votingRatioThreshold, float extendingDistanceThreshold, float minObbLength);
	static bool detectOneGrid(RoadGraph& roads, const Polygon2D& area, int roadType, GridFeature& gf, int numBins, float minTotalLength, float minMaxBinRatio, float angleThreshold, float votingRatioThreshold, float extendingDistanceThreshold, float minObbLength);
	static int traverseConnectedEdges(RoadGraph& roads, RoadEdgeDesc e, QMap<RoadEdgeDesc, int>& edges, int segment_id);
	static void reduceGridGroup(RoadGraph& roads, GridFeature& gf, QMap<RoadEdgeDesc, float>& edges);
	static void extendGridGroup(RoadGraph& roads, const Polygon2D& area, int roadType, GridFeature& gf, QMap<RoadEdgeDesc, float>& edges, float angleThreshold, float votingRatioThreshold, float distanceThreshold);

	//static void detectPlaza(RoadGraph& roads, AbstractArea& area);

	static void detectRadial(RoadGraph& roads, const Polygon2D& area, int roadType, int maxIteration, float scale1, float scale2, float centerErrorTol2, float angleThreshold2, float scale3, float centerErrorTol3, float angleThreshold3, float votingRatioThreshold, float seedDistance, float minSeedDirections, float extendingAngleThreshold);
	static bool detectOneRadial(RoadGraph& roads, const Polygon2D& area, int roadType, RadialFeature& rf, float scale1, float scale2, float centerErrorTol2, float angleThreshold2, float scale3, float centerErrorTol3, float angleThreshold3, float votingRatioThreshold, float seedDistance, float minSeedDirections, float extendingAngleThreshold);
	static void detectRadialCenterInScaled(RoadGraph& roads, const Polygon2D& area, int roadType, float scale, RadialFeature& rf);
	static void refineRadialCenterInScaled(RoadGraph& roads, const Polygon2D& area, int roadType, float scale, RadialFeature& rf, float distanceThreshold, float angleThreshold);
	static void reduceRadialGroup(RoadGraph& roads, RadialFeature& rf, QMap<RoadEdgeDesc, bool>& edges, float distanceThreshold);
	static void extendRadialGroup(RoadGraph& roads, const Polygon2D& area, int roadType, RadialFeature& rf, QMap<RoadEdgeDesc, bool>& edges, float angleThreshold, float dirCheckRatio);
	static int countNumDirections(RoadGraph& roads, const RadialFeature& rf, QMap<RoadEdgeDesc, bool>& edges, int size);

	static void detectRoundabout(RoadGraph& roads, AbstractArea& area);
};

