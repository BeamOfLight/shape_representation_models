#include <shape_representation_models/abstract_contour_model.h>

ShapeRepresentationModels::AbstractContourModel::AbstractContourModel(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize)
{
	this->contoursCountSize = contoursCountSize;
	this->pointsCountSize = pointsCountSize;
	this->pointRepresentationSize = pointRepresentationSize;
}

int ShapeRepresentationModels::AbstractContourModel::getMethodType() const
{
	return AbstractModel::MethodType::CONTOUR;
}

ShapeRepresentationModels::AbstractModel::AbstractRepresentation* ShapeRepresentationModels::AbstractContourModel::encodeSingleObject(const cv::Mat &object)
{	
	Representation* objectRepresentation = new Representation();
	objectRepresentation->data = convertPointRepresentation2EncodedObject(DpCore::Object::convertObject2PointRepresentation(object, 0));
	
	return objectRepresentation;
}

cv::Mat ShapeRepresentationModels::AbstractContourModel::decodeSingleObject(ShapeRepresentationModels::AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);

	return DpCore::Object::convertPointRepresentation2Object(convertEncodedObject2PointRepresentation(objectRepresentation->data));
}

DpCore::ObjectRepresentation < ShapeRepresentationModels::AbstractContourModel::AbstractContourRepresentation* > ShapeRepresentationModels::AbstractContourModel::convertPointRepresentation2EncodedObject(const DpCore::ObjectRepresentation < std::vector < cv::Point > > &pointRepresentation)
{
	DpCore::ObjectRepresentation < AbstractContourModel::AbstractContourRepresentation* > encodedObject;
	encodedObject.outterContour = getNewContourRepresentation();
	encodedObject.outterContour->initFromPoints(pointRepresentation.outterContour);
	for (size_t contour_id = 0; contour_id < pointRepresentation.innerContours.size(); contour_id++) {
		encodedObject.innerContours.push_back(getNewContourRepresentation());
		encodedObject.innerContours.back()->initFromPoints(pointRepresentation.innerContours[contour_id]);
	}
	
	return encodedObject;
}

DpCore::ObjectRepresentation < std::vector < cv::Point > > ShapeRepresentationModels::AbstractContourModel::convertEncodedObject2PointRepresentation(DpCore::ObjectRepresentation < ShapeRepresentationModels::AbstractContourModel::AbstractContourRepresentation* > encodedObject)
{
	DpCore::ObjectRepresentation < std::vector < cv::Point > > pointRepresentation;
	pointRepresentation.outterContour = encodedObject.outterContour->convert2Points();
	for (size_t contourId = 0; contourId < encodedObject.innerContours.size(); contourId++) {
		pointRepresentation.innerContours.push_back(
			encodedObject.innerContours[contourId]->convert2Points()
		);
	}
	
	return pointRepresentation;
}

