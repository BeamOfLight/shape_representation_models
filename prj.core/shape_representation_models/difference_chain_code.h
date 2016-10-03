#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_DIFFERENCE_CHAIN_CODE_H
#define SHAPE_REPRESENTATION_MODELS_DIFFERENCE_CHAIN_CODE_H

#include <shape_representation_models/abstract_contour_model.h>

namespace ShapeRepresentationModels {
	class DifferenceChainCode : public AbstractContourModel
	{
	   protected:
		bool use4pin; // флаг использования 4-связного кода Фримана
	
		int getPinCount();
		AbstractContourRepresentation* getNewContourRepresentation();
	  public:
		struct ContourRepresentation : public AbstractContourRepresentation
		{
			cv::Point firstPoint;
			std::vector < unsigned char > chainCodes;
			bool use4pin;
		
			ContourRepresentation(bool use4pin);
			std::vector < cv::Point > convert2Points();
			void initFromPoints(const std::vector < cv::Point > &points);
		};

		DifferenceChainCode(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize, bool use4pin);
		std::string getMethodName();
		int getObjectRepresentationSize(AbstractRepresentation* encodedObject);
	};
}

#endif