#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_ABSTRACT_AREA_MODEL_H
#define SHAPE_REPRESENTATION_MODELS_ABSTRACT_AREA_MODEL_H

#include <shape_representation_models/abstract_model.h>

namespace ShapeRepresentationModels {
	class AbstractAreaModel: public AbstractModel
	{
	  protected:
		std::vector < cv::Point > getImageContour(const cv::Mat &image);	

	  public:
		int getMethodType() const;
	};
}

#endif