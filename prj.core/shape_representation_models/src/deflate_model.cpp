/**
    https://github.com/BeamOfLight/shape_representation_models.git
    deflate_model.cpp

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04
*/

#include <shape_representation_models/deflate_model.h>

ShapeRepresentationModels::DeflateModel::DeflateModel(size_t pointRepresentationSize, size_t compressionLevel, size_t strategy, bool biLevel)
{
	this->pointRepresentationSize = pointRepresentationSize;
	this->compressionLevel = compressionLevel;
	this->strategy = strategy;
	this->biLevel = biLevel;
}

std::string ShapeRepresentationModels::DeflateModel::getMethodName()
{
	// https://ru.wikipedia.org/wiki/Deflate
	std::string biletter = "";
	if (biLevel) {
		biletter = "B";
	}

	std::string strategyStr = "";
	switch (strategy) {
		case cv::IMWRITE_PNG_STRATEGY_DEFAULT:
			strategyStr = "Def";
			break;
		case cv::IMWRITE_PNG_STRATEGY_FILTERED:
			strategyStr = "Fil";
			break;
		case cv::IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY:
			strategyStr = "Huf";
			break;
		case cv::IMWRITE_PNG_STRATEGY_RLE :
			strategyStr = "RLE";
			break;
		case cv::IMWRITE_PNG_STRATEGY_FIXED :
			strategyStr = "Fix";
			break;
	}

	std::stringstream ss;
	ss << "Deflate-" << strategyStr << "(" << compressionLevel << biletter << ")";

	return ss.str();
}

int ShapeRepresentationModels::DeflateModel::getObjectRepresentationSize(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);

	return objectRepresentation->buffer.size();
}

ShapeRepresentationModels::AbstractModel::AbstractRepresentation* ShapeRepresentationModels::DeflateModel::encodeSingleObject(const cv::Mat &object)
{
	Representation* objectRepresentation = new Representation();
	std::vector < int > compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(compressionLevel);
	compression_params.push_back(CV_IMWRITE_PNG_STRATEGY);
	compression_params.push_back(strategy);
	compression_params.push_back(CV_IMWRITE_PNG_BILEVEL );
	compression_params.push_back((int) biLevel);

	cv::imencode(".png", object, objectRepresentation->buffer, compression_params);

	return objectRepresentation;
}

cv::Mat ShapeRepresentationModels::DeflateModel::decodeSingleObject(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
	cv::Mat result = cv::imdecode(objectRepresentation->buffer, CV_LOAD_IMAGE_GRAYSCALE);

	return result;
}
