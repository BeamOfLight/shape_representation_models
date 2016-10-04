/**
  https://github.com/BeamOfLight/shape_representation_models.git
  abstract_area_model.cpp

  @author Denis Borisoglebskiy
  @version 1.0 2016-10-04
*/

#include <shape_representation_models/abstract_area_model.h>

int ShapeRepresentationModels::AbstractAreaModel::getMethodType() const
{
  return ShapeRepresentationModels::AbstractModel::MethodType::AREA;
}

std::vector < cv::Point > ShapeRepresentationModels::AbstractAreaModel::getImageContour(const cv::Mat &image)
{
  std::vector < std::vector < cv::Point > > srcContours;
  std::vector < cv::Vec4i > hierarchy;

  cv::Mat tmp = cv::Mat::zeros(image.size(), CV_8UC1);
  image.copyTo(tmp);

  cv::findContours(tmp, srcContours, hierarchy, CV_RETR_TREE , CV_CHAIN_APPROX_NONE);
  tmp.release();

  return srcContours[1];
}

