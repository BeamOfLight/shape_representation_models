/**
    https://github.com/BeamOfLight/shape_representation_models.git
    resampling_chain_code.cpp

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04
*/

#include <shape_representation_models/resampling_chain_code.h>

ShapeRepresentationModels::ResamplingChainCode::ResamplingChainCode(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize, bool use4pin, size_t resamplingLevel)
: AbstractContourModel (contoursCountSize, pointsCountSize, pointRepresentationSize)
{
	this->use4pin = use4pin;
	this->resamplingLevel = (size_t) log2((double) resamplingLevel);
}

int ShapeRepresentationModels::ResamplingChainCode::getPinCount()
{
	return use4pin ? 4 : 8;
}

std::string ShapeRepresentationModels::ResamplingChainCode::getMethodName()
{
	std::stringstream ss;
	ss << "RCC" << getPinCount() << "(" << resamplingLevel << ")";

	return ss.str();
}

ShapeRepresentationModels::AbstractContourModel::AbstractContourRepresentation* ShapeRepresentationModels::ResamplingChainCode::getNewContourRepresentation()
{
	return new ContourRepresentation(use4pin, resamplingLevel);
}

int ShapeRepresentationModels::ResamplingChainCode::getObjectRepresentationSize(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
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

cv::Size ShapeRepresentationModels::ResamplingChainCode::getOptimalSize(cv::Size objectSize)
{
	int step = (int) pow(2, resamplingLevel);
	return cv::Size(
			step * (int) ceil((float) objectSize.width / step),
			step * (int) ceil((float) objectSize.height / step)
		);
}

unsigned char ShapeRepresentationModels::ResamplingChainCode::getNPointValue(const cv::Mat &object, cv::Point pt)
{
	int cnt = 0;
	int step = (int) pow(2, resamplingLevel);
	for (int y = 0; y < step; y++) {
		for (int x = 0; x < step; x++) {
			if (object.at < uchar > (cv::Point(pt.x + x, pt.y + y))) {
				cnt++;
			}
		}
	}

	return (step * step <= 2 * cnt) ? 255 : 0;
}

ShapeRepresentationModels::AbstractModel::AbstractRepresentation* ShapeRepresentationModels::ResamplingChainCode::encodeSingleObject(const cv::Mat &object)
{
	cv::Size objectSize = getOptimalSize(object.size());

	cv::Mat newObject = cv::Mat::zeros(objectSize, CV_8UC1);
	for (int y = 0; y < object.rows; y++) {
		for (int x = 0; x < object.cols; x++) {
			newObject.at < uchar > (y, x) = object.at < uchar > (y, x);
		}
	}
	int step = (int) pow(2, resamplingLevel);
	cv::Size smallObjectSize = cv::Size(objectSize.width / step, objectSize.height / step);
	cv::Mat smallObject = cv::Mat::zeros(smallObjectSize, CV_8UC1);

	for (int y = 0; y < smallObject.rows; y++) {
		for (int x = 0; x < smallObject.cols; x++) {
			smallObject.at < uchar > (y, x) = getNPointValue(newObject, cv::Point(step * x, step * y));
		}
	}

	return AbstractContourModel::encodeSingleObject(smallObject);
}

ShapeRepresentationModels::ResamplingChainCode::ContourRepresentation::ContourRepresentation(bool use4pin, size_t resamplingLevel)
{
	this->use4pin = use4pin;
	this->resamplingLevel = resamplingLevel;
}

void ShapeRepresentationModels::ResamplingChainCode::ContourRepresentation::initFromPoints(const std::vector < cv::Point > &points)
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

std::vector < cv::Point > ShapeRepresentationModels::ResamplingChainCode::ContourRepresentation::convert2Points()
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
	int step = (int) pow(2, resamplingLevel);
	cv::Point realFirstPoint = cv::Point(
		firstPoint.x * step,
		firstPoint.y * step
	);
	result.push_back(realFirstPoint);
	cv::Point currentPoint = realFirstPoint;
	int chainCodesCount = chainCodes.size();
	for (int i = 0; i < chainCodesCount; i++) {
		for (int j = 0; j < step; j++) {
			currentPoint.x -= chainCodesInvMap8[chainCodes[i]][1];
			currentPoint.y += chainCodesInvMap8[chainCodes[i]][0];
			result.push_back(currentPoint);
		}
	}

	return result;
}
