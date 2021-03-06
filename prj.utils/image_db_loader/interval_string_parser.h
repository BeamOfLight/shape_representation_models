/**
Code from http://stackoverflow.com/a/18764531

@author Neil Kirk	http://ideone.com/2W99Tt
*/

#pragma once
#ifndef IMAGE_DB_LOADER_INTERVAL_STRING_PARSER_H
#define IMAGE_DB_LOADER_INTERVAL_STRING_PARSER_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

namespace ImageDbLoader
{
  namespace IntervalStringParser
  {
		int convertString2Int(const std::string& str);
		std::vector<std::string> splitStringToArray(const std::string& str, char splitter);
		std::vector<int> parseIntervalString(const std::string& data);
	}
}

#endif