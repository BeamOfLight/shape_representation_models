/**
  https://github.com/BeamOfLight/shape_representation_models.git
  run_length_encoding.cpp

  @author Denis Borisoglebskiy
  @version 1.0 2016-10-04
*/

#include <shape_representation_models/run_length_encoding.h>

ShapeRepresentationModels::RunLengthEncoding::RunLengthEncoding(size_t pointRepresentationSize)
{
  this->pointRepresentationSize = pointRepresentationSize;
}

std::string ShapeRepresentationModels::RunLengthEncoding::getMethodName()
{
  return std::string("RLE");
}

int ShapeRepresentationModels::RunLengthEncoding::getObjectRepresentationSize(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
  Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
  int firstBitSize = 1;
  int verticalDirectionSize = 1;
  int headerSize = 2 * pointRepresentationSize + firstBitSize + verticalDirectionSize;
  float resultInBits = (float) headerSize;

  int runLengthParametersSize = objectRepresentation->runLengthParameters.size();
  for (int i = 0; i < runLengthParametersSize; i++) {
    size_t value = objectRepresentation->runLengthParameters[i];
    if (value > 0 && value <= 8) {
      resultInBits += 4;
    } else if (value > 8 && value <= 120) {
      resultInBits += 8;
    } else if (value > 120 && value <= 4215) {
      resultInBits += 16;
    } else if (value > 4225 && value <= 4294971521) {
      resultInBits += 40;
    } else {
      std::cout << "[" << getMethodName() << "] Incorrect value " << value << std::endl;
      throw;
    }
  }

  return (int) ceil(resultInBits / 8);
}

ShapeRepresentationModels::AbstractModel::AbstractRepresentation* ShapeRepresentationModels::RunLengthEncoding::encodeSingleObject(const cv::Mat &object)
{
  Representation* objectRepresentation = new Representation();
  std::vector < cv::Point > contour = getImageContour(object);
  cv::Rect rect = boundingRect(contour);
  objectRepresentation->rect = cv::Rect(rect.x + 1, rect.y + 1, rect.width - 2, rect.height - 2);
  cv::Mat mask = cv::Mat(object, objectRepresentation->rect);

  objectRepresentation->firstBit = mask.at < uchar > (0, 0) > 0;

  int count = 0;
  bool currentValue = objectRepresentation->firstBit;

  // Horizontal
  std::vector < int > horizontalData;
  for (int y = 0; y < mask.rows; y++) {
    for (int x = 0; x < mask.cols; x++) {
      if (currentValue == (mask.at < uchar > (y, x) > 0)) {
        count++;
      } else {
        horizontalData.push_back(count);
        count = 1;
        currentValue = !currentValue;
      }
    }
  }

  if (count) {
    horizontalData.push_back(count);
  }

  //Vertical
  count = 0;
  std::vector < int > verticalData;
  for (int y = 0; y < mask.rows; y++) {
    for (int x = 0; x < mask.cols; x++) {
      if (currentValue == (mask.at < uchar > (y, x) > 0)) {
        count++;
      } else {
        verticalData.push_back(count);
        count = 1;
        currentValue = !currentValue;
      }
    }
  }

  if (count) {
    verticalData.push_back(count);
  }

  if (horizontalData.size() > verticalData.size()) {
    objectRepresentation->verticalDirection = true;
    objectRepresentation->runLengthParameters = verticalData;
  } else {
    objectRepresentation->verticalDirection = false;
    objectRepresentation->runLengthParameters = horizontalData;
  }

  return objectRepresentation;
}

cv::Mat ShapeRepresentationModels::RunLengthEncoding::decodeSingleObject(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
  Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
  std::vector < uchar > data;
  uchar currentValue = objectRepresentation->firstBit ? 255 : 0;
  int runLengthParametersCount = objectRepresentation->runLengthParameters.size();
  for (int i = 0 ; i < runLengthParametersCount; i++) {
    for (int j = 0; j < objectRepresentation->runLengthParameters[i]; j++) {
      data.push_back(currentValue);
    }
    currentValue = 255 - currentValue;
  }

  cv::Size imageSize = cv::Size(
    objectRepresentation->rect.x + objectRepresentation->rect.width,
    objectRepresentation->rect.y + objectRepresentation->rect.height
  );
  cv::Mat result = cv::Mat::ones(imageSize, CV_8UC1);
  int k = 0;
  if (objectRepresentation->verticalDirection) {
    for (int x = 0; x < objectRepresentation->rect.width; x++) {
      for (int y = 0; y < objectRepresentation->rect.height; y++) {
        result.at<uchar>(
          y + objectRepresentation->rect.y,
          x + objectRepresentation->rect.x
        ) = data[k++];
      }
    }
  } else {
    for (int y = 0; y < objectRepresentation->rect.height; y++) {
      for (int x = 0; x < objectRepresentation->rect.width; x++) {
        result.at<uchar>(
          y + objectRepresentation->rect.y,
          x + objectRepresentation->rect.x
        ) = data[k++];
      }
    }
  }

  return result;
}
