/**
    https://github.com/BeamOfLight/shape_representation_models.git
    grid.cpp

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04
*/

#include <shape_representation_models/grid.h>

ShapeRepresentationModels::Grid::Grid(size_t pointRepresentationSize)
{
	this->pointRepresentationSize = pointRepresentationSize;
}

std::string ShapeRepresentationModels::Grid::getMethodName()
{
	return std::string("Grid");
}

int ShapeRepresentationModels::Grid::getObjectRepresentationSize(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
	int headerSize = 2 * pointRepresentationSize;
	float resultInBits = (float) (objectRepresentation->mask.rows * objectRepresentation->mask.cols + headerSize);

	return (int) ceil(resultInBits / 8);
}

ShapeRepresentationModels::AbstractModel::AbstractRepresentation* ShapeRepresentationModels::Grid::encodeSingleObject(const cv::Mat &object)
{
	Representation* objectRepresentation = new Representation();
	std::vector < cv::Point > contour = getImageContour(object);
	cv::Rect rect = boundingRect(contour);
	objectRepresentation->rect = cv::Rect(rect.x + 1, rect.y + 1, rect.width - 2, rect.height - 2);
	objectRepresentation->mask = cv::Mat(object, objectRepresentation->rect);

	return objectRepresentation;
}

cv::Mat ShapeRepresentationModels::Grid::decodeSingleObject(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
	cv::Size imageSize = cv::Size(
		objectRepresentation->rect.x + objectRepresentation->rect.width,
		objectRepresentation->rect.y + objectRepresentation->rect.height
	);
	cv::Mat result = cv::Mat::ones(imageSize, CV_8UC1);

	for (int y = 0; y < objectRepresentation->mask.rows; y++) {
		for (int x = 0; x < objectRepresentation->mask.cols; x++) {

			result.at<uchar>(
				y + objectRepresentation->rect.y,
				x + objectRepresentation->rect.x
			) = objectRepresentation->mask.at<uchar>(y, x);	

		}
	}

	return result;
}
