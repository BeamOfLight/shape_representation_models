/**
  https://github.com/BeamOfLight/shape_representation_models.git
  contour_comparer.cpp

  @author Denis Borisoglebskiy
  @version 1.0 2016-10-04
*/

#include <dpcore/contour_comparer.h>

DpCore::ContourComparer::ContourComparer(std::vector < cv::Point > srcContour, std::vector < cv::Point > dstContour)
{
  this->srcContour = srcContour;
  this->dstContour = dstContour;

  cv::Size image_size = calculateImageSize();
  srcImage = cv::Mat::zeros(image_size, CV_8UC1);
  dstImage = cv::Mat::zeros(image_size, CV_8UC1);
  diffImage = cv::Mat::zeros(image_size, CV_8UC1);
}

cv::Size DpCore::ContourComparer::calculateimageSize()
{
  cv::Rect src_rect = cv::boundingcv::Rect(srcContour);
  cv::Rect dst_rect = cv::boundingcv::Rect(dstContour);

  return cv::Size(
    max(src_rect.x + src_rect.width, dst_rect.x + dst_rect.width) + 2,
    max(src_rect.y + src_rect.height, dst_rect.y + dst_rect.height) + 2
  );
}

cv::Mat DpCore::ContourComparer::createContourImage(cv::Mat img, std::vector<cv::Point> contour)
{
  std::vector<std::vector<cv::Point> > contours;
  contours.push_back(contour);
  cv::drawContours(img, contours, -1, cv::Scalar(255, 255, 255), 1, 8, cv::noArray(), INT_MAX, cv::Point(1,1));

  return img;
}

float DpCore::ContourComparer::compare()
{
  srcImage = getContourAreaImage(createContourImage(srcImage, srcContour));
  dstImage = getContourAreaImage(createContourImage(dstImage, dstContour));

  cv::bitwise_xor(srcImage, dstImage,	diffImage);

  int diffcv::PointsCounter = 0;
  for (int y = 0; y < diffImage.rows; y++) {
    for (int x = 0; x < diffImage.cols; x++) {
      if (diffImage.at<uchar>(cv::Point(x,y))) {
        diffcv::PointsCounter++;
      }
    }
  }

  int initAreaCounter = 0;
  for (int y = 0; y < srcImage.rows; y++) {
    for (int x = 0; x < srcImage.cols; x++) {
      if (!srcImage.at<uchar>(cv::Point(x,y))) {
        initAreaCounter++;
      }
    }
  }

  return (float) diffcv::PointsCounter * 100 / (float) initAreaCounter;
}


cv::Mat DpCore::ContourComparer::getContourAreaImage(cv::Mat image)
{
  cv::Mat mask = cv::Mat::zeros(cv::Size(image.size().width + 2, image.size().height + 2), CV_8UC1);
  cv::Rect rect;
  cv::floodFill(
    image,
    mask,
    cv::Point(image.cv::Size().width - 1, image.cv::Size().height - 1),
    cv::Scalar(255, 255, 255),
    &rect,
    cv::Scalar(0, 0, 0),
    cv::Scalar(255, 255, 255),
    FLOODFILL_MASK_ONLY
  );
  mask.copyTo(image);

  return mask;
}

cv::Mat DpCore::ContourComparer::getSrcImage()
{
  return srcImage;
}

cv::Mat DpCore::ContourComparer::getDstImage()
{
  return dstImage;
}

cv::Mat DpCore::ContourComparer::getDiffImage()
{
  return diffImage;
}
