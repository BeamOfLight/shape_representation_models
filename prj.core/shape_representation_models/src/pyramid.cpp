/**
  https://github.com/BeamOfLight/shape_representation_models.git
  pyramid.cpp

  @author Denis Borisoglebskiy
  @version 1.0 2016-10-04
*/

#include <shape_representation_models/pyramid.h>

ShapeRepresentationModels::Pyramid::Pyramid(size_t pointRepresentationSize, size_t levelsCount)
{
  this->pointRepresentationSize = pointRepresentationSize;
  this->levelsCount = levelsCount;
}

std::string ShapeRepresentationModels::Pyramid::getMethodName()
{
  std::stringstream ss;
  ss << "Pyramid(" << levelsCount << ")";

  return ss.str();
}

cv::Size ShapeRepresentationModels::Pyramid::getOptimalSize(int width, int height)
{
  int step = (int) pow(2, levelsCount - 1);

  return cv::Size(
      step * (int) ceil((float) width / step),
      step * (int) ceil((float) height / step)
    );
}

int ShapeRepresentationModels::Pyramid::getObjectRepresentationSize(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
  Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
  int levelsCountRepresentationSize = 4;
  float resultInBits = (float) (2 * pointRepresentationSize + levelsCountRepresentationSize);
  for (size_t level = 0; level < levelsCount; level++) {
    resultInBits += objectRepresentation->masks[level].rows * objectRepresentation->masks[level].cols;
  }

  return (int) ceil(resultInBits / 8);
}

ShapeRepresentationModels::AbstractModel::AbstractRepresentation* ShapeRepresentationModels::Pyramid::encodeSingleObject(const cv::Mat &object)
{
  Representation* objectRepresentation = new Representation();
  std::vector < cv::Point > contour = getImageContour(object);
  cv::Rect rect = boundingRect(contour);
  objectRepresentation->rect = cv::Rect(rect.x + 1, rect.y + 1, rect.width - 2, rect.height - 2);

  cv::Size imageSize = getOptimalSize(objectRepresentation->rect.width, objectRepresentation->rect.height);
  objectRepresentation->masks.push_back(
    cv::Mat::ones(imageSize, CV_8UC1) * 255
  );

  for (int y = 0; y < imageSize.height; y++) {
    for (int x = 0; x < imageSize.width; x++) {
      objectRepresentation->masks[0].at < uchar > (y, x) = object.at < uchar > (y + objectRepresentation->rect.y, x + objectRepresentation->rect.x);
    }
  }

  for (size_t level = 1; level < levelsCount; level++) {
    int ratio = (int) pow(2, level);
    cv::Size currentImageSize = cv::Size(
      imageSize.width / ratio,
      imageSize.height / ratio
    );
    objectRepresentation->masks.push_back(
      cv::Mat::ones(currentImageSize, CV_8UC1) * 255
    );

    pyrDown(objectRepresentation->masks[level - 1], objectRepresentation->masks[level], currentImageSize);
  }

  return objectRepresentation;
}

cv::Mat ShapeRepresentationModels::Pyramid::decodeSingleObject(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
  Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
  cv::Size imageSize = cv::Size(
    objectRepresentation->rect.x + objectRepresentation->rect.width,
    objectRepresentation->rect.y + objectRepresentation->rect.height
  );
  cv::Mat result = cv::Mat::ones(imageSize, CV_8UC1);

  for (int y = 0; y < objectRepresentation->rect.height; y++) {
    for (int x = 0; x < objectRepresentation->rect.width; x++) {

      result.at<uchar>(
        y + objectRepresentation->rect.y,
        x + objectRepresentation->rect.x
      ) = objectRepresentation->masks[0].at<uchar>(y, x);
    }
  }

  return result;
}
