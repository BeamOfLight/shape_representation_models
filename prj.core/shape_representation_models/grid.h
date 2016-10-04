/**
    https://github.com/BeamOfLight/shape_representation_models.git
    grid.h

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04
*/

#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_GRID_H
#define SHAPE_REPRESENTATION_MODELS_GRID_H

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cmath>

#include <shape_representation_models/abstract_area_model.h>

namespace ShapeRepresentationModels {
	class Grid : public AbstractAreaModel
	{
	  private:
		size_t pointRepresentationSize;
	  public:
		struct Representation : public AbstractRepresentation
		{
			cv::Mat mask;
			cv::Rect rect;
		};

		Grid(size_t pointRepresentationSize);
		std::string getMethodName();

		int getObjectRepresentationSize(AbstractRepresentation* encodedObject);
		AbstractRepresentation* encodeSingleObject(const cv::Mat &object);
		cv::Mat decodeSingleObject(AbstractRepresentation* encodedObject);
	};
}

#endif
