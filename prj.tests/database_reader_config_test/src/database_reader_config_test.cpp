
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <dpcore/common.h>
#include <dpcore/filesystem.h>


int main(int argc, char** argv)
{
	std::string database1 = "silhouettes_database";
	std::vector<std::pair<std::string,std::string>> database1_categories;
	database1_categories.push_back(std::make_pair("cats", "all"));
	database1_categories.push_back(std::make_pair("dogs", "1-10, 15, 18-21"));
	database1_categories.push_back(std::make_pair("shapes", "51, 63-70"));

	std::string filename =  DpCore::Filesystem::getCrossPlatformFileName("config/database_reader_default_config.yml");
	cv::FileStorage fs(filename, cv::FileStorage::WRITE);
	fs << "databases" << "[";
	fs << "{";
	fs << "database" << database1;
	fs << "categories" << "[";
	for(size_t i = 0; i < database1_categories.size(); i++) {
		fs << "{"
		   << "category" << database1_categories[i].first
		   << "images" << database1_categories[i].second
		   << "}";
	}
	fs << "]";
	fs << "}";
	fs << "]";

	fs.release(); 
    std::cout << "Write Done." << std::endl;
}