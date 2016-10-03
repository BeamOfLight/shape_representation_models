#include <shape_representation_models/polygon_model.h>

ShapeRepresentationModels::PolygonModel::PolygonModel(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize, double epsilon)
: AbstractContourModel (contoursCountSize, pointsCountSize, pointRepresentationSize)
{
	this->epsilon = epsilon;
}

std::string ShapeRepresentationModels::PolygonModel::getMethodName()
{
	std::stringstream ss;
	ss << "Polygon(";
	ss.precision(1);
	ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
	ss << epsilon << ")";
	
	return ss.str();
}

ShapeRepresentationModels::AbstractContourModel::AbstractContourRepresentation* ShapeRepresentationModels::PolygonModel::getNewContourRepresentation()
{
	return new ContourRepresentation(epsilon);
}

int ShapeRepresentationModels::PolygonModel::getObjectRepresentationSize(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
	int verticesCount = static_cast < ContourRepresentation* > (objectRepresentation->data.outterContour)->vertices.size();
	int holesCount = objectRepresentation->data.innerContours.size();
	for (int contourId = 0; contourId < holesCount; contourId++) {
		verticesCount += static_cast < ContourRepresentation* > (objectRepresentation->data.innerContours[contourId])->vertices.size();
	}
	float resultInBits = (float) (pointsCountSize * (holesCount + 1) + verticesCount * pointRepresentationSize + contoursCountSize);

	return (int) ceil(resultInBits / 8);
}

ShapeRepresentationModels::PolygonModel::ContourRepresentation::ContourRepresentation(double epsilon)
{
	this->epsilon = epsilon;
}

void ShapeRepresentationModels::PolygonModel::ContourRepresentation::initFromPoints(const std::vector < cv::Point > &points)
{
	approxPolyDP(points, vertices, epsilon, true);
}

std::vector < cv::Point > ShapeRepresentationModels::PolygonModel::ContourRepresentation::convert2Points()
{	
	return vertices;
}