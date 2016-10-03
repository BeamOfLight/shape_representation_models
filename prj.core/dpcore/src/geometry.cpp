#include <dpcore/geometry.h>

double DpCore::Geometry::distance(const cv::Point &pt1, const cv::Point &pt2)
{
	return sqrt(pow(pt2.x - pt1.x, 2) + pow(pt2.y - pt1.y, 2));
}

double DpCore::Geometry::distance(const cv::Point2d &pt1, const cv::Point2d &pt2)
{
	return sqrt(pow(pt2.x - pt1.x, 2) + pow(pt2.y - pt1.y, 2));
}

double DpCore::Geometry::length(const cv::Point &pt)
{
	return distance(static_cast < cv::Point2d > (pt), cv::Point2d(0, 0));
}

double DpCore::Geometry::angleBetweenVectors(const cv::Point2d &pt1, const cv::Point2d &pt2)
{
	return std::acos((pt1.x * pt2.x + pt1.y * pt2.y) / (length(pt1) * length(pt2)));
}

double DpCore::Geometry::angle4ThreePoints(const cv::Point &pt1, const cv::Point &pt2, const cv::Point &pt3)
{
	cv::Point2d vec1 = cv::Point2d(pt2.x - pt1.x, pt2.y - pt1.y);
	cv::Point2d vec2 = cv::Point2d(pt3.x - pt2.x, pt3.y - pt2.y);
	double sign = vec1.x * vec2.y - vec1.y * vec2.x > 0 ? 1.0 : -1.0;

	return sign * angleBetweenVectors(vec1, vec2);
}

cv::Point DpCore::Geometry::roundPoint(const cv::Point2d &pt)
{
	return cv::Point(
		DpCore::Common::round(pt.x),
		DpCore::Common::round(pt.y)
	);
}

cv::Point2d DpCore::Geometry::centerPoint(const cv::Point2d &pt1, const cv::Point2d &pt2)
{
	return cv::Point2d(
		(pt1.x + pt2.x)  / 2,
		(pt1.y + pt2.y)  / 2
	);
}