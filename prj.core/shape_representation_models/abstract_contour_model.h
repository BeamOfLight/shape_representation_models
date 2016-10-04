/**
	https://github.com/BeamOfLight/shape_representation_models.git
    abstract_contour_model.h

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04 
*/

#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_ABSTRACT_CONTOUR_MODEL_H
#define SHAPE_REPRESENTATION_MODELS_ABSTRACT_CONTOUR_MODEL_H

#include <dpcore/object.h>
#include <shape_representation_models/abstract_model.h>

namespace ShapeRepresentationModels {
	class AbstractContourModel: public AbstractModel
	{
	  public:
		struct AbstractContourRepresentation
		{
			virtual void initFromPoints(const std::vector < cv::Point > &points) = 0;
			virtual std::vector < cv::Point > convert2Points() = 0;
		};
	
		struct Representation : public AbstractRepresentation
		{
			DpCore::ObjectRepresentation < AbstractContourRepresentation* > data;
		};

		AbstractContourModel(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize);
		int getMethodType() const;
	
	  protected:
		size_t pointRepresentationSize;
		size_t pointsCountSize;
		size_t contoursCountSize;
	
		DpCore::ObjectRepresentation < AbstractContourRepresentation* > convertPointRepresentation2EncodedObject(DpCore::ObjectRepresentation < std::vector < cv::Point > > const &pointDescriptor);
		DpCore::ObjectRepresentation < std::vector < cv::Point > > convertEncodedObject2PointRepresentation(DpCore::ObjectRepresentation < AbstractContourRepresentation* > encodedObject);
	
		AbstractRepresentation* encodeSingleObject(const cv::Mat &object);
		cv::Mat decodeSingleObject(AbstractRepresentation* encodedObject);
	
		virtual AbstractContourRepresentation* getNewContourRepresentation() = 0;
	};
}

#endif