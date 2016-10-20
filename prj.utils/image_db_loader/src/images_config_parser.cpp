#include <image_db_loader/images_config_parser.h>

const int DIGITS_COUNT_IN_IMG_NUMBER = 4;

std::vector<std::string> ImageDbLoader::ImagesConfigParser::parseConfig(const std::string& filename, bool enableLog, const std::string &suffix)
{
  std::vector<std::string> result;

  // Open
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  if (!fs.isOpened()) {
    std::cerr << "Failed to open " << filename << std::endl;
    return result;
  }

  // Check databases field
  cv::FileNode fileNodeDatabases = fs["databases"];
  if (fileNodeDatabases.type() != cv::FileNode::SEQ) {
    std::cerr << "Field \"databases\" is not a sequence! FAIL" << std::endl;
    return result;
  }
  cv::FileNodeIterator db_it = fileNodeDatabases.begin(), db_it_end = fileNodeDatabases.end();
  for( ; db_it != db_it_end; ++db_it) {
  	// Read database name
  	std::string database_name = (std::string)(*db_it)["database"];
  	if (enableLog) {
  	 std::cout << "database: " << database_name << std::endl;
  	}
  	//Read categories
  	cv::FileNode fileNodeCategories = (*db_it)["categories"];
  	if (fileNodeCategories.type() != cv::FileNode::SEQ) {
      std::cerr << "Field \"categories\" for database \"" << database_name << "\" is not a sequence! FAIL" << std::endl;
      return result;
  	}
  	cv::FileNodeIterator cat_it = fileNodeCategories.begin(), cat_it_end = fileNodeCategories.end();
  	for( ; cat_it != cat_it_end; ++cat_it) {
  	  std::string category = (std::string)(*cat_it)["category"];
  	  std::string category_images_list = (std::string)(*cat_it)["images"];
  	  if (enableLog) {
  	    std::cout << "category: " << category << " images: " << category_images_list << std::endl;
  	  }
  	  std::vector<int> img_ids = ImageDbLoader::IntervalStringParser::parseIntervalString(category_images_list);
  	  int img_cnt = std::min(static_cast<int>(img_ids.size()), static_cast<int>(pow(10, DIGITS_COUNT_IN_IMG_NUMBER)));
  	  std::stringstream ss;
  	  for (int image_id = 0; image_id < img_cnt; image_id++) {
  	  	ss.str("");
  	  	ss << "data/images/" << database_name << "/" << category << "/" << category << "_" << std::setw(DIGITS_COUNT_IN_IMG_NUMBER) << std::setfill('0') << image_id << suffix;
  	  	result.push_back(ss.str());
  	  	if (enableLog) {
          std::cout << ss.str() << std::endl;
        }
  	  }
  	}

  }

  fs.release();
  return result;
}