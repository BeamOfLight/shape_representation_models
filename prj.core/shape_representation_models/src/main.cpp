#include <ctime>
#include <omp.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include <dpcore/common.h>
#include <dpcore/filesystem.h>
#include <shape_representation_models/shape_representation_experimental_stand.h>
#include <shape_representation_models/configuration.h>
#include <shape_representation_models/pyramid.h>
#include <shape_representation_models/run_length_encoding.h>
#include <shape_representation_models/deflate_model.h>
#include <shape_representation_models/grid.h>
#include <shape_representation_models/grid_n.h>
#include <shape_representation_models/quad_tree.h>
#include <shape_representation_models/compressed_quad_tree.h>
#include <shape_representation_models/points.h>
#include <shape_representation_models/centered_points.h>
#include <shape_representation_models/freeman_chain_code.h>
#include <shape_representation_models/freeman_chain_code_RLE.h>
#include <shape_representation_models/difference_chain_code.h>
#include <shape_representation_models/resampling_chain_code.h>
#include <shape_representation_models/polygon_model.h>
#include <shape_representation_models/fourier.h>

#define CONTOURS_COUNT_SIZE 8
#define POINTS_COUNT_SIZE 16
#define SINGLE_POINT_SIZE 24

std::vector < std::vector < cv::Point > > getAreaContours(cv::Mat areaImage, size_t minContourSize)
{
	std::vector < std::vector < cv::Point > > contours;
	std::vector < std::vector < cv::Point > > resultContours;
	std::vector < cv::Vec4i > hierarchy;
		
	findContours(areaImage, contours, hierarchy, CV_RETR_TREE , CV_CHAIN_APPROX_NONE, cv::Point(0, 0));
	
	size_t contours_cnt = contours.size();
	for( size_t contour_id = 0; contour_id < contours_cnt; contour_id++ ) {
		if (contours[contour_id].size() > minContourSize) {
			resultContours.push_back(contours[contour_id]);
		}
	}
	
	return resultContours;
}

std::vector < cv::Point > processImage(const char* fname)
{
	cv::Mat image = cv::imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
	std::vector < std::vector < cv::Point > > srcContours;
	std::vector < cv::Vec4i > hierarchy;
	findContours(image, srcContours, hierarchy, CV_RETR_TREE , CV_CHAIN_APPROX_NONE, cv::Point(0, 0));
	image.release();
	
	return srcContours[1];
}

std::vector < cv::Mat > getAllObjects(const std::string &prefix, const std::string &suffix, int first_image_id, int last_image_id)
{
	std::vector < cv::Mat > objects;
	cv::Mat tmp;
	for (int image_id = first_image_id; image_id <= last_image_id; image_id++) {
		tmp = cv::imread((prefix + DpCore::Common::toString(image_id) + suffix).c_str(), 0);
		if (!tmp.empty()) {
			objects.push_back(tmp.clone());
		}
	}

	return objects;
}

ShapeRepresentationModels::AbstractModel* getModelByName(std::string modelName)
{
	ShapeRepresentationModels::AbstractModel* model;
	
	if (modelName == "Points") {
		model = new ShapeRepresentationModels::Points(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE);
	} else if (modelName == "CenteredPoints") {
		model = new ShapeRepresentationModels::CenteredPoints(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE);
	} else if (modelName == "FCC4") {
		model = new ShapeRepresentationModels::FreemanChainCode(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, true);
	} else if (modelName == "FCC8") {
		model = new ShapeRepresentationModels::FreemanChainCode(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, false);
	} else if (modelName == "FCC8+RLE(2)") {
		model = new ShapeRepresentationModels::FreemanChainCodeRLE(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, false, 2);
	} else if (modelName == "FCC8+RLE(5)") {
		model = new ShapeRepresentationModels::FreemanChainCodeRLE(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, false, 5);
	} else if (modelName == "FCC4+RLE(2)") {
		model = new ShapeRepresentationModels::FreemanChainCodeRLE(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, true, 2);
	} else if (modelName == "FCC4+RLE(6)") {
		model = new ShapeRepresentationModels::FreemanChainCodeRLE(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, true, 6);
	} else if (modelName == "DCC4") {
		model = new ShapeRepresentationModels::DifferenceChainCode(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, true);
	} else if (modelName == "DCC8") {
		model = new ShapeRepresentationModels::DifferenceChainCode(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, false);
	} else if (modelName == "RCC4(2)") {
		model = new ShapeRepresentationModels::ResamplingChainCode(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, true, 2);
	} else if (modelName == "RCC8(2)") {
		model = new ShapeRepresentationModels::ResamplingChainCode(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, false, 2);
	} else if (modelName == "RCC4(4)") {
		model = new ShapeRepresentationModels::ResamplingChainCode(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, true, 4);
	} else if (modelName == "RCC8(4)") {
		model = new ShapeRepresentationModels::ResamplingChainCode(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, false, 4);
	} else if (modelName == "Polygon(1.0)") {
		model = new ShapeRepresentationModels::PolygonModel(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, 1);
	} else if (modelName == "Polygon(2.0)") {
		model = new ShapeRepresentationModels::PolygonModel(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, 2);
	} else if (modelName == "Polygon(3.0)") {
		model = new ShapeRepresentationModels::PolygonModel(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, 3);
	} else if (modelName == "Polygon(4.0)") {
		model = new ShapeRepresentationModels::PolygonModel(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, 4);
	} else if (modelName == "Polygon(5.0)") {
		model = new ShapeRepresentationModels::PolygonModel(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, 5);
	} else if (modelName == "Grid") {
		model = new ShapeRepresentationModels::Grid(SINGLE_POINT_SIZE);
	} else if (modelName == "GridN(2)") {
		model = new ShapeRepresentationModels::GridN(SINGLE_POINT_SIZE, 2);
	} else if (modelName == "GridN(3)") {
		model = new ShapeRepresentationModels::GridN(SINGLE_POINT_SIZE, 3);
	} else if (modelName == "GridN(4)") {
		model = new ShapeRepresentationModels::GridN(SINGLE_POINT_SIZE, 4);
	} else if (modelName == "Fourier(10)") {
		model = new ShapeRepresentationModels::Fourier(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, 10);
	} else if (modelName == "Fourier(15)") {
		model = new ShapeRepresentationModels::Fourier(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, 15);
	} else if (modelName == "Fourier(20)") {
		model = new ShapeRepresentationModels::Fourier(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, 20);
	} else if (modelName == "Fourier(30)") {
		model = new ShapeRepresentationModels::Fourier(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, 30);
	} else if (modelName == "Fourier(40)") {
		model = new ShapeRepresentationModels::Fourier(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, 40);
	} else if (modelName == "Fourier(50)") {
		model = new ShapeRepresentationModels::Fourier(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, 50);
	} else if (modelName == "Fourier(100)") {
		model = new ShapeRepresentationModels::Fourier(CONTOURS_COUNT_SIZE, POINTS_COUNT_SIZE, SINGLE_POINT_SIZE, 100);
	} else if (modelName == "QuadTree") {
		model = new ShapeRepresentationModels::QuadTree(SINGLE_POINT_SIZE);
	} else if (modelName == "Comp. QuadTree") {
		model = new ShapeRepresentationModels::CompressedQuadTree(SINGLE_POINT_SIZE);
	} else if (modelName == "Pyramid(2)") {
		model = new ShapeRepresentationModels::Pyramid(SINGLE_POINT_SIZE, 2);
	} else if (modelName == "Pyramid(3)") {
		model = new ShapeRepresentationModels::Pyramid(SINGLE_POINT_SIZE, 3);
	} else if (modelName == "Pyramid(4)") {
		model = new ShapeRepresentationModels::Pyramid(SINGLE_POINT_SIZE, 4);
	} else if (modelName == "RLE") {
		model = new ShapeRepresentationModels::RunLengthEncoding(SINGLE_POINT_SIZE);
	} else if (modelName.length() >= 14 && modelName.substr(0,7) == "Deflate") {
		int compressionLevel = atoi(modelName.substr(12,1).c_str());
		bool biLevel = modelName.substr(13,1) == "B";
		std::string strategyStr = modelName.substr(8,3);
		if (strategyStr == "Def") {
			model = new ShapeRepresentationModels::DeflateModel(SINGLE_POINT_SIZE, compressionLevel, cv::IMWRITE_PNG_STRATEGY_DEFAULT, biLevel);
		} else if (strategyStr == "Fil") {
			model = new ShapeRepresentationModels::DeflateModel(SINGLE_POINT_SIZE, compressionLevel, cv::IMWRITE_PNG_STRATEGY_FILTERED, biLevel);
		} else if (strategyStr == "Huf") {
			model = new ShapeRepresentationModels::DeflateModel(SINGLE_POINT_SIZE, compressionLevel, cv::IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY, biLevel);
		} else if (strategyStr == "RLE") {
			model = new ShapeRepresentationModels::DeflateModel(SINGLE_POINT_SIZE, compressionLevel, cv::IMWRITE_PNG_STRATEGY_RLE, biLevel);
		} else if (strategyStr == "Fix") {
			model = new ShapeRepresentationModels::DeflateModel(SINGLE_POINT_SIZE, compressionLevel, cv::IMWRITE_PNG_STRATEGY_FIXED, biLevel);
		} else {
			model = 0;
			std::cout << "Cannot found Deflate model '" << modelName.c_str() << "'" << std::endl;
		}
	} else {
		model = 0;
		std::cout << "Cannot found model '" << modelName.c_str() << "'" << std::endl;
	}
	
	return model;
}
 
std::vector < ShapeRepresentationModels::AbstractModel* > initModels(ShapeRepresentationModels::Configuration* configuration)
{
	std::vector < ShapeRepresentationModels::AbstractModel* > models;
	int modelsCount = configuration->modelNames.size();
	for (int i = 0; i < modelsCount; i ++) {
		ShapeRepresentationModels::AbstractModel* model = getModelByName(configuration->modelNames[i]);

		if (model) {
			models.push_back(model);
		}
	}

	return models;
}
 
ShapeRepresentationModels::Configuration* getConfigurationByFilename(std::string configFilename)
{
	ShapeRepresentationModels::Configuration* configuration = new ShapeRepresentationModels::Configuration();
	cv::FileStorage fs(configFilename.c_str(), cv::FileStorage::READ);
	
	cv::FileNode modelsSeq = fs["Models"];
	cv::FileNodeIterator modelIt = modelsSeq.begin(), modelItEnd = modelsSeq.end();
	for ( ; modelIt != modelItEnd; ++modelIt) {
		configuration->modelNames.push_back(
			(std::string) (*modelIt)
		);
	}
	
	configuration->firstImageId = fs["FirstImageId"];
	configuration->lastImageId = fs["LastImageId"];
	configuration->showImagesFlag = fs["ShowImagesFlag"];
	configuration->showNotZeroDiffCounter = fs["ShowNotZeroDiffCounter"];
	configuration->showImageIds = fs["ShowImageIds"];
	configuration->enableMultithreading = ((int) fs["EnableMultithreading"] > 0);
	configuration->threadsCount = 1;
	if (configuration->enableMultithreading) {
		configuration->threadsCount = omp_get_max_threads();
	}
	
	return configuration;
}

void showConfiguration(ShapeRepresentationModels::Configuration* configuration)
{
	std::cout << "Configuration" << std::endl
		<< "FirstImageId:\t\t\t" << configuration->firstImageId << std::endl
		<< "LastImageId:\t\t\t" << configuration->lastImageId << std::endl
		<< "ShowImagesFlag:\t\t\t" << configuration->showImagesFlag << std::endl
		<< "ShowNotZeroDiffCounter:\t\t" << configuration->showNotZeroDiffCounter << std::endl
		<< "ShowImageIds:\t\t\t" << configuration->showImageIds << std::endl
		<< "ThreadsCount:\t\t\t" << configuration->threadsCount << std::endl;

	int modelsCount = configuration->modelNames.size();
	std::cout << "Models(" << modelsCount << "):\t\t\t";	
	for(int i = 0; i < modelsCount; i ++) {
		std::cout << configuration->modelNames[i] << "\t";	
	}
	std::cout << std::endl << std::endl;	
}

int main(int argc, char** argv) 
{
	// Load configuration
	cv::FileStorage config_fs = DpCore::Common::loadConfig(argc, argv, "config/shape_representation_models.yml");
	std::string filename = DpCore::Filesystem::getCrossPlatformFileName((std::string) config_fs["config"]);
	config_fs.release();

	// Show configuration
	std::cout << "Configuration" << std::endl << "config: " << filename << std::endl
		<< "omp_get_max_threads: " << omp_get_max_threads() << std::endl
		<< std::endl;

	// File checking
	if (!DpCore::Filesystem::isFileExist(filename)) {
		std::cerr << "File does not exist: " << filename << std::endl;
		throw;
	}

	std::string configFilename = std::string(filename);
	std::cout << "Loading configuration from '" << configFilename.c_str() << "' ...\t";
	ShapeRepresentationModels::Configuration* configuration = getConfigurationByFilename(configFilename);

	std::cout << "OK" << std::endl;
	showConfiguration(configuration);
	
	// Подгрузка силуэтов
	std::string prefix = DpCore::Filesystem::getRootProjectDirectory() + "data/silhouettes-database/images/a";
	std::string suffix = ".bmp";
	std::cout << "Image path sample: " << prefix << DpCore::Common::toString(1) << suffix << std::endl;

	std::cout << "Loading objects...\t\t";
	std::vector < cv::Mat > objects = getAllObjects(prefix, suffix, configuration->firstImageId, configuration->lastImageId);
	std::cout << objects.size() << std::endl;

	if (!objects.size()) {
		std::cout << "Need more objects!" << std::endl;
		return 1;
	}
	
	// Инициализация моделей способов представления силуэтов
	std::cout << "Models initialization...\t";
	std::vector < ShapeRepresentationModels::AbstractModel* > models = initModels(configuration);
	std::cout << models.size() << std::endl << "Start";
	
	ShapeRepresentationModels::ShapeRepresentationExperimentalStand* stand = new ShapeRepresentationModels::ShapeRepresentationExperimentalStand(models, objects, configuration);
	std::cout << "...\t";
	float processingTimeInSeconds = stand->start();
	std::cout << "OK" << std::endl;
	
	std::cout << std::endl << "====================================CONTOUR====================================";
	stand->show(ShapeRepresentationModels::AbstractModel::MethodType::CONTOUR);
	
	std::cout << std::endl << "=====================================AREA======================================";
	stand->show(ShapeRepresentationModels::AbstractModel::MethodType::AREA);
	
	std::cout << std::endl << "====================================OTHER======================================";
	stand->show(ShapeRepresentationModels::AbstractModel::MethodType::OTHER);
	
	std::cout << "Processing time: " << processingTimeInSeconds << " seconds" << std::endl;
	getchar();

	return 0;
}
