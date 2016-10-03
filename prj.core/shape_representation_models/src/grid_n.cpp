#include <shape_representation_models/grid_n.h>

ShapeRepresentationModels::GridN::GridN(size_t pointRepresentationSize, size_t step)
{
	this->pointRepresentationSize = pointRepresentationSize;
	this->step = step;
}

std::string ShapeRepresentationModels::GridN::getMethodName()
{
	std::stringstream ss;
	ss << "GridN(" << step << ")";
	
	return ss.str();
}

// Сохраняем маску + данные о ее размере и положении + 1 байт на step
int ShapeRepresentationModels::GridN::getObjectRepresentationSize(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
	int headerSize = 2 * pointRepresentationSize + 8;
	float resultInBits = (float) (objectRepresentation->mask.rows * objectRepresentation->mask.cols + headerSize);
	
	return (int) ceil(resultInBits / 8);
}

unsigned char ShapeRepresentationModels::GridN::getNPointValue(const cv::Mat &object, cv::Point pt)
{
	size_t cnt = 0;
	for (size_t y = 0; y < step; y++) {
		for (size_t x = 0; x < step; x++) {
			if (object.at < uchar > (cv::Point(pt.y + y, pt.x + x))) {
				cnt++;
			}
		}
	}
	
	return (step * step <= 2 * cnt) ? 255 : 0;
}

void ShapeRepresentationModels::GridN::setNPointValue(cv::Mat &object, cv::Point pt, unsigned char value)
{
	for (size_t y = 0; y < step; y++) {
		for (size_t x = 0; x < step; x++) {
			object.at < uchar > (cv::Point(pt.y + y, pt.x + x)) = value;
		}
	}
}

ShapeRepresentationModels::AbstractModel::AbstractRepresentation* ShapeRepresentationModels::GridN::encodeSingleObject(const cv::Mat &object)
{
	Representation* objectRepresentation = new Representation();
	std::vector < cv::Point > contour = getImageContour(object);
	cv::Rect rect = boundingRect(contour);
	objectRepresentation->rect = cv::Rect(rect.x + 1, rect.y + 1, rect.width - 2, rect.height - 2);
	
	cv::Point ptA = cv::Point(objectRepresentation->rect.x, objectRepresentation->rect.y);
	cv::Point ptB = cv::Point(
		objectRepresentation->rect.x + objectRepresentation->rect.width,
		objectRepresentation->rect.y + objectRepresentation->rect.height
	);

	ptA.x = step * (ptA.x / step);
	ptA.y = step * (ptA.y / step);
	ptB.x = step * ((int) ceil((float) ptB.x / step)) - 1;
	ptB.y = step * ((int) ceil((float) ptB.y / step)) - 1;
	objectRepresentation->rect = cv::Rect(ptA.x, ptA.y, ptB.x - ptA.x, ptB.y - ptA.y);
	
	cv::Size maskSize = cv::Size(
		(ptB.x - ptA.x) / step,
		(ptB.y - ptA.y) / step
	);
	objectRepresentation->mask = cv::Mat::zeros(maskSize, CV_8UC1);
	for (int y = 0; y < objectRepresentation->mask.rows; y++) {
		for (int x =0; x < objectRepresentation->mask.cols; x++) {
			objectRepresentation->mask.at < uchar > (y, x) = getNPointValue(object, cv::Point(ptA.y + step * y, ptA.x + step * x));
		}
	}
	
	return objectRepresentation;
}

cv::Mat ShapeRepresentationModels::GridN::decodeSingleObject(ShapeRepresentationModels::AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
	cv::Size imageSize = cv::Size(
		objectRepresentation->rect.x + objectRepresentation->rect.width,
		objectRepresentation->rect.y + objectRepresentation->rect.height
	);
	cv::Mat result = cv::Mat::ones(imageSize, CV_8UC1);
	
	for (int y = 0; y < objectRepresentation->mask.rows; y++) {
		for (int x = 0; x < objectRepresentation->mask.cols; x++) {
			setNPointValue(
				result,
				cv::Point(
					step * y + objectRepresentation->rect.y,
					step * x + objectRepresentation->rect.x
				),
				objectRepresentation->mask.at < uchar > (y, x)
			);
		}
	}
	
	return result;
}