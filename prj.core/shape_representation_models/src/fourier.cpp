/**
	https://github.com/BeamOfLight/shape_representation_models.git
    fourier.cpp

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04 
*/

#include <shape_representation_models/fourier.h>

ShapeRepresentationModels::Fourier::Fourier(size_t contoursCountSize, size_t pointsCountSize, size_t pointRepresentationSize, int fourierDescriptorSize)
: AbstractContourModel (contoursCountSize, pointsCountSize, pointRepresentationSize)
{
	this->fourierDescriptorSize = fourierDescriptorSize;
}

std::string ShapeRepresentationModels::Fourier::getMethodName()
{
	std::stringstream ss;
	ss << "Fourier(" << fourierDescriptorSize << ")";
	
	return ss.str();
}

ShapeRepresentationModels::AbstractContourModel::AbstractContourRepresentation* ShapeRepresentationModels::Fourier::getNewContourRepresentation()
{
	return new ContourRepresentation(fourierDescriptorSize);
}

int ShapeRepresentationModels::Fourier::getObjectRepresentationSize(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
	int fourierElementSizeInBits = 128; // 4 float по 4 байта
	int fourierMaxElementsPerContour = 256;
	int fourierDescriptorElementsCount = static_cast < ContourRepresentation* > (objectRepresentation->data.outterContour)
		->getResultFourierDescriptorSize();
	int holesCount = objectRepresentation->data.innerContours.size();
	for (int contourId = 0; contourId < holesCount; contourId++) {
		fourierDescriptorElementsCount += static_cast < ContourRepresentation* > (objectRepresentation->data.innerContours[contourId])
			->getResultFourierDescriptorSize();
	}
	
	float resultInBits = (float) (pointsCountSize * (holesCount + 1) + fourierDescriptorElementsCount * fourierElementSizeInBits + contoursCountSize + log2(fourierMaxElementsPerContour));

	return (int) ceil(resultInBits / 8);
}


ShapeRepresentationModels::Fourier::ContourRepresentation::ContourRepresentation(int fourierDescriptorSize)
{
	this->fourierDescriptorSize = fourierDescriptorSize;
}

void ShapeRepresentationModels::Fourier::ContourRepresentation::initFromPoints(const std::vector < cv::Point > &points)
{
	contourLength = points.size();
	cv::Size resultSize = cv::Size(contourLength, 1);
	cv::Mat pointsX = cv::Mat::zeros(resultSize, CV_32F);
	cv::Mat pointsY = cv::Mat::zeros(resultSize, CV_32F);
	for (int i = 0; i < contourLength; i++) {
		pointsX.at < float > (0, i) = (float) points[i].x;
		pointsY.at < float > (0, i) = (float) points[i].y;
	}
	cv::Mat planes[] = {pointsX, pointsY};
		
    cv::Mat complexI;    //Complex plane to contain the DFT coefficients {[0]-Real,[1]-Img}
    cv::merge(planes, 2, complexI);
    cv::dft(complexI, complexI);  // Applying DFT

	fourierDescriptorLeft.clear();
	fourierDescriptorRight.clear();
	int resultFourierDescriptorSize = getResultFourierDescriptorSize();
	for (int i = 0; i < resultFourierDescriptorSize; i++) {
		fourierDescriptorLeft.push_back(
			complexI.at < cv::Vec2f > (0, i)
		);
		fourierDescriptorRight.push_back(
			complexI.at < cv::Vec2f > (0, contourLength - i - 1)
		);
	}
}

int ShapeRepresentationModels::Fourier::ContourRepresentation::getResultFourierDescriptorSize()
{
	int resultFourierDescriptorSize = fourierDescriptorSize;
	if (fourierDescriptorSize > contourLength / 2) {
		resultFourierDescriptorSize = contourLength / 2;
	}
	
	return resultFourierDescriptorSize;
}

std::vector < cv::Point > ShapeRepresentationModels::Fourier::ContourRepresentation::convert2Points()
{
	cv::Size resultSize = cv::Size(contourLength, 1);
	cv::Mat complexI = cv::Mat::zeros(resultSize, CV_32FC2);
	int resultFourierDescriptorSize = getResultFourierDescriptorSize();
	for (int i = 0; i < resultFourierDescriptorSize; i++) {
		complexI.at < cv::Vec2f > (0, i) = fourierDescriptorLeft[i];
		complexI.at < cv::Vec2f > (0, contourLength - i - 1) = fourierDescriptorRight[i];
	}
	
	// Reconstructing original image from the DFT coefficients
    cv::Mat invDFT;
    cv::idft(complexI, invDFT, cv::DFT_SCALE | cv::DFT_COMPLEX_OUTPUT ); // Applying IDFT
	
	std::vector < cv::Point > result;
	for (int i = 0; i < contourLength; i++) {
		result.push_back(
			cv::Point(
				DpCore::Common::round(invDFT.at < float > (0, i * 2)),
				DpCore::Common::round(invDFT.at < float > (0, i * 2 + 1))
			)
		);
	}

	return result;
}