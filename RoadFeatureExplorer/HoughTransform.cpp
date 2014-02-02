#include "HoughTransform.h"

#define SQR(x)		((x) * (x))

HoughTransform::HoughTransform(const Polygon2D& area, float scale) {
	this->area = area;
	this->scale = scale;

	bbox = area.getLoopAABB();
	htSpace = cv::Mat::zeros(bbox.dy() * scale, bbox.dx() * scale, CV_32F);

	std::cout << "HoughTransform: (" << htSpace.rows << " x " << htSpace.cols << ")" << std::endl;
}

/**
 * オリジナルの座標系での頂点座標v1とv2を通る直線を、縮尺された座標系のhtSpace上に描画する。
 */
void HoughTransform::line(const QVector2D& v1, const QVector2D& v2) {
	QVector2D vv1 = (v1 - bbox.minPt) * scale;
	QVector2D vv2 = (v2 - bbox.minPt) * scale;

	QVector2D dir = vv2 - vv1;
	float len = dir.length();

	float sigma = bbox.dx() * scale * 0.05f;

	if (dir.x() > dir.y()) {
		for (int x = 0; x < htSpace.cols; x++) {
			int y = dir.y() * ((float)x - v1.x()) / dir.x() + v1.y() + 0.5f;
			if (y < 0 || y >= htSpace.rows) continue;

			float h = 0;
			if (x >= std::min(v1.x(), v2.x()) && x <= std::max(v1.x(), v2.x())) {
				h = len;
			} else if (x < std::min(v1.x(), v2.x())) {
				h = len * expf(-SQR(x - std::min(v1.x(), v2.x())) / 2.0f / SQR(sigma));
			} else {
				h = len * expf(-SQR(x - std::max(v1.x(), v2.x())) / 2.0f / SQR(sigma));
			}

			htSpace.at<float>(y, x) += h;
		}
	} else {
		for (int y = 0; y < htSpace.rows; y++) {
			int x = dir.x() * ((float)y - v1.y()) / dir.y() + v1.x() + 0.5f;
			if (x < 0 || x >= htSpace.cols) continue;

			float h = 0;
			if (y >= std::min(v1.y(), v2.y()) && y <= std::max(v1.y(), v2.y())) {
				h = len;
			} else if (y < std::min(v1.y(), v2.y())) {
				h = len * expf(-SQR(y - std::min(v1.y(), v2.y())) / 2.0f / SQR(sigma));
			} else {
				h = len * expf(-SQR(y - std::max(v1.y(), v2.y())) / 2.0f / SQR(sigma));
			}

			htSpace.at<float>(y, x) += h;
		}
	}
}

QVector2D HoughTransform::maxPoint() const {
	QVector2D ret;

	float max_value = 0.0f;

	for (int v = 0; v < htSpace.rows; v++) {
		for (int u = 0; u < htSpace.cols; u++) {
			if (htSpace.at<float>(v, u) > max_value) {
				max_value = htSpace.at<float>(v, u);
				ret.setX(u + 0.5f);
				ret.setY(v + 0.5f);
			}
		}
	}

	// 投票結果を画像として保存する
	cv::Mat m;
	cv::flip(htSpace, m, 0);
	m /= (max_value / 255.0f);
	m.convertTo(m, CV_8U);
	cv::imwrite(QString("result%1.jpg").arg(scale).toUtf8().data(), m);


	ret /= scale;
	ret += bbox.minPt;

	return ret;
}
