#include <shape_representation_models/difference_chain_code.h>

ShapeRepresentationModels::DifferenceChainCode::DifferenceChainCode(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize, bool use4pin)
: AbstractContourModel (contoursCountSize, pointsCountSize, pointRepresentationSize)
{
	this->use4pin = use4pin;
}

int ShapeRepresentationModels::DifferenceChainCode::getPinCount()
{
	return use4pin ? 4 : 8;
}

std::string ShapeRepresentationModels::DifferenceChainCode::getMethodName()
{
	std::stringstream ss;
	ss << "DCC" << getPinCount();
	
	return ss.str();
}

ShapeRepresentationModels::AbstractContourModel::AbstractContourRepresentation* ShapeRepresentationModels::DifferenceChainCode::getNewContourRepresentation()
{
	return new DifferenceChainCode::ContourRepresentation(use4pin);
}

int ShapeRepresentationModels::DifferenceChainCode::getObjectRepresentationSize(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
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

ShapeRepresentationModels::DifferenceChainCode::ContourRepresentation::ContourRepresentation(bool use4pin)
{
	this->use4pin = use4pin;
}

void ShapeRepresentationModels::DifferenceChainCode::ContourRepresentation::initFromPoints(const std::vector < cv::Point > &points)
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
	
	std::vector < unsigned char > freemanChainCodes;
	unsigned char curChainCode;
	if (!use4pin) {
		for (int i = 1; i < pointsCount; i++) {
			freemanChainCodes.push_back(
				chainCodesMap8[points[i].x - points[i-1].x + 1][points[i].y - points[i-1].y + 1]
			);
		}
	} else {
		for (int i = 1; i < pointsCount; i++) {
			curChainCode = chainCodesMap8[points[i].x - points[i-1].x + 1][points[i].y - points[i-1].y + 1];
			if (curChainCode % 2 == 1) {
				if (curChainCode == 1) {
					freemanChainCodes.push_back(2);
					freemanChainCodes.push_back(0);
				} else if (curChainCode == 3) {
					freemanChainCodes.push_back(4);
					freemanChainCodes.push_back(2);
				} else if (curChainCode == 5) {
					freemanChainCodes.push_back(6);
					freemanChainCodes.push_back(4);
				} else if (curChainCode == 7) {
					freemanChainCodes.push_back(0);
					freemanChainCodes.push_back(6);
				}
			} else {
				freemanChainCodes.push_back(curChainCode);
			}
		}
	}
	
	chainCodes.clear();
	int chainCodesCount = freemanChainCodes.size();
	if (chainCodesCount) {
		chainCodes.push_back(freemanChainCodes[0]);
	}
	for (int i = 1; i < chainCodesCount; i++) {
		chainCodes.push_back(
			(8 + freemanChainCodes[i] - freemanChainCodes[i - 1]) % 8
		);
	}
}

std::vector < cv::Point > ShapeRepresentationModels::DifferenceChainCode::ContourRepresentation::convert2Points()
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
	unsigned char curChainCode;
	if (chainCodesCount) {
		curChainCode = chainCodes[0];
		currentPoint.x -= chainCodesInvMap8[curChainCode][1];
		currentPoint.y += chainCodesInvMap8[curChainCode][0];
		result.push_back(currentPoint);
	}

	for (int i = 1; i < chainCodesCount; i++) {
		curChainCode = (curChainCode + chainCodes[i]) % 8;
		currentPoint.x -= chainCodesInvMap8[curChainCode][1];
		currentPoint.y += chainCodesInvMap8[curChainCode][0];
		result.push_back(currentPoint);
	}
	
	return result;
}