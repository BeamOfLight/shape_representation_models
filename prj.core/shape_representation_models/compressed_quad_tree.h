/**
	https://github.com/BeamOfLight/shape_representation_models.git
	compressed_quad_tree.h

	@author Denis Borisoglebskiy
	@version 1.0 2016-10-04
*/

#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_COMPRESSED_QUADTREE_H
#define SHAPE_REPRESENTATION_MODELS_COMPRESSED_QUADTREE_H

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <shape_representation_models/abstract_area_model.h>

namespace ShapeRepresentationModels {
  class CompressedQuadTree : public AbstractAreaModel
  {
    public:
			struct CompressedQuadTreeNode
			{
				// 0 - Empty, 1 - Full, 2 - P, Need detalization
				char value;
				std::vector < unsigned char > compressParameters;

				CompressedQuadTreeNode* topLeft;
				CompressedQuadTreeNode* topRight;
				CompressedQuadTreeNode* bottomLeft;
				CompressedQuadTreeNode* bottomRight;

				CompressedQuadTreeNode()
				{
					this->value = -1;
					topLeft = 0;
					topRight = 0;
					bottomLeft = 0;
					bottomRight = 0;
				}

				void initNodes()
				{
					topLeft     = new CompressedQuadTreeNode();
					topRight    = new CompressedQuadTreeNode();
					bottomLeft  = new CompressedQuadTreeNode();
					bottomRight = new CompressedQuadTreeNode();
				}
			};

			struct Representation : public AbstractRepresentation
			{
				cv::Rect rect;
				CompressedQuadTreeNode* tree;
			};

			CompressedQuadTree(size_t pointRepresentationSize);
			std::string getMethodName();

			int getObjectRepresentationSize(AbstractRepresentation* encodedObject);
			AbstractRepresentation* encodeSingleObject(const cv::Mat &object);
			cv::Mat decodeSingleObject(AbstractRepresentation* encodedObject);

			private:
			size_t pointRepresentationSize;

			cv::Vec4i getNodeCounters(CompressedQuadTreeNode* tree, cv::Vec4i counters = cv::Vec4i(0, 0, 0, 0));
			int getCompressDataSizeCounter(CompressedQuadTreeNode* tree, int counters = 0);
			cv::Size getOptimalSize(int width, int height);
			void fillCompressedQuadTree(cv::Mat image, CompressedQuadTreeNode* tree);
			CompressedQuadTreeNode* compressNode(CompressedQuadTreeNode* node);
			void compress(CompressedQuadTreeNode* tree);
			void drawCompressedQuadTree(cv::Mat image, CompressedQuadTreeNode* tree, cv::Rect rect);
		};
}

#endif
