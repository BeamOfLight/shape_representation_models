#pragma once
#ifndef DPCORE_OBJECT_H
#define DPCORE_OBJECT_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <string>
#include <iostream>
#include <map>

#include <dpcore/common.h>

namespace DpCore
{
  template <typename T>
	struct ObjectRepresentation
	{
		T outterContour;
		std::vector < T > innerContours;
	};

  class Object
  {
    public:
	  // Конвертирует силуэт объекта в контурный дескриптор объекта
	  static ObjectRepresentation < std::vector < cv::Point > > convertObject2PointRepresentation(const cv::Mat &object, size_t minContourSize = 0);

	  // Восстанавливает контурный дескриптор объекта в силуэт объекта
	  static cv::Mat convertPointRepresentation2Object(ObjectRepresentation < std::vector < cv::Point > > pointRepresentation);

	  // Сравнение двух объектов через площадь области разности
	  static float compareObjects(const cv::Mat &object1, const cv::Mat &object2);
  };
}

#endif
