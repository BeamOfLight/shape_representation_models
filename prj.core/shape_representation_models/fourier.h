#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_FOURIER_H
#define SHAPE_REPRESENTATION_MODELS_FOURIER_H

#include <shape_representation_models/abstract_contour_model.h>

namespace ShapeRepresentationModels {
	class Fourier : public AbstractContourModel
	{
	  protected:
		int fourierDescriptorSize;

		AbstractContourRepresentation* getNewContourRepresentation();
	  public:
		struct ContourRepresentation : public AbstractContourRepresentation
		{
			std::vector < cv::Vec2f > fourierDescriptorLeft;
			std::vector < cv::Vec2f > fourierDescriptorRight;
			int contourLength;
			int fourierDescriptorSize;
		
			int getResultFourierDescriptorSize();
		
			ContourRepresentation(int fourierDescriptorSize);
			std::vector < cv::Point > convert2Points();
			void initFromPoints(const std::vector < cv::Point > &points);
		};  

		Fourier(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize, int fourierDescriptorSize);
		std::string getMethodName();
		int getObjectRepresentationSize(AbstractRepresentation* encodedObject);
	};
}

#endif