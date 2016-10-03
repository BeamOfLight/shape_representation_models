#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_FREEMAN_CHAIN_CODE_RLE_H
#define SHAPE_REPRESENTATION_MODELS_FREEMAN_CHAIN_CODE_RLE_H

#include <shape_representation_models/abstract_contour_model.h>

namespace ShapeRepresentationModels {
	class FreemanChainCodeRLE : public AbstractContourModel
	{
	  protected:
		bool use4pin; // флаг использования 4-связного кода Фримана
		size_t rleBits; // Биты, выделяемые под RLE поле
	
		int getPinCount();
		AbstractContourRepresentation* getNewContourRepresentation();
	  public:
		struct ContourRepresentation : public AbstractContourRepresentation
		{
			cv::Point firstPoint;
			std::vector < unsigned char > chainCodes;
			std::vector < size_t > rleCodes;
			bool use4pin;
			size_t rleBits;
		
			ContourRepresentation(bool use4pin, size_t rleBits);
			std::vector < cv::Point > convert2Points();
			void initFromPoints(const std::vector < cv::Point > &points);
		};  

		FreemanChainCodeRLE(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize, bool use4pin, size_t rleBits);
		std::string getMethodName();
		int getObjectRepresentationSize(AbstractRepresentation* encodedObject);
	};
}

#endif