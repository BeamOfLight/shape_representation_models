/**
	https://github.com/BeamOfLight/shape_representation_models.git
    freeman_chain_code.cpp

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04 
*/

#include <shape_representation_models/freeman_chain_code.h>

ShapeRepresentationModels::FreemanChainCode::FreemanChainCode(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize, bool use4pin)
: AbstractContourModel (contoursCountSize, pointsCountSize, pointRepresentationSize)
{
	this->use4pin = use4pin;
}

int ShapeRepresentationModels::FreemanChainCode::getPinCount()
{
	return use4pin ? 4 : 8;
}

std::string ShapeRepresentationModels::FreemanChainCode::getMethodName()
{
	std::stringstream ss;
	ss << "FCC" << getPinCount();
	
	return ss.str();
}

ShapeRepresentationModels::AbstractContourModel::AbstractContourRepresentation* ShapeRepresentationModels::FreemanChainCode::getNewContourRepresentation()
{
	return new ContourRepresentation(use4pin);
}

int ShapeRepresentationModels::FreemanChainCode::getObjectRepresentationSize(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
	int chainCodesCount = static_cast < ContourRepresentation* > (objectRepresentation->data.outterContour)->chainCodes.size();
	int holesCount = objectRepresentation->data.innerContours.size();
	for (int contourId = 0; contourId < holesCount; contourId++) {
		chainCodesCount += static_cast < ContourRepresentation* > (objectRepresentation->data.innerContours[contourId])->chainCodes.size();
	}
	float resultInBits = (float) ((pointsCountSize + pointRepresentationSize) * (holesCount + 1) + chainCodesCount * log2(getPinCount()) + contoursCountSize);

	return (int) ceil(resultInBits / 8);
}

ShapeRepresentationModels::FreemanChainCode::ContourRepresentation::ContourRepresentation(bool use4pin)
{
	this->use4pin = use4pin;
}

void ShapeRepresentationModels::FreemanChainCode::ContourRepresentation::initFromPoints(const std::vector < cv::Point > &points)
{
	unsigned char chainCodesMap8[3][3] = {
		{7, 0, 1},
		{6, 9, 2},
		{5, 4, 3},
	};

	int pointsCount = points.size();
	if (pointsCount > 0) {
		firstPoint = points[0];
	}
	
	chainCodes.clear();
	unsigned char curChainCode;
	if (!use4pin) {
		for (int i = 1; i < pointsCount; i++) {
			chainCodes.push_back(
				chainCodesMap8[points[i].x - points[i-1].x + 1][points[i].y - points[i-1].y + 1]
			);
		}
	} else {
		for (int i = 1; i < pointsCount; i++) {
			curChainCode = chainCodesMap8[points[i].x - points[i-1].x + 1][points[i].y - points[i-1].y + 1];
			if (curChainCode % 2 == 1) {
				if (curChainCode == 1) {
					chainCodes.push_back(2);
					chainCodes.push_back(0);
				} else if (curChainCode == 3) {
					chainCodes.push_back(4);
					chainCodes.push_back(2);
				} else if (curChainCode == 5) {
					chainCodes.push_back(6);
					chainCodes.push_back(4);
				} else if (curChainCode == 7) {
					chainCodes.push_back(0);
					chainCodes.push_back(6);
				}
			} else {
				chainCodes.push_back(curChainCode);
			}
		}
	}
}

std::vector < cv::Point > ShapeRepresentationModels::FreemanChainCode::ContourRepresentation::convert2Points()
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
	int chainCodesCount = chainCodes.size();

	for (int i = 0; i < chainCodesCount; i++) {
		currentPoint.x -= chainCodesInvMap8[chainCodes[i]][1];
		currentPoint.y += chainCodesInvMap8[chainCodes[i]][0];
		result.push_back(currentPoint);
	}
	
	return result;
}