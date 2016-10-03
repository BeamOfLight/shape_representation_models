#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_RUN_LENGTH_ENCODING_H
#define SHAPE_REPRESENTATION_MODELS_RUN_LENGTH_ENCODING_H

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <shape_representation_models/abstract_area_model.h>

namespace ShapeRepresentationModels {
	class RunLengthEncoding : public AbstractAreaModel
	{
	  private:
		size_t pointRepresentationSize;

	  public:
		struct Representation : public AbstractRepresentation
		{
			cv::Rect rect;
			bool firstBit;
			bool verticalDirection;
			std::vector < int > runLengthParameters;
		};

		RunLengthEncoding(size_t pointRepresentationSize);
		std::string getMethodName();
	
		int getObjectRepresentationSize(AbstractRepresentation* encodedObject);
		AbstractRepresentation* encodeSingleObject(const cv::Mat &object);
		cv::Mat decodeSingleObject(AbstractRepresentation* encodedObject);	
	};
}

#endif