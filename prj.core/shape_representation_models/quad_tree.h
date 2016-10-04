/**
	https://github.com/BeamOfLight/shape_representation_models.git
    quad_tree.h

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04 
*/

#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_QUADTREE_H
#define SHAPE_REPRESENTATION_MODELS_QUADTREE_H

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <shape_representation_models/abstract_area_model.h>

namespace ShapeRepresentationModels {
	class QuadTree : public AbstractAreaModel
	{
	  public:
		struct QuadTreeNode
		{
			// 0 - Empty, 1 - Full, 2 - P, Need detalization
			unsigned char value;
			QuadTreeNode* topLeft;
			QuadTreeNode* topRight;
			QuadTreeNode* bottomLeft;
			QuadTreeNode* bottomRight;
		
			QuadTreeNode()
			{
				this->value = 99;
				topLeft = 0;
				topRight = 0;
				bottomLeft = 0;
				bottomRight = 0;
			}
		
			void initNodes()
			{
				topLeft     = new QuadTreeNode();
				topRight    = new QuadTreeNode();
				bottomLeft  = new QuadTreeNode();
				bottomRight = new QuadTreeNode();
			}
		};

		struct Representation : public AbstractRepresentation
		{
			cv::Rect rect;
			QuadTreeNode* tree;
		};

		QuadTree(size_t pointRepresentationSize);
		std::string getMethodName();
	
		int getObjectRepresentationSize(AbstractRepresentation* encodedObject);
		AbstractRepresentation* encodeSingleObject(const cv::Mat &object);
		cv::Mat decodeSingleObject(AbstractRepresentation* encodedObject);	
	  private:
		size_t pointRepresentationSize;
	
		cv::Vec3i getNodeCounters(QuadTreeNode* tree, cv::Vec3i counters = cv::Vec3i(0, 0, 0));
		cv::Size getOptimalSize(int width, int height);
		void fillQuadTree(cv::Mat image, QuadTreeNode* tree);
		void drawQuadTree(cv::Mat image, QuadTreeNode* tree, cv::Rect rect);
	};
}

#endif