/**
	https://github.com/BeamOfLight/shape_representation_models.git
    shape_representation_experimental_stand.h

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04 
*/

#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_SHAPE_REPRESENTATION_EXPERIMENTAL_STAND_H
#define SHAPE_REPRESENTATION_MODELS_SHAPE_REPRESENTATION_EXPERIMENTAL_STAND_H

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <utility>
#include <omp.h>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <dpcore/common.h>
#include <shape_representation_models/abstract_model.h>
#include <shape_representation_models/configuration.h>

namespace ShapeRepresentationModels {
	struct SummaryStat
	{
		float sum;
		float min;
		float max;
		float avg;
		float sigma;
	};

	struct ModelInfo
	{
		std::string methodName;
		SummaryStat areaDiff;
		int representationSize;
		float encodeTime;
		float decodeTime;
	};

	class ShapeRepresentationExperimentalStand
	{
	  protected:
		std::vector < cv::Mat > srcObjects;
		std::vector < AbstractModel* > models;
		std::vector < ModelInfo* > modelsInfo;
		Configuration* configuration;
	
		// Работа с потоками
		std::vector < cv::Mat > srcThreadObjects;
		std::vector < AbstractModel::AbstractRepresentation* > encodedThreadObjects;
		std::vector < cv::Mat > decodedThreadObjects;
		std::vector < int > threadStatus;
	
		void encodeObjectThreadFunc(std::pair < int, int > const &params);
		void decodeObjectThreadFunc(std::pair < int, int > const &params);
	
		float getTimeInSeconds(clock_t time);
		SummaryStat getAreaDiffSummaryStat(int modelId, std::vector < cv::Mat > const &dstObjects);
		cv::Mat normalizeBinaryImage(cv::Mat image);
	  public:
		ShapeRepresentationExperimentalStand(std::vector < AbstractModel* >, std::vector < cv::Mat >, Configuration*);
		float compare(cv::Mat object1, cv::Mat object2, int objectId = 0);
		float start();
		void startForSingleModel(int);
		int getModelRepresentationSize(int modelId, std::vector < AbstractModel::AbstractRepresentation* > encodedObjects);
		std::vector < ModelInfo* > getModelsInfo();
		void show(int methodType);
	
		std::vector < AbstractModel::AbstractRepresentation* > encode(int modelId, std::vector < cv::Mat > objects);
		std::vector < cv::Mat > decode(int modelId, std::vector < AbstractModel::AbstractRepresentation* > encodedObjects);
	};
}

#endif