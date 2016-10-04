/**
	https://github.com/BeamOfLight/shape_representation_models.git
    shape_representation_experimantal_stand.cpp

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04 
*/

#include <shape_representation_models/shape_representation_experimental_stand.h>

ShapeRepresentationModels::ShapeRepresentationExperimentalStand::ShapeRepresentationExperimentalStand(
	std::vector < AbstractModel* > models,
	std::vector < cv::Mat > srcObjects,
	Configuration* configuration
)
{
	this->models = models;
	this->srcObjects = srcObjects;
	this->configuration = configuration;
}

cv::Mat ShapeRepresentationModels::ShapeRepresentationExperimentalStand::normalizeBinaryImage(cv::Mat image)
{
	cv::Mat result = cv::Mat::zeros(image.size(), CV_8UC1);
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			result.at<uchar>(cv::Point(x,y)) = (image.at<uchar>(cv::Point(x,y)) ? 255 : 0);
		}
	}
	
	return result;
}

float ShapeRepresentationModels::ShapeRepresentationExperimentalStand::compare(cv::Mat object1, cv::Mat object2, int objectId)
{
	if (configuration->showImagesFlag) {
		DpCore::Common::showBinaryImage("A", object1, cv::Point(50, 50));
		DpCore::Common::showBinaryImage("B", object2, cv::Point(500, 50));
		cv::waitKey();
	}
	
	object1 = DpCore::Common::normalizeBinaryImage(object1);
	object2 = DpCore::Common::normalizeBinaryImage(object2);
	cv::Size newSize = cv::Size(
		std::max(object1.cols, object2.cols),
		std::max(object1.rows, object2.rows)
	);

	if (object1.cols < newSize.width || object1.rows < newSize.height) {
		cv::Mat resizedObject1 = cv::Mat::ones(newSize, CV_8UC1) * 255;
		for (int y = 0; y < object1.rows; y++) {
			for (int x = 0; x < object1.cols; x++) {
				resizedObject1.at < uchar > (cv::Point(x,y)) = object1.at < uchar > (cv::Point(x,y));
			}
		}
		resizedObject1.copyTo(object1);	
	}
	
	if (object2.cols < newSize.width || object2.rows < newSize.height) {
		cv::Mat resizedObject2 = cv::Mat::ones(newSize, CV_8UC1) * 255;
		for (int y = 0; y < object2.rows; y++) {
			for (int x = 0; x < object2.cols; x++) {
				resizedObject2.at < uchar > (cv::Point(x,y)) = object2.at < uchar > (cv::Point(x,y));
			}
		}
		resizedObject2.copyTo(object2);	
	}
	
	cv::Mat diffImage = cv::Mat::zeros(object1.size(), CV_8UC1);
	cv::bitwise_xor(object1, object2, diffImage);
	
	if (configuration->showImagesFlag) {
		DpCore::Common::showImage("Diff", diffImage, cv::Point(900, 50)); cv::waitKey();
	}
	//imwrite("B.bmp", object2);
	
	int diffPointsCounter = 0;
	for (int y = 0; y < diffImage.rows; y++) {
		for (int x = 0; x < diffImage.cols; x++) {
			if (diffImage.at<uchar>(cv::Point(x,y))) {
				diffPointsCounter++;
			}
		}
	}
	
	int initAreaCounter = 0;
	for (int y = 0; y < object1.rows; y++) {
		for (int x = 0; x < object1.cols; x++) {
			if (!object1.at<uchar>(cv::Point(x,y))) {
				initAreaCounter++;
			}
		}
	}
	
	if (configuration->showNotZeroDiffCounter && diffPointsCounter > 0) {
		std::cout << "[" << objectId << "] DiffPointsCounter: " << diffPointsCounter << " / " << initAreaCounter << std::endl;
	}
	
	return (float) diffPointsCounter * 100 / (float) initAreaCounter;
}

float ShapeRepresentationModels::ShapeRepresentationExperimentalStand::getTimeInSeconds(clock_t time)
{
	return (float) time / CLOCKS_PER_SEC;
}

int ShapeRepresentationModels::ShapeRepresentationExperimentalStand::getModelRepresentationSize(int model_id, std::vector < ShapeRepresentationModels::AbstractModel::AbstractRepresentation* > encodedObjects)
{
	int representationSize = 0;
	int objects_count = srcObjects.size();
	for (int object_id = 0; object_id < objects_count; object_id++) {
		representationSize+= models[model_id]->getObjectRepresentationSize(encodedObjects[object_id]);
	}
	
	return representationSize;
}

ShapeRepresentationModels::SummaryStat ShapeRepresentationModels::ShapeRepresentationExperimentalStand::getAreaDiffSummaryStat(int modelId, std::vector < cv::Mat > const &dstObjects)
{
	SummaryStat areaDiffSummaryStat;
	areaDiffSummaryStat.sum = 0;
	float areaDiff;

	int objectsCount = srcObjects.size();
	std::vector <float> areaDiffs;
	areaDiffs.reserve(objectsCount);
	for (int objectId = 0; objectId < objectsCount; objectId++) {
		areaDiff = compare(srcObjects[objectId], dstObjects[objectId], objectId);
		areaDiffSummaryStat.sum += areaDiff;
		areaDiffs.push_back(areaDiff);
	}
	
	areaDiffSummaryStat.avg = areaDiffSummaryStat.sum / objectsCount;
	areaDiffSummaryStat.min = areaDiffs[0];
	areaDiffSummaryStat.max = areaDiffs[0];
	
	// TODO расчитать
	areaDiffSummaryStat.sigma = -1;
	
	for (int objectId = 0; objectId < objectsCount; objectId++) {
		areaDiffSummaryStat.min = std::min(areaDiffSummaryStat.min, areaDiffs[objectId]);
		areaDiffSummaryStat.max = std::max(areaDiffSummaryStat.max, areaDiffs[objectId]);
	}
	
	return areaDiffSummaryStat;
}

void ShapeRepresentationModels::ShapeRepresentationExperimentalStand::startForSingleModel(int modelId)
{
	ModelInfo* modelInfo = new ModelInfo();
	modelInfo->methodName = std::string(models[modelId]->getMethodName());

	// Замеряем время кодирования всех контуров
	if (configuration->showImageIds) {
		std::cout << "Encode" << std::endl;
	}
	clock_t startTime = clock();
	std::vector < AbstractModel::AbstractRepresentation* > encodedObjects = encode(modelId, srcObjects);
	modelInfo->encodeTime = getTimeInSeconds(clock() - startTime);
	
	// Расчет количественных характеристик
	modelInfo->representationSize = getModelRepresentationSize(modelId, encodedObjects);

	// Замеряем время восстановления всех описаний
	if (configuration->showImageIds) {
		std::cout << "Decode" << std::endl;
	}
	startTime = clock();
	std::vector < cv::Mat > dstObjects = decode(modelId, encodedObjects);
	modelInfo->decodeTime = getTimeInSeconds(clock() - startTime);
	
	// Расчет качественных характеристик
	if (configuration->showImageIds) {
		std::cout << "Diff" << std::endl;
	}
	modelInfo->areaDiff = getAreaDiffSummaryStat(modelId, dstObjects);
	modelsInfo.push_back(modelInfo);
}

float ShapeRepresentationModels::ShapeRepresentationExperimentalStand::start()
{
	clock_t startTime = clock();
	int modelsCount = models.size();
	for (int modelId = 0; modelId < modelsCount; modelId++) {
		if (configuration->showImageIds || configuration->showImagesFlag) {
			std::cout << "[" << modelId << "] " << configuration->modelNames[modelId] << std::endl;
		}
		startForSingleModel(modelId);
	}
	
	return getTimeInSeconds(clock() - startTime);
}

std::vector < ShapeRepresentationModels::ModelInfo* > ShapeRepresentationModels::ShapeRepresentationExperimentalStand::getModelsInfo()
{
	return modelsInfo;
}

void ShapeRepresentationModels::ShapeRepresentationExperimentalStand::show(int methodType)
{
	int modelsCount = modelsInfo.size();
	printf("\n|     Method      |  size   | encTime | decTime |          areaDiff           |");
	printf("\n|                 |  bytes  |   sec   |   sec   |  min %%  |  max %%  |  avg %%  |");
	printf("\n===============================================================================\n");
	for (int modelId = 0; modelId < modelsCount; modelId++) {
		if (models[modelId]-> getMethodType() == methodType) {
			printf(
				"| %-15s | %7i | %7.2f | %7.2f | %7.3f | %7.3f | %7.3f |\n",
				modelsInfo[modelId]->methodName.c_str(),
				modelsInfo[modelId]->representationSize,
				modelsInfo[modelId]->encodeTime,
				modelsInfo[modelId]->decodeTime,
				modelsInfo[modelId]->areaDiff.min,
				modelsInfo[modelId]->areaDiff.max,
				modelsInfo[modelId]->areaDiff.avg
			);
		}
	}
	printf("===============================================================================\n");
}

void ShapeRepresentationModels::ShapeRepresentationExperimentalStand::encodeObjectThreadFunc(std::pair < int, int > const &params)
{
	
	int modelId = params.first;
	int threadId = params.second;
	int objectsCount = srcThreadObjects.size();

	for (int objectId = threadId; objectId < objectsCount; objectId += configuration->threadsCount) {
		encodedThreadObjects[objectId] = models[modelId]->encodeSingleObject(srcThreadObjects[objectId]);
	}

	if (threadStatus.size()) {
		threadStatus[threadId] = 1;
	}
}

void ShapeRepresentationModels::ShapeRepresentationExperimentalStand::decodeObjectThreadFunc(std::pair < int, int > const &params)
{
	int modelId = params.first;
	int threadId = params.second;
	int objectsCount = decodedThreadObjects.size();

	for (int objectId = threadId; objectId < objectsCount; objectId += configuration->threadsCount) {
		decodedThreadObjects[objectId] = models[modelId]->decodeSingleObject(encodedThreadObjects[objectId]);
	}

	if (threadStatus.size()) {
		threadStatus[threadId] = 1;
	}
}

std::vector < ShapeRepresentationModels::AbstractModel::AbstractRepresentation* > ShapeRepresentationModels::ShapeRepresentationExperimentalStand::encode(int modelId, std::vector < cv::Mat > objects)
{
	int objectsCount = objects.size();
	srcThreadObjects.clear();
	encodedThreadObjects.clear();
	encodedThreadObjects.reserve(objectsCount);
	for (int objectId = 0; objectId < objectsCount; objectId++) {
		srcThreadObjects.push_back(objects[objectId]);
		encodedThreadObjects.push_back(0);
	}

	// Запуск нескольких потоков
	threadStatus.clear();
	if (configuration->threadsCount == 1) {
		encodeObjectThreadFunc(std::make_pair(modelId, 0));
	} else {
		for (int threadId = 0; threadId < configuration->threadsCount; threadId++) {
			threadStatus.push_back(0);
		}

		#pragma omp parallel for
		for (int threadId = 0; threadId < configuration->threadsCount; threadId++) {
			encodeObjectThreadFunc(std::make_pair(modelId, threadId));
		}
	}
	
	return encodedThreadObjects;
}

std::vector < cv::Mat > ShapeRepresentationModels::ShapeRepresentationExperimentalStand::decode(int modelId, std::vector < AbstractModel::AbstractRepresentation* > encodedObjects)
{
	int objectsCount = encodedObjects.size();
	encodedThreadObjects.clear();
	decodedThreadObjects.clear();
	decodedThreadObjects.reserve(objectsCount);
	cv::Mat emptyMat;
	for (int objectId = 0; objectId < objectsCount; objectId++) {
		encodedThreadObjects.push_back(encodedObjects[objectId]);
		decodedThreadObjects.push_back(emptyMat);
	}

	// Запуск нескольких потоков
	threadStatus.clear();
	if (configuration->threadsCount == 1) {
		decodeObjectThreadFunc(std::make_pair(modelId, 0));
	} else {
		for (int threadId = 0; threadId < configuration->threadsCount; threadId++) {
			threadStatus.push_back(0);
		}

		#pragma omp parallel for
		for (int threadId = 0; threadId < configuration->threadsCount; threadId++) {
			decodeObjectThreadFunc(std::make_pair(modelId, threadId));
		}
	}

	return decodedThreadObjects;
}