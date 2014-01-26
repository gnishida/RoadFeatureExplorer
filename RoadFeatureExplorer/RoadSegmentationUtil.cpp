#include "RoadSegmentationUtil.h"
#include "GraphUtil.h"
#include "Util.h"
#include "BBox.h"
#include <math.h>

#ifndef Q_MOC_RUN
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#endif

#ifndef SQR(x)
#define SQR(x)	((x) * (x))
#endif

RoadGraph* roadGraphPtr;
std::vector<RoadEdgeDesc> plaza;
int numOutingEdges;
float plazaLength;
std::vector<std::vector<RoadEdgeDesc> > plaza_list;

//Vertex visitor
struct faceVisitorForPlazaDetection : public boost::planar_face_traversal_visitor {
	void begin_face() {
		plaza.clear();
		numOutingEdges = 0;
		plazaLength = 0.0f;
	}

	void end_face() {
		if (plaza.size() > 4 && numOutingEdges >= 4 && plazaLength < 1000.0f && plazaLength > 200.0f) {
			plaza_list.push_back(plaza);
		}
	}

	template <typename Vertex> 
	void next_vertex(Vertex v) {
		//plaza.push_back(v);
		numOutingEdges += GraphUtil::getNumEdges(*roadGraphPtr, v) - 2;
	}

	template <typename Edge> 
	void next_edge(Edge e) {
		plaza.push_back(e);
		plazaLength += roadGraphPtr->graph[e]->getLength();
	}
};

/**
 * 指定されたエッジから、同じグループに属するエッジを辿っていく。
 */
int RoadSegmentationUtil::traverseConnectedEdges(RoadGraph& roads, RoadEdgeDesc e, QMap<RoadEdgeDesc, int>& edges, int segment_id) {
	int count = 0;

	QList<RoadVertexDesc> queue;
	RoadVertexDesc src = boost::source(e, roads.graph);
	queue.push_back(src);

	QList<RoadVertexDesc> visited;
	visited.push_back(src);

	while (!queue.empty()) {
		RoadVertexDesc v = queue.front();
		queue.pop_front();

		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(v, roads.graph); ei != eend; ++ei) {
			if (!roads.graph[*ei]->valid) continue;

			// 異なるグループに属するエッジは、辿らない
			if (roads.graph[*ei]->group != roads.graph[e]->group) continue;

			// 当該エッジのセグメントIDを登録する
			edges.insert(*ei, segment_id);

			RoadVertexDesc u = boost::target(*ei, roads.graph);
			if (!roads.graph[u]->valid) continue;
			if (visited.contains(u)) continue;

			visited.push_back(u);
			queue.push_back(u);
			count++;
		}
	}

	return count;
}

/**
 * 最大連結成分に属さないエッジは、group_idを-1に戻す
 */
void RoadSegmentationUtil::reduceGroup(RoadGraph& roads, int group_id) {
	QMap<RoadEdgeDesc, int> edges;

	// 指定されたグループに属するエッジのセットを生成する
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;
		if (roads.graph[*ei]->group == group_id) {
			edges.insert(*ei, -1);
		}
	}

	// エッジと接続されているエッジ群
	int numSegments = 0;
	std::vector<int> hist;
	for (QMap<RoadEdgeDesc, int>::iterator it = edges.begin(); it != edges.end(); ++it) {
		if (edges[it.key()] >= 0) continue;

		int num = traverseConnectedEdges(roads, it.key(), edges, numSegments);
		hist.push_back(num);

		numSegments++;
	}

	// 最大セグメントを取得
	int max_num = 0;
	int max_segment;
	for (int i = 0; i < hist.size(); i++) {
		if (hist[i] > max_num) {
			max_num = hist[i];
			max_segment = i;
		}
	}

	// 最大セグメント以外は、グループIDを-1に戻す
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;
		if (roads.graph[*ei]->group != group_id) continue;

		if (edges[*ei] != max_segment) {
			roads.graph[*ei]->group = -1;
		}
	}
}

/**
 * グリッドを検知する
 */
void RoadSegmentationUtil::detectGrid(RoadGraph& roads) {
	// 全てのエッジのグループIDを0に初期化する
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		roads.graph[*ei]->group = -1;
	}

	for (int i = 0; i < 3; i++) {
		if (!detectOneGrid(roads, 9, 3000, 0.5f)) break;
	}
}

/**
 * １つのグリッドを検知する。
 * 既にグループに属しているエッジはスキップする。
 */
bool RoadSegmentationUtil::detectOneGrid(RoadGraph& roads, int numBins, float minTotalLength, float minMaxBinRatio) {
	// ヒストグラムの初期化
	cv::Mat dirMat = cv::Mat::zeros(numBins, 1, CV_32F);

	int count = 0;
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;
		if (roads.graph[*ei]->group >= 0) continue;

		for (int i = 0; i < roads.graph[*ei]->polyLine.size() - 1; i++) {
			QVector2D dir = roads.graph[*ei]->polyLine[i + 1] - roads.graph[*ei]->polyLine[i];
			float theta = atan2f(dir.y(), dir.x());
			if (theta < 0) theta += M_PI;
			if (theta > M_PI * 0.5f) theta -= M_PI * 0.5f;

			// どのビンか決定
			int bin_id = theta * (float)numBins / M_PI * 2.0f;
			if (bin_id >= numBins) bin_id = numBins - 1;

			// 投票する
			dirMat.at<float>(bin_id, 0) += dir.length();
			count++;
		}
	}

	// ビンの中で、最大値を探す
	float max_hist_value = 0.0f;
	int max_bin_id;
	for (int i = 0; i < dirMat.rows; i++) {
		if (dirMat.at<float>(i, 0) > max_hist_value) {
			max_hist_value = dirMat.at<float>(i, 0);
			max_bin_id = i;
		}
	}

	// 最大頻度のビンの割合が50%未満なら、グリッドとは見なさない
	if (max_hist_value < minTotalLength || max_hist_value < (float)count * minMaxBinRatio) return false;

	// 最大値となるビンと同じビンに入るエッジについて、青色にする
	QVector2D dir1, dir2;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;
		if (roads.graph[*ei]->group >= 0) continue;

		float length = 0.0f;
		for (int i = 0; i < roads.graph[*ei]->polyLine.size() - 1; i++) {
			QVector2D dir = roads.graph[*ei]->polyLine[i + 1] - roads.graph[*ei]->polyLine[i];
			float theta = atan2f(dir.y(), dir.x());
			if (theta < 0) theta += M_PI;
			if (theta > M_PI * 0.5f) theta -= M_PI * 0.5f;

			// どのビンか決定
			int bin_id = theta * (float)numBins / M_PI * 2.0f;
			if (bin_id >= numBins) bin_id = numBins - 1;

			// 同じビンに入るなら、カウントをインクリメント
			if (bin_id == max_bin_id) length += dir.length();
		}

		// 80%以上、グリッドの方向と同じエッジなら、そのエッジを当該グループに入れる
		if (length >= roads.graph[*ei]->getLength() * 0.8f) {
			roads.graph[*ei]->group = max_bin_id;
		}
	}

	// 
	reduceGroup(roads, max_bin_id);

	roads.setModified();

	return true;
}

/**
 * Plazaを検知する
 * グラフのfaceについて、そのサイズ、faceから出るスポークの数などから、プラザかどうか判断する。
 * ぜんぜん検知精度が良くない。
 */
void RoadSegmentationUtil::detectPlaza(RoadGraph& roads) {
	plaza_list.clear();
	roadGraphPtr = &roads;

	//Make sure graph is planar
	typedef std::vector<RoadEdgeDesc > tEdgeDescriptorVector;
	std::vector<tEdgeDescriptorVector> embedding(boost::num_vertices(roads.graph));
	if (!boost::boyer_myrvold_planarity_test(boost::boyer_myrvold_params::graph = roads.graph, boost::boyer_myrvold_params::embedding = &embedding[0])) {
		std::cout << "The road graph is not a planar graph." << std::endl;
	}

	//Create edge index property map
	typedef std::map<RoadEdgeDesc, size_t> EdgeIndexMap;
	EdgeIndexMap mapEdgeIdx;
	boost::associative_property_map<EdgeIndexMap> pmEdgeIndex(mapEdgeIdx);		
	RoadEdgeIter ei, eend;	
	int edge_count = 0;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		mapEdgeIdx.insert(std::make_pair(*ei, edge_count++));
	}

	//Extract blocks from road graph using boost graph planar_face_traversal
	faceVisitorForPlazaDetection vis;	
	boost::planar_face_traversal(roads.graph, &embedding[0], vis, pmEdgeIndex);

	for (int i = 0; i < plaza_list.size(); i++) {
		for (int j = 0; j < plaza_list[i].size(); j++) {
			RoadEdgeDesc e = plaza_list[i][j];

			//roads.graph[e]->color = QColor(0, 255, 0);
			roads.graph[e]->group = 8;
		}
	}

	roads.setModified();
}

/**
 * Plazaを検知する
 * Hough transformにより、円を検知する。
 */
void RoadSegmentationUtil::detectRadial(RoadGraph& roads) {
	// 0.01スケールで、円の中心を求める
	QVector2D center = detectOneRadial(roads, 0.01f);

	// 0.1スケールで、より正確な円の中心を求める
	center = detectOneRadial(roads, 0.1f, center, 200.0f, 0.4f);

	// 0.2スケールで、より正確な円の中心を求める
	//center = detectOneRadial(roads, 1.0f, center, 100.0f, 0.2f);

	// 各エッジについて、radialの中心点に合うもののグループIDを1にする
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		float length = 0.0f;
		for (int i = 0; i < roads.graph[*ei]->polyLine.size() - 1; i++) {
			QVector2D dir1 = roads.graph[*ei]->polyLine[i + 1] - roads.graph[*ei]->polyLine[i];
			QVector2D dir2 = roads.graph[*ei]->polyLine[i] - center;

			if (GraphUtil::diffAngle(dir1, dir2) < 0.5f || GraphUtil::diffAngle(dir1, -dir2) < 0.5f) {
				length += dir1.length();
			}
		}

		// 50%以上、このradialと同じ方向なら、そのエッジを当該グループに入れる
		if (length >= roads.graph[*ei]->getLength() * 0.5f) {
			roads.graph[*ei]->group = 1;
		}
	}

	// 円の中心から一定距離以内のエッジをシードとし、シードと連結されていないエッジのグループを-1に戻す
	reduceRadialGroup(roads, center, 1, 200.0f);

	roads.setModified();
}

/**
 * Plazaを１つ検知し、円の中心を返却する
 * Hough transformにより、円を検知する。
 */
QVector2D RoadSegmentationUtil::detectOneRadial(RoadGraph& roads, float scale) {
	BBox bbox = GraphUtil::getAABoundingBox(roads);
	cv::Mat houghTransform = cv::Mat::zeros(bbox.dy() * scale, bbox.dx() * scale, CV_32F);

	float sigma = bbox.dx() * scale * 0.05f;

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		for (int i = 0; i < roads.graph[*ei]->polyLine.size() - 1; i++) {
			QVector2D v1 = (roads.graph[*ei]->polyLine[i] - bbox.minPt) * scale;
			QVector2D v2 = (roads.graph[*ei]->polyLine[i + 1] - bbox.minPt) * scale;
			QVector2D v12 = (v1 + v2) * 0.5f;

			QVector2D dir = v2 - v1;
			float len = dir.length();

			// 長さが0のエッジはスキップ
			if (dir.x() < 0.000001f && dir.y() < 0.000001f) continue;

			if (dir.x() > dir.y()) {
				for (int x = 0; x < houghTransform.cols; x++) {
					int y = dir.y() * ((float)x - v1.x()) / dir.x() + v1.y() + 0.5f;
					if (y < 0 || y >= houghTransform.rows) continue;

					float h = 0;
					if (x >= std::min(v1.x(), v2.x()) && x <= std::max(v1.x(), v2.x())) {
						h = len;
					} else if (x < std::min(v1.x(), v2.x())) {
						h = len * expf(-SQR(x - std::min(v1.x(), v2.x())) / 2.0f / SQR(sigma));
					} else {
						h = len * expf(-SQR(x - std::max(v1.x(), v2.x())) / 2.0f / SQR(sigma));
					}

					houghTransform.at<float>(y, x) += h;
				}
			} else {
				for (int y = 0; y < houghTransform.rows; y++) {
					int x = dir.x() * ((float)y - v1.y()) / dir.y() + v1.x() + 0.5f;
					if (x < 0 || x >= houghTransform.cols) continue;

					float h = 0;
					if (y >= std::min(v1.y(), v2.y()) && y <= std::max(v1.y(), v2.y())) {
						h = len;
					} else if (y < std::min(v1.y(), v2.y())) {
						h = len * expf(-SQR(y - std::min(v1.y(), v2.y())) / 2.0f / SQR(sigma));
					} else {
						h = len * expf(-SQR(y - std::max(v1.y(), v2.y())) / 2.0f / SQR(sigma));
					}

					houghTransform.at<float>(y, x) += h;
				}
			}
		}
	}

	// 最大値を取得する
	QVector2D center;
	float max_value = 0.0f;
	for (int v = 0; v < houghTransform.rows; v++) {
		for (int u = 0; u < houghTransform.cols; u++) {
			if (houghTransform.at<float>(v, u) > max_value) {
				max_value = houghTransform.at<float>(v, u);
				center.setX(u + 0.5f);
				center.setY(v + 0.5f);
			}
		}
	}

	// radial の中心点を求める
	center /= scale;
	center += bbox.minPt;

	return center;
}

/**
 * 円のだいたいの中心点を使って、より正確な円の中心を返却する
 * Hough transformにより、円を検知する。
 */
QVector2D RoadSegmentationUtil::detectOneRadial(RoadGraph& roads, float scale, QVector2D& centerApprox, float distanceThreshold, float angleThreshold) {
	BBox bbox = GraphUtil::getAABoundingBox(roads);
	cv::Mat houghTransform = cv::Mat::zeros(bbox.dy() * scale, bbox.dx() * scale, CV_32F);

	float sigma = bbox.dx() * scale * 0.05f;

	QVector2D c = (centerApprox - bbox.minPt) * scale;

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);
		if (src == 68 || tgt == 68) {
			int k = 0;
		}

		for (int i = 0; i < roads.graph[*ei]->polyLine.size() - 1; i++) {
			QVector2D v1 = (roads.graph[*ei]->polyLine[i] - bbox.minPt) * scale;
			QVector2D v2 = (roads.graph[*ei]->polyLine[i + 1] - bbox.minPt) * scale;

			// だいたいの中心点から離れすぎたエッジは、スキップする
			if (Util::pointSegmentDistanceXY(v1, v2, c, false) > distanceThreshold) continue;

			QVector2D dir = v2 - v1;

			// だいたいの中心点への方向が大きくずれている場合は、スキップする
			if (GraphUtil::diffAngle(v1 - c, dir) > angleThreshold && GraphUtil::diffAngle(v1 - c, -dir) > angleThreshold) continue;
			
			float len = dir.length();

			// 長さが0のエッジはスキップ
			if (dir.x() < 0.000001f && dir.y() < 0.000001f) continue;

			if (dir.x() > dir.y()) {
				for (int x = 0; x < houghTransform.cols; x++) {
					int y = dir.y() * ((float)x - v1.x()) / dir.x() + v1.y() + 0.5f;
					if (y < 0 || y >= houghTransform.rows) continue;

					float h = 0;
					if (x >= std::min(v1.x(), v2.x()) && x <= std::max(v1.x(), v2.x())) {
						h = len;
					} else if (x < std::min(v1.x(), v2.x())) {
						h = len * expf(-SQR(x - std::min(v1.x(), v2.x())) / 2.0f / SQR(sigma));
					} else {
						h = len * expf(-SQR(x - std::max(v1.x(), v2.x())) / 2.0f / SQR(sigma));
					}

					houghTransform.at<float>(y, x) += h;
				}
			} else {
				for (int y = 0; y < houghTransform.rows; y++) {
					int x = dir.x() * ((float)y - v1.y()) / dir.y() + v1.x() + 0.5f;
					if (x < 0 || x >= houghTransform.cols) continue;

					float h = 0;
					if (y >= std::min(v1.y(), v2.y()) && y <= std::max(v1.y(), v2.y())) {
						h = len;
					} else if (y < std::min(v1.y(), v2.y())) {
						h = len * expf(-SQR(y - std::min(v1.y(), v2.y())) / 2.0f / SQR(sigma));
					} else {
						h = len * expf(-SQR(y - std::max(v1.y(), v2.y())) / 2.0f / SQR(sigma));
					}

					houghTransform.at<float>(y, x) += h;
				}
			}
		}
	}

	/*
	cv::flip(houghTransform, houghTransform, 0);
	cv::namedWindow("drawing", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::imshow("drawing", houghTransform);

	cv::imwrite("result.jpg", houghTransform);
	*/

	// 最大値を取得する
	QVector2D center;
	float max_value = 0.0f;
	for (int v = 0; v < houghTransform.rows; v++) {
		for (int u = 0; u < houghTransform.cols; u++) {
			if (houghTransform.at<float>(v, u) > max_value) {
				max_value = houghTransform.at<float>(v, u);
				center.setX(u + 0.5f);
				center.setY(v + 0.5f);
			}
		}
	}

	// radial の中心点を求める
	center /= scale;
	center += bbox.minPt;

	return center;
}

/**
 * 円の中心からdistanceThreshold以内のRadialグループをシードとし、
 * シードと連結されていないエッジのradial groupを-1い戻す。
 */
void RoadSegmentationUtil::reduceRadialGroup(RoadGraph& roads, QVector2D& center, int group_id, float distanceThreshold) {
	float distanceThreshold2 = distanceThreshold * distanceThreshold;

	QList<RoadVertexDesc> queue;
	//RoadVertexDesc src = boost::source(e, roads.graph);
	//queue.push_back(src);

	QList<RoadVertexDesc> visited;
	//visited.push_back(src);

	QMap<RoadEdgeDesc, bool> edges;

	// シードの構築
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;
		if (roads.graph[*ei]->group != group_id) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		// 円の中心から離れているエッジは、シードにしない
		if ((roads.graph[src]->pt - center).lengthSquared() > distanceThreshold2 && (roads.graph[tgt]->pt - center).lengthSquared() > distanceThreshold2) continue;

		queue.push_back(src);
		queue.push_back(tgt);
		visited.push_back(src);
		visited.push_back(tgt);

		edges.insert(*ei, true);
	}

	//
	while (!queue.empty()) {
		RoadVertexDesc v = queue.front();
		queue.pop_front();

		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(v, roads.graph); ei != eend; ++ei) {
			if (!roads.graph[*ei]->valid) continue;

			// 異なるグループに属するエッジは、辿らない
			if (roads.graph[*ei]->group != group_id) continue;

			// 当該エッジを登録する
			edges.insert(*ei, true);

			RoadVertexDesc u = boost::target(*ei, roads.graph);
			if (!roads.graph[u]->valid) continue;
			if (visited.contains(u)) continue;

			visited.push_back(u);
			queue.push_back(u);
		}
	}

	// 登録されたエッジ以外は、group_idを-1に戻す
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;
		if (roads.graph[*ei]->group != group_id) continue;

		roads.graph[*ei]->group = -1;
	}
	for (QMap<RoadEdgeDesc, bool>::iterator it = edges.begin(); it != edges.end(); ++it) {
		roads.graph[it.key()]->group = group_id;
	}
}
