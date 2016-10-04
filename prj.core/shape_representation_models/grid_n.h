/**
    https://github.com/BeamOfLight/shape_representation_models.git
    grid_n.h

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04
*/

#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_GRID_N_H
#define SHAPE_REPRESENTATION_MODELS_GRID_N_H

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cmath>

#include <shape_representation_models/abstract_area_model.h>

namespace ShapeRepresentationModels {
	class GridN : public AbstractAreaModel
	{
	  private:
		size_t step;
		size_t pointRepresentationSize;

		unsigned char getNPointValue(const cv::Mat &object, cv::Point pt);
		void setNPointValue(cv::Mat &object, cv::Point pt, unsigned char value);
	  public:
		struct Representation : public AbstractRepresentation
		{
			// Маска силуэта с размерами rect.width/step и rect.height/step
			cv::Mat mask;

			//кодируемая область на исходном изображении
			cv::Rect rect;
			size_t step;
		};

		GridN(size_t pointRepresentationSize, size_t step);
		std::string getMethodName();

		int getObjectRepresentationSize(AbstractRepresentation* encodedObject);
		AbstractRepresentation* encodeSingleObject(const cv::Mat &object);
		cv::Mat decodeSingleObject(AbstractRepresentation* encodedObject);
	};
}

#endif
