/**
    https://github.com/BeamOfLight/shape_representation_models.git
    polygon_model.h

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04
*/

#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_POLYGON_MODEL_H
#define SHAPE_REPRESENTATION_MODELS_POLYGON_MODEL_H

#include <shape_representation_models/abstract_contour_model.h>

namespace ShapeRepresentationModels {
	class PolygonModel : public AbstractContourModel
	{
	  protected:
		double epsilon;

		AbstractContourRepresentation* getNewContourRepresentation();
	  public:
		struct ContourRepresentation : public AbstractContourRepresentation
		{
			std::vector < cv::Point > vertices;
			double epsilon;

			ContourRepresentation(double epsilon);
			std::vector < cv::Point > convert2Points();
			void initFromPoints(const std::vector < cv::Point > &points);
		};

		PolygonModel(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize, double epsilon);
		std::string getMethodName();
		int getObjectRepresentationSize(AbstractRepresentation* encodedObject);
	};
}

#endif
