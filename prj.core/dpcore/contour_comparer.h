/**
    https://github.com/BeamOfLight/shape_representation_models.git
    contour_comparer.h

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04 
*/

#pragma once
#ifndef DPCORE_CONTOUR_COMPARER_H
#define DPCORE_CONTOUR_COMPARER_H

#include <vector>
#include <iostream>

#include "opencv2/opencv.hpp"

namespace DpCore
{
  class ContourComparer
  {
    private:
      std::vector < cv::Point > srcContour;
      std::vector < cv::Point > dstContour;
      cv::Mat srcImage;
      cv::Mat dstImage;
      cv::Mat diffImage;
      cv::Size calculateImageSize();
      cv::Mat createContourImage(cv::Mat img, std::vector < cv::Point > contour);
      cv::Mat getContourAreaImage(cv::Mat image);

    public:
      ContourComparer(std::vector <Point> srcContour, std::vector < cv::Point > dstContour);

      float compare();
      cv::Mat getSrcImage();
      cv::Mat getDstImage();
      cv::Mat getDiffImage();
  };
}

#endif
