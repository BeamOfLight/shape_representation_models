#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_CENTERED_POINTS_H
#define SHAPE_REPRESENTATION_MODELS_CENTERED_POINTS_H

#include <shape_representation_models/abstract_contour_model.h>

namespace ShapeRepresentationModels {
	class CenteredPoints : public AbstractContourModel
	{
	  protected:
		AbstractContourRepresentation* getNewContourRepresentation();
	  public:
		struct ContourRepresentation : public AbstractContourRepresentation
		{
			cv::Point center;
			std::vector < cv::Point > normalizedContour;
		
			std::vector < cv::Point > convert2Points();
			void initFromPoints(const std::vector < cv::Point > &points);
		};  

		CenteredPoints(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize);
		std::string getMethodName();
		int getObjectRepresentationSize(AbstractRepresentation* encodedObject);
	};
}

#endif