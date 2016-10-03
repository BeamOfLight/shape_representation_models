#include <shape_representation_models/quad_tree.h>

ShapeRepresentationModels::QuadTree::QuadTree(size_t pointRepresentationSize)
{
	this->pointRepresentationSize = pointRepresentationSize;
}

std::string ShapeRepresentationModels::QuadTree::getMethodName()
{
	return std::string("QuadTree");
}

cv::Vec3i ShapeRepresentationModels::QuadTree::getNodeCounters(QuadTreeNode* node, cv::Vec3i counters)
{
	counters[node->value]++;
	if (node->value == 2) {
		counters = getNodeCounters(node->topLeft, counters);
		counters = getNodeCounters(node->topRight, counters);
		counters = getNodeCounters(node->bottomLeft, counters);
		counters = getNodeCounters(node->bottomRight, counters);
	}

	return counters;
}

cv::Size ShapeRepresentationModels::QuadTree::getOptimalSize(int width, int height)
{
	int value = std::max(width, height);
	int newValue = (int) pow(2, ceil(log2(value)));
	
	return cv::Size(newValue, newValue);
}

void ShapeRepresentationModels::QuadTree::fillQuadTree(cv::Mat image, QuadTreeNode* tree)
{
	double min, max;
	cv::minMaxLoc(image, &min, &max);

	if (min == max) {
		// Добавляем E || F узел
		tree->value = min ? 0 : 1;
	} else {
		// Добавляем детализирующий узел P
		tree->value = 2;
		tree->initNodes();

		fillQuadTree(
			cv::Mat(image, cv::Rect(0, 0, image.cols / 2, image.rows / 2)),
			tree->topLeft
		);
		fillQuadTree(
			cv::Mat(image, cv::Rect(image.cols / 2, 0, image.cols / 2, image.rows / 2)),
			tree->topRight
		);
		fillQuadTree(
			cv::Mat(image, cv::Rect(0, image.rows / 2, image.cols / 2, image.rows / 2)),
			tree->bottomLeft
		);
		fillQuadTree(
			cv::Mat(image, cv::Rect(image.cols / 2, image.rows / 2, image.cols / 2, image.rows / 2)),
			tree->bottomRight
		);
	}
}

void ShapeRepresentationModels::QuadTree::drawQuadTree(cv::Mat image, QuadTreeNode* tree, cv::Rect rect)
{	
	if (tree->value == 2) {
		drawQuadTree(
			image,
			tree->topLeft,
			cv::Rect(rect.x, rect.y, rect.width / 2, rect.height / 2)
		);
		drawQuadTree(
			image,
			tree->topRight,
			cv::Rect(rect.x + rect.width / 2, rect.y, rect.width / 2, rect.height / 2)
		);
		drawQuadTree(
			image,
			tree->bottomLeft,
			cv::Rect(rect.x, rect.y + rect.height / 2, rect.width / 2, rect.height / 2)
		);
		drawQuadTree(
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

int ShapeRepresentationModels::QuadTree::getObjectRepresentationSize(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
{
	Representation* objectRepresentation = static_cast < Representation* > (abstractObjectRepresentation);
	cv::Vec3i counters = getNodeCounters(objectRepresentation->tree);
	int bitsPerLink = 16;
	int bitsPerEFNode = 2;
	// Сохраняем степень 2 - для одной из сторон нормированного изображения - квадрата
	int bitsPerImageSizeRatio = 12;

	int countEF = counters[0] + counters[1];
	int countP = counters[2];
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
	int nodesPSize = bitsPerLink * (countP - 1);
	float resultInBits = (float) (headerSize + nodesEFSize + nodesPSize);

	return (int) ceil(resultInBits / 8);
}

ShapeRepresentationModels::AbstractModel::AbstractRepresentation* ShapeRepresentationModels::QuadTree::encodeSingleObject(const cv::Mat &object)
{
	Representation* objectRepresentation = new Representation();
	std::vector < cv::Point > contour = getImageContour(object);
	cv::Rect rect = boundingRect(contour);
	cv::Rect area = cv::Rect(rect.x + 1, rect.y + 1, rect.width - 2, rect.height - 2);

	cv::Size imageSize = getOptimalSize(area.width, area.height);
	cv::Mat image = cv::Mat::ones(imageSize, CV_8UC1) * 255;
	for (int y = 0; y < area.height; y++) {
		for (int x = 0; x < area.width; x++) {
			image.at<uchar>(y, x) = object.at < uchar > (y + area.y, x + area.x);	
		}
	}

	QuadTreeNode* tree = new QuadTreeNode();
	fillQuadTree(image, tree);
	
	objectRepresentation->rect = area;
	objectRepresentation->tree = tree;
	
	return objectRepresentation;
}

cv::Mat ShapeRepresentationModels::QuadTree::decodeSingleObject(AbstractModel::AbstractRepresentation* abstractObjectRepresentation)
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
	drawQuadTree(result, objectRepresentation->tree, rect);

	return result;
}
