#include <shape_representation_models/compressed_quad_tree.h>

ShapeRepresentationModels::CompressedQuadTree::CompressedQuadTree(size_t pointRepresentationSize)
{
	this->pointRepresentationSize = pointRepresentationSize;
}

std::string ShapeRepresentationModels::CompressedQuadTree::getMethodName()
{
	return std::string("Comp. QuadTree");
}

int ShapeRepresentationModels::CompressedQuadTree::getObjectRepresentationSize(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
	cv::Vec4i counters = getNodeCounters(objectRepresentation->tree);
	int compressDataSize = getCompressDataSizeCounter(objectRepresentation->tree);
	int bitsPerLink = 16;
	int bitsPerCompressParametersSize = 4;
	int bitsPerEFNode = 2;
	int bitsPerCompressParameter = 2;
	// Сохраняем степень 2 - для одной из сторон нормированного изображения - квадрата
	int bitsPerImageSizeRatio = 12;

	int countEF = counters[0] + counters[1];
	int countP = counters[2];
	int countSummaryCompressParametersSize = counters[3];
	int totalCount = counters[0] + counters[1] + counters[2];
	if (totalCount > pow(2, bitsPerLink - 1)) {
		std::cout << "Too many nodes: " << totalCount << std::endl;
		throw;
	}

	if (countEF == 1 && !countP) {
		// По формату корневой узел нельзя закодировать без представления через дочерние
		countEF = 4;
	}

	int headerSize = pointRepresentationSize + bitsPerImageSizeRatio;
	int nodesEFSize = bitsPerEFNode * countEF;
	int nodesPSize = bitsPerCompressParameter * countSummaryCompressParametersSize + bitsPerLink * (countP - 1) + compressDataSize;
	float resultInBits = (float) (headerSize + nodesEFSize + nodesPSize);

	return (int) ceil(resultInBits / 8);
}

cv::Vec4i ShapeRepresentationModels::CompressedQuadTree::getNodeCounters(CompressedQuadTreeNode* node, cv::Vec4i counters)
{
	counters[node->value]++;
	counters[3] += node->compressParameters.size();
	if (node->value == 2) {
		counters = getNodeCounters(node->topLeft, counters);
		counters = getNodeCounters(node->topRight, counters);
		counters = getNodeCounters(node->bottomLeft, counters);
		counters = getNodeCounters(node->bottomRight, counters);
	}

	return counters;
}

int ShapeRepresentationModels::CompressedQuadTree::getCompressDataSizeCounter(CompressedQuadTreeNode* node, int counter)
{
	if (node->value == 2) {
		int val = node->compressParameters.size();
		if (!val) {
			counter += 1;
		} else if (val >= 1 && val <= 8) {
			counter += 4;
		} else if (val > 8) {
			counter += 7;
		}
	
		counter = getCompressDataSizeCounter(node->topLeft, counter);
		counter = getCompressDataSizeCounter(node->topRight, counter);
		counter = getCompressDataSizeCounter(node->bottomLeft, counter);
		counter = getCompressDataSizeCounter(node->bottomRight, counter);
	}

	return counter;
}

cv::Size ShapeRepresentationModels::CompressedQuadTree::getOptimalSize(int width, int height)
{
	int value = std::max(width, height);
	int newValue = (int) pow(2, ceil(log2(value)));
	
	return cv::Size(newValue, newValue);
}

ShapeRepresentationModels::CompressedQuadTree::CompressedQuadTreeNode* ShapeRepresentationModels::CompressedQuadTree::compressNode(CompressedQuadTree::CompressedQuadTreeNode* node)
{
	if (node->value == 2) {
		if (node->topLeft->value == 2 && !node->topRight->value && !node->bottomLeft->value && !node->bottomRight->value) {
			node = node->topLeft;
			node->compressParameters.push_back(0);
		} else if (!node->topLeft->value && node->topRight->value == 2 && !node->bottomLeft->value && !node->bottomRight->value) {
			node = node->topRight;
			node->compressParameters.push_back(1);
		} else if (!node->topLeft->value && !node->topRight->value && node->bottomLeft->value == 2 && !node->bottomRight->value) {
			node = node->bottomLeft;
			node->compressParameters.push_back(2);
		} else if (!node->topLeft->value && !node->topRight->value && !node->bottomLeft->value && node->bottomRight->value == 2) {
			node = node->bottomRight;
			node->compressParameters.push_back(3);
		}	
	}
	
	return node;
}

void ShapeRepresentationModels::CompressedQuadTree::compress(CompressedQuadTreeNode* tree)
{
	if (tree->value == 2) {
		tree->topLeft = compressNode(tree->topLeft);
		compress(tree->topLeft);
		
		tree->topRight = compressNode(tree->topRight);
		compress(tree->topRight);
		
		tree->bottomLeft = compressNode(tree->bottomLeft);
		compress(tree->bottomLeft);
		
		tree->bottomRight = compressNode(tree->bottomRight);
		compress(tree->bottomRight);
	}
}

void ShapeRepresentationModels::CompressedQuadTree::fillCompressedQuadTree(cv::Mat image, CompressedQuadTreeNode* tree)
{
	double min, max;
	minMaxLoc(image, &min, &max);
	if (min == max) {
		// Добавляем E || F узел
		tree->value = min ? 0 : 1;
	} else {
		// Добавляем детализирующий узел P
		tree->value = 2;
		tree->initNodes();

		fillCompressedQuadTree(
			cv::Mat(image, cv::Rect(0, 0, image.cols / 2, image.rows / 2)),
			tree->topLeft
		);
		fillCompressedQuadTree(
			cv::Mat(image, cv::Rect(image.cols / 2, 0, image.cols / 2, image.rows / 2)),
			tree->topRight
		);
		fillCompressedQuadTree(
			cv::Mat(image, cv::Rect(0, image.rows / 2, image.cols / 2, image.rows / 2)),
			tree->bottomLeft
		);
		fillCompressedQuadTree(
			cv::Mat(image, cv::Rect(image.cols / 2, image.rows / 2, image.cols / 2, image.rows / 2)),
			tree->bottomRight
		);
	}
}

ShapeRepresentationModels::AbstractModel::AbstractRepresentation* ShapeRepresentationModels::CompressedQuadTree::encodeSingleObject(const cv::Mat &object)
{
	Representation* objectRepresentation = new Representation();
	std::vector < cv::Point > contour = getImageContour(object);
	cv::Rect rect = boundingRect(contour);
	cv::Rect area = cv::Rect(rect.x + 1, rect.y + 1, rect.width - 2, rect.height - 2);
	
	cv::Size imageSize = getOptimalSize(area.width, area.height);
	cv::Mat image = cv::Mat::ones(imageSize, CV_8UC1) * 255;
	for (int y = 0; y < area.height; y++) {
		for (int x = 0; x < area.width; x++) {
			image.at<uchar>(y, x) = object.at<uchar>(y + area.y, x + area.x);	
		}
	}
	
	CompressedQuadTreeNode* tree = new CompressedQuadTreeNode();
	fillCompressedQuadTree(image, tree);
	compress(tree);
	
	objectRepresentation->rect = area;
	objectRepresentation->tree = tree;
	
	return objectRepresentation;
}

void ShapeRepresentationModels::CompressedQuadTree::drawCompressedQuadTree(cv::Mat image, CompressedQuadTreeNode* tree, cv::Rect rect)
{	
	int paramsCount = tree->compressParameters.size();
	for (int i = 0; i < paramsCount; i++) {
		if (!tree->compressParameters[i]) {
			rect = cv::Rect(rect.x, rect.y, rect.width / 2, rect.height / 2);
		} else if (tree->compressParameters[i] == 1) {
			rect = cv::Rect(rect.x + rect.width / 2, rect.y, rect.width / 2, rect.height / 2);
		} else if (tree->compressParameters[i] == 2) {
			rect = cv::Rect(rect.x, rect.y + rect.height / 2, rect.width / 2, rect.height / 2);
		} else if (tree->compressParameters[i] == 3) {
			rect = cv::Rect(rect.x + rect.width / 2, rect.y + rect.height / 2, rect.width / 2, rect.height / 2);
		}
	}

	if (tree->value == 2) {
		drawCompressedQuadTree(
			image,
			tree->topLeft,
			cv::Rect(rect.x, rect.y, rect.width / 2, rect.height / 2)
		);
		drawCompressedQuadTree(
			image,
			tree->topRight,
			cv::Rect(rect.x + rect.width / 2, rect.y, rect.width / 2, rect.height / 2)
		);
		drawCompressedQuadTree(
			image,
			tree->bottomLeft,
			cv::Rect(rect.x, rect.y + rect.height / 2, rect.width / 2, rect.height / 2)
		);
		drawCompressedQuadTree(
			image,
			tree->bottomRight,
			cv::Rect(rect.x + rect.width / 2, rect.y + rect.height / 2, rect.width / 2, rect.height / 2)
		);
	} else if (tree->value){
		cv::Point ptA = cv::Point(rect.x, rect.y);
		cv::Point ptB = cv::Point(rect.x + rect.width - 1, rect.y + rect.height - 1);
		rectangle(image, ptA, ptB, cv::Scalar(0, 0, 0), -1, 8);
	}
}

cv::Mat ShapeRepresentationModels::CompressedQuadTree::decodeSingleObject(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
	cv::Size optimalImageSize = getOptimalSize(objectRepresentation->rect.width, objectRepresentation->rect.height);
	cv::Size imageSize = cv::Size(
		objectRepresentation->rect.x + optimalImageSize.width,
		objectRepresentation->rect.y + optimalImageSize.height
	);
	cv::Rect rect = cv::Rect(
		objectRepresentation->rect.x,
		objectRepresentation->rect.y,
		optimalImageSize.width,
		optimalImageSize.height
	);
	
	cv::Mat result = cv::Mat::ones(imageSize, CV_8UC1) * 255;
	drawCompressedQuadTree(result, objectRepresentation->tree, rect);

	return result;
}