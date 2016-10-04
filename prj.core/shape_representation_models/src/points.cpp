/**
    https://github.com/BeamOfLight/shape_representation_models.git
    points.cpp

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04
*/

#include <shape_representation_models/points.h>

ShapeRepresentationModels::Points::Points(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize)
: AbstractContourModel (contoursCountSize, pointsCountSize, pointRepresentationSize)
{
}

std::string ShapeRepresentationModels::Points::getMethodName()
{
	return std::string("Points");
}

ShapeRepresentationModels::AbstractContourModel::AbstractContourRepresentation* ShapeRepresentationModels::Points::getNewContourRepresentation()
{
	return new ContourRepresentation();
}

int ShapeRepresentationModels::Points::getObjectRepresentationSize(ShapeRepresentationModels::AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
	int pointsCount = static_cast < ContourRepresentation* > (objectRepresentation->data.outterContour)->points.size();
	int holesCount = objectRepresentation->data.innerContours.size();
	for (int contourId = 0; contourId < holesCount; contourId++) {
		pointsCount += static_cast < ContourRepresentation* > (objectRepresentation->data.innerContours[contourId])->points.size();
	}
	float resultInBits = (float) (pointRepresentationSize * pointsCount + pointsCountSize * (holesCount + 1) + contoursCountSize);

	return (int) std::ceil(resultInBits / 8);
}

void ShapeRepresentationModels::Points::ContourRepresentation::initFromPoints(std::vector < cv::Point > const &points)
{
	this->points = std::vector < cv::Point > (points);
}

std::vector < cv::Point > ShapeRepresentationModels::Points::ContourRepresentation::convert2Points()
{
	std::vector < cv::Point > result = std::vector < cv::Point > (points);

	return result;
}
