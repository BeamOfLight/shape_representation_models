#include <image_db_loader/image_loader.h>

ImageDbLoader::ImageLoader::ImageLoader()
{

}

size_t ImageDbLoader::ImageLoader::loadImages(const std::string &config_filename, bool inversion)
{
  size_t count = 0;
  std::vector<std::string> filenames = ImageDbLoader::ImagesConfigParser::parseConfig(config_filename, true);
  cv::Mat tmp;

  if (inversion) {
    for(size_t id = 0; id < filenames.size(); id++) {
      tmp = cv::imread(DpCore::Filesystem::getRootProjectDirectory() + filenames[id], cv::IMREAD_GRAYSCALE );
      if (!tmp.empty()) {
        images.push_back(tmp.clone());
        count++;
      }
    }
  } else {
    cv::Mat ones;
    for(size_t id = 0; id < filenames.size(); id++) {
      tmp = cv::imread(DpCore::Filesystem::getRootProjectDirectory() + filenames[id], cv::IMREAD_GRAYSCALE );
      ones = cv::Mat::ones(tmp.size(), tmp.type());
      tmp = ones - tmp;
      if (!tmp.empty()) {
        images.push_back(tmp.clone());
        count++;
      }
    }
  }
  

  return count;
}

size_t ImageDbLoader::ImageLoader::getImagesCount()
{
  return static_cast<size_t>(images.size());
}
  
cv::Mat ImageDbLoader::ImageLoader::getImageById(size_t image_id)
{
  cv::Mat result;
  if (image_id < getImagesCount()) {
    result = images[image_id]; 
  }

  return result;
}

