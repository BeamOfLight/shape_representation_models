/**
	https://github.com/BeamOfLight/shape_representation_models.git
    abstract_model.h

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04 
*/

#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_ABSTRACT_MODEL_H
#define SHAPE_REPRESENTATION_MODELS_ABSTRACT_MODEL_H

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cmath>
#include <vector>
#include <sstream>

namespace ShapeRepresentationModels
{
	inline long double log2(const long double x){
		return  log(x) / CV_LOG2;
	}

	struct AbstractModel
	{
		enum MethodType {CONTOUR = 1, AREA = 2, OTHER = 3};
		struct AbstractRepresentation
		{
		};
	
		virtual std::string getMethodName() = 0;
		virtual int getMethodType() const = 0;
		virtual int getObjectRepresentationSize(AbstractRepresentation* encodedObject) = 0;
		virtual AbstractRepresentation* encodeSingleObject(const cv::Mat &object) = 0;
		virtual cv::Mat decodeSingleObject(AbstractRepresentation* encodedObject) = 0;	
	};
}

#endif