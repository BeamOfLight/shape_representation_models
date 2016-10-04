/**
    https://github.com/BeamOfLight/shape_representation_models.git
    resampling_chain_code.h

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04
*/

#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_RESAMPLING_CHAIN_CODE_H
#define SHAPE_REPRESENTATION_MODELS_RESAMPLING_CHAIN_CODE_H

#include <shape_representation_models/abstract_contour_model.h>

namespace ShapeRepresentationModels {
	class ResamplingChainCode : public AbstractContourModel
	{
	   protected:
		bool use4pin; // флаг использования 4-связного кода Фримана
		size_t resamplingLevel;

		int getPinCount();
		cv::Size getOptimalSize(cv::Size objectSize);
		unsigned char getNPointValue(const cv::Mat &object, cv::Point pt);
		AbstractContourRepresentation* getNewContourRepresentation();
		AbstractRepresentation* encodeSingleObject(const cv::Mat &object);
	  public:
		struct ContourRepresentation : public AbstractContourRepresentation
		{
			cv::Point firstPoint;
			std::vector < unsigned char > chainCodes;
			bool use4pin;
			size_t resamplingLevel;

			ContourRepresentation(bool use4pin, size_t resamplingLevel);
			std::vector < cv::Point > convert2Points();
			void initFromPoints(const std::vector < cv::Point > &points);
		};

		ResamplingChainCode(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize, bool use4pin, size_t resamplingLevel);
		std::string getMethodName();
		int getObjectRepresentationSize(AbstractRepresentation* encodedObject);
	};
}

#endif
