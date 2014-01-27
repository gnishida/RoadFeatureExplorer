#pragma once

#include "RoadGraph.h"
#include "AbstractArea.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>

class RoadSegmentationUtil {
protected:
	RoadSegmentationUtil() {}
	~RoadSegmentationUtil() {}

public:
	static void detectGrid(RoadGraph& roads, AbstractArea& area, float numBins, float minTotalLength, float minMaxBinRatio, float votingRatioThreshold);
	static bool detectOneGrid(RoadGraph& roads, AbstractArea& area, int numBins, float minTotalLength, float minMaxBinRatio, float votingRatioThreshold);
	static int traverseConnectedEdges(RoadGraph& roads, RoadEdgeDesc e, QMap<RoadEdgeDesc, int>& edges, int segment_id);
	static void reduceGroup(RoadGraph& roads, int group_id);

	static void detectPlaza(RoadGraph& roads, AbstractArea& area);

	static void detectRadial(RoadGraph& roads, AbstractArea& area, float scale1, float scale2, float centerErrorTol2, float angleThreshold2, float scale3, float centerErrorTol3, float angleThreshold3, float votingRatioThreshold, float seedDistance, float extendingAngleThreshold);
	static QVector2D detectOneRadial(RoadGraph& roads, AbstractArea& area, float scale);
	static QVector2D detectOneRadial(RoadGraph& roads, AbstractArea& area, float scale, QVector2D& centerApprox, float distanceThreshold, float angleThreshold);
	static void reduceRadialGroup(RoadGraph& roads, QVector2D& center, int group_id, float distanceThreshold);
	static void extendRadialGroup(RoadGraph& roads, AbstractArea& area, QVector2D& center, int group_id, float angleThreshold, float dirCheckRatio);
};

