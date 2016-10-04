/**
  https://github.com/BeamOfLight/shape_representation_models.git
  pyramid.h

  @author Denis Borisoglebskiy
  @version 1.0 2016-10-04
*/

#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_PYRAMID_H
#define SHAPE_REPRESENTATION_MODELS_PYRAMID_H

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <shape_representation_models/abstract_area_model.h>

namespace ShapeRepresentationModels {
  class Pyramid : public AbstractAreaModel
  {
    private:
      size_t pointRepresentationSize;
      size_t levelsCount;

      cv::Size getOptimalSize(int width, int height);
    public:
      struct Representation : public AbstractRepresentation
      {
        std::vector < cv::Mat > masks;
        cv::Rect rect;
      };

      Pyramid(size_t pointRepresentationSize, size_t levelsCount);
      std::string getMethodName();

      int getObjectRepresentationSize(AbstractRepresentation* encodedObject);
      AbstractRepresentation* encodeSingleObject(const cv::Mat &object);
      cv::Mat decodeSingleObject(AbstractRepresentation* encodedObject);
  };
}

#endif
