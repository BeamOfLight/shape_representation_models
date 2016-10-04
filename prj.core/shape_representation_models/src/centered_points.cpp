/**
	https://github.com/BeamOfLight/shape_representation_models.git
    centered_points.cpp

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04 
*/

#include <shape_representation_models/centered_points.h>

ShapeRepresentationModels::CenteredPoints::CenteredPoints(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize)
: AbstractContourModel (contoursCountSize, pointsCountSize, pointRepresentationSize)
{
}

std::string ShapeRepresentationModels::CenteredPoints::getMethodName()
{
	return std::string("CenteredPoints");
}

ShapeRepresentationModels::AbstractContourModel::AbstractContourRepresentation* ShapeRepresentationModels::CenteredPoints::getNewContourRepresentation()
{
	return new ContourRepresentation();
}

int ShapeRepresentationModels::CenteredPoints::getObjectRepresentationSize(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
	int summaryContourSize = static_cast < ContourRepresentation* > (objectRepresentation->data.outterContour)->normalizedContour.size();
	int holesCount = objectRepresentation->data.innerContours.size();
	for (int contourId = 0; contourId < holesCount; contourId++) {
		summaryContourSize += static_cast < ContourRepresentation* > (objectRepresentation->data.innerContours[contourId])->normalizedContour.size();
	}
	
	float resultInBits = (float) ((pointsCountSize + pointRepresentationSize) * (holesCount + 1) + summaryContourSize * pointRepresentationSize + contoursCountSize);

	return (int) ceil(resultInBits / 8);
}

void ShapeRepresentationModels::CenteredPoints::ContourRepresentation::initFromPoints(const std::vector < cv::Point > &points)
{
	int contourLength = points.size();
	
	center = cv::Point(0,0);
	if (contourLength) {
		int xSum = 0;
		int ySum = 0;
	
		for (int i = 0; i < contourLength; i++) {
			xSum += points[i].x;
			ySum += points[i].y;
		}
		
		center = cv::Point(
			DpCore::Common::round((float) xSum / contourLength),
			DpCore::Common::round((float) ySum / contourLength)
		);
	}
	
	normalizedContour.clear();
	for (int i = 0; i < contourLength; i++) {
		normalizedContour.push_back(
			cv::Point(
				points[i].x - center.x,
				points[i].y - center.y
			)
		);
	}	
}

std::vector < cv::Point > ShapeRepresentationModels::CenteredPoints::ContourRepresentation::convert2Points()
{
	std::vector < cv::Point > resultContour;
	int contourLength = normalizedContour.size();
	for (int i = 0; i < contourLength; i++) {
		resultContour.push_back(
			cv::Point(
				center.x + normalizedContour[i].x,
				center.y + normalizedContour[i].y
			)
		);
	}
	
	return resultContour;
}