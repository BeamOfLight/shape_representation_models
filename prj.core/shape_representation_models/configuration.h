#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_CONFIGURATION_H
#define SHAPE_REPRESENTATION_MODELS_CONFIGURATION_H

namespace ShapeRepresentationModels {
	struct Configuration
	{
		int firstImageId;
		int lastImageId;
		int showImagesFlag;
		int showNotZeroDiffCounter;
		int threadsCount;
		int showImageIds;
		bool enableMultithreading;
		std::vector < std::string > modelNames;
	};
}

#endif