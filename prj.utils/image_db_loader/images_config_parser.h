#pragma once
#ifndef IMAGE_DB_LOADER_IMAGES_CONFIG_PARSER_H
#define IMAGE_DB_LOADER_IMAGES_CONFIG_PARSER_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>

#include <opencv2/core/core.hpp>

#include <image_db_loader/interval_string_parser.h>

namespace ImageDbLoader
{
  namespace ImagesConfigParser
  {
		std::vector<std::string> parseConfig(const std::string& filename, bool enableLog = false, const std::string &suffix = ".bmp");
	}
}

#endif