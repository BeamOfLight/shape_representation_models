#pragma once
#ifndef IMAGE_DB_LOADER_IMAGE_LOADER_H
#define IMAGE_DB_LOADER_IMAGE_LOADER_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <dpcore/filesystem.h>
#include <image_db_loader/images_config_parser.h>

namespace ImageDbLoader
{
  class ImageLoader
  {
  private:
  	std::vector<cv::Mat> images;
  public:
    ImageLoader();
    size_t loadImages(const std::string &config_filename, bool inversion = false);
    size_t getImagesCount();
    cv::Mat getImageById(size_t image_id);
  };
}

#endif