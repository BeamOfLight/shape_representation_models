/**
  https://github.com/BeamOfLight/shape_representation_models.git
  freeman_chain_code_RLE.cpp

  @author Denis Borisoglebskiy
  @version 1.0 2016-10-04
*/

#include <shape_representation_models/freeman_chain_code_RLE.h>

ShapeRepresentationModels::FreemanChainCodeRLE::FreemanChainCodeRLE(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize, bool use4pin, size_t rleBits)
: AbstractContourModel (contoursCountSize, pointsCountSize, pointRepresentationSize)
{
  this->use4pin = use4pin;
  this->rleBits = rleBits;
}

int ShapeRepresentationModels::FreemanChainCodeRLE::getPinCount()
{
  return use4pin ? 4 : 8;
}

std::string ShapeRepresentationModels::FreemanChainCodeRLE::getMethodName()
{
  std::stringstream ss;
  ss << "FCC" << getPinCount() << "+RLE(" << rleBits << ")";

  return ss.str();
}

ShapeRepresentationModels::AbstractContourModel::AbstractContourRepresentation* ShapeRepresentationModels::FreemanChainCodeRLE::getNewContourRepresentation()
{
  return new ContourRepresentation(use4pin, rleBits);
}

int ShapeRepresentationModels::FreemanChainCodeRLE::getObjectRepresentationSize(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
  Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
  int chainCodesCount = static_cast < ContourRepresentation* > (objectRepresentation->data.outterContour)->chainCodes.size();
  int holesCount = objectRepresentation->data.innerContours.size();
  for (int contourId = 0; contourId < holesCount; contourId++) {
    chainCodesCount += static_cast < ContourRepresentation* > (objectRepresentation->data.innerContours[contourId])->chainCodes.size();
  }
  float resultInBits = (float) ((pointsCountSize + pointRepresentationSize) * (holesCount + 1) + chainCodesCount * (rleBits + log2(getPinCount())) + contoursCountSize);

  return (int) ceil(resultInBits / 8);
}

ShapeRepresentationModels::FreemanChainCodeRLE::ContourRepresentation::ContourRepresentation(bool use4pin, size_t rleBits)
{
  this->use4pin = use4pin;
  this->rleBits = rleBits;
}

void ShapeRepresentationModels::FreemanChainCodeRLE::ContourRepresentation::initFromPoints(const std::vector < cv::Point > &points)
{
  std::vector < unsigned char > chainCodesWithoutRLE;
  unsigned char chainCodesMap8[3][3] = {
    {7, 0, 1},
    {6, 9, 2},
    {5, 4, 3},
  };

  int pointsCount = points.size();
  if (pointsCount > 0) {
    firstPoint = points[0];
  }

  unsigned char curChainCode;
  if (!use4pin) {
    for (int i = 1; i < pointsCount; i++) {
      chainCodesWithoutRLE.push_back(
        chainCodesMap8[points[i].x - points[i-1].x + 1][points[i].y - points[i-1].y + 1]
      );
    }
  } else {
    for (int i = 1; i < pointsCount; i++) {
      curChainCode = chainCodesMap8[points[i].x - points[i-1].x + 1][points[i].y - points[i-1].y + 1];
      if (curChainCode % 2 == 1) {
        if (curChainCode == 1) {
          chainCodesWithoutRLE.push_back(2);
          chainCodesWithoutRLE.push_back(0);
        } else if (curChainCode == 3) {
          chainCodesWithoutRLE.push_back(4);
          chainCodesWithoutRLE.push_back(2);
        } else if (curChainCode == 5) {
          chainCodesWithoutRLE.push_back(6);
          chainCodesWithoutRLE.push_back(4);
        } else if (curChainCode == 7) {
          chainCodesWithoutRLE.push_back(0);
          chainCodesWithoutRLE.push_back(6);
        }
      } else {
        chainCodesWithoutRLE.push_back(curChainCode);
      }
    }
  }

  size_t chainCodesWithoutRLECount = chainCodesWithoutRLE.size();
  unsigned char lastChainCode = -1;
  size_t chainCodesCount = 0;

  chainCodes.clear();
  rleCodes.clear();
  for (size_t i = 0; i < chainCodesWithoutRLECount; i++) {
    if (lastChainCode == chainCodesWithoutRLE[i] && rleCodes[chainCodesCount - 1] < pow(2, rleBits) - 1) {
      rleCodes[chainCodesCount - 1]++;
    } else {
      chainCodes.push_back(chainCodesWithoutRLE[i]);
      rleCodes.push_back(0);
      chainCodesCount++;
      lastChainCode = chainCodesWithoutRLE[i];
    }
  }
}

std::vector < cv::Point > ShapeRepresentationModels::FreemanChainCodeRLE::ContourRepresentation::convert2Points()
{
  char chainCodesInvMap8[8][2] = {
    {0, 1},
    {1, 1},
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, -1},
    {-1, 0},
    {-1, 1}
  };

  std::vector < cv::Point > result;
  result.push_back(firstPoint);
  cv::Point currentPoint = firstPoint;
  size_t chainCodesCount = chainCodes.size();

  for (size_t i = 0; i < chainCodesCount; i++) {
    for (size_t j = 0; j < rleCodes[i] + 1; j++) {
      currentPoint.x -= chainCodesInvMap8[chainCodes[i]][1];
      currentPoint.y += chainCodesInvMap8[chainCodes[i]][0];
      result.push_back(currentPoint);
    }
  }

  return result;
}
