/**
	https://github.com/BeamOfLight/shape_representation_models.git
    points.h

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04 
*/

#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_POINTS_H
#define SHAPE_REPRESENTATION_MODELS_POINTS_H

#include <shape_representation_models/abstract_contour_model.h>

namespace ShapeRepresentationModels {
	class Points : public AbstractContourModel
	{
	  protected:
		AbstractContourRepresentation* getNewContourRepresentation();
	  public:
		struct ContourRepresentation : public AbstractContourRepresentation
		{
			std::vector < cv::Point > points;
		
			std::vector < cv::Point > convert2Points();
			void initFromPoints(std::vector < cv::Point > const &points);
		};  

		Points(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize);
		std::string getMethodName();
		int getObjectRepresentationSize(AbstractRepresentation* encodedObject);
	};
}

#endif