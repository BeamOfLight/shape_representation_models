#pragma once
#ifndef DPCORE_GEOMETRY_H
#define DPCORE_GEOMETRY_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <string>
#include <iostream>
#include <map>
#include <cmath>

#include <dpcore/common.h>

namespace DpCore
{
	namespace Geometry
	{
		double distance(const cv::Point &pt1, const cv::Point &pt2);
		double distance(const cv::Point2d &pt1, const cv::Point2d &pt2);
		double length(const cv::Point &pt);
		double angleBetweenVectors(const cv::Point2d &pt1, const cv::Point2d &pt2);
		double angle4ThreePoints(const cv::Point &pt1, const cv::Point &pt2, const cv::Point &pt3);
		cv::Point roundPoint(const cv::Point2d &pt);
		cv::Point2d centerPoint(const cv::Point2d &pt1, const cv::Point2d &pt2);
	}
}

#endif
