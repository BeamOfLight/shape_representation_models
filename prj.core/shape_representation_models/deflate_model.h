/**
	https://github.com/BeamOfLight/shape_representation_models.git
    deflate_model.h

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04 
*/

#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_DEFLATE_MODEL_H
#define SHAPE_REPRESENTATION_MODELS_DEFLATE_MODEL_H

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <shape_representation_models/abstract_other_model.h>

/*
https://ru.wikipedia.org/wiki/Deflate
LZ77 + Huffman coding
*/
namespace ShapeRepresentationModels {
	class DeflateModel : public AbstractOtherModel
	{
	  private:
		size_t pointRepresentationSize;
		size_t compressionLevel;
		size_t strategy;
		bool biLevel;

	  public:
		struct Representation : public AbstractRepresentation
		{
			std::vector < uchar > buffer;
		};

		DeflateModel(size_t pointRepresentationSize, size_t compressionLevel, size_t strategy, bool biLevel);
		std::string getMethodName();

		int getObjectRepresentationSize(AbstractRepresentation* encodedObject);
		AbstractRepresentation* encodeSingleObject(const cv::Mat &object);
		cv::Mat decodeSingleObject(AbstractRepresentation* encodedObject);
	};
}

#endif