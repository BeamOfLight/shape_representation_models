/**
	https://github.com/BeamOfLight/shape_representation_models.git
    object.cpp

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04 
*/

#include <dpcore/object.h>

DpCore::ObjectRepresentation < std::vector < cv::Point > > DpCore::Object::convertObject2PointRepresentation(const cv::Mat &object, size_t minContourSize)
{
	std::vector < std::vector < cv::Point > > srcContours;
	std::vector < cv::Vec4i > hierarchy;

	cv::Mat tmp = cv::Mat::zeros(object.size(), CV_8UC1);
	object.copyTo(tmp);
	cv::findContours(tmp, srcContours, hierarchy, CV_RETR_TREE , CV_CHAIN_APPROX_NONE);
	tmp.release();
		
	int contoursCount = srcContours.size();
	std::vector < std::vector < cv::Point > > resultContours;
	std::vector < cv::Vec4i > result_hierarchy;
	for (int i = 0; i < contoursCount; i++) {
		if (srcContours[i].size() > minContourSize && hierarchy[i][3] != -1) {
			resultContours.push_back(srcContours[i]);
			result_hierarchy.push_back(hierarchy[i]);
		}
	}

	ObjectRepresentation < std::vector < cv::Point > > objectDscriptor;
	if (contoursCount > 0) {
		contoursCount = resultContours.size();
		int longest_contour_id = -1;
		size_t max_length = 0;
		for (int contour_id = 0; contour_id < contoursCount; contour_id++) {
			if (resultContours[contour_id].size() > max_length) {
				max_length = resultContours[contour_id].size();
				longest_contour_id = contour_id;
			}
		}	

		if (longest_contour_id >= 0) {
			objectDscriptor.outterContour = resultContours[longest_contour_id];
			resultContours.erase(resultContours.begin() + longest_contour_id);
			result_hierarchy.erase(result_hierarchy.begin() + longest_contour_id);
		}

		contoursCount = resultContours.size();
		for (int contour_id = 0; contour_id < contoursCount; contour_id++) {
			if (result_hierarchy[contour_id][3] == longest_contour_id + 1) {
				objectDscriptor.innerContours.push_back(resultContours[contour_id]);
			}
		}
	}
		
	return objectDscriptor;
}

cv::Mat DpCore::Object::convertPointRepresentation2Object(ObjectRepresentation < std::vector < cv::Point > > pointRepresentation)
{
	std::vector < std::vector < cv::Point > > contours;
	contours.push_back(pointRepresentation.outterContour);
	int holesCount = pointRepresentation.innerContours.size();
	for (int i = 0; i < holesCount; i++) {
		contours.push_back(pointRepresentation.innerContours[i]);
	}
		
	cv::Rect rect = cv::boundingRect(pointRepresentation.outterContour);

	cv::Mat object = cv::Mat::ones(
		cv::Size(
			rect.x + rect.width + 1,
			rect.y + rect.height + 1
		),
		CV_8UC1
	) * 255;
	cv::drawContours(object, contours, 0, cv::Scalar(0, 0, 0), -1, 8, cv::noArray(), INT_MAX, cv::Point(0, 0));
	cv::drawContours(object, contours, 0, cv::Scalar(255, 255, 255), 1, 8, cv::noArray(), INT_MAX, cv::Point(0, 0));

	for (int i = 0; i < holesCount; i++) {
		cv::drawContours(object, contours, i + 1, cv::Scalar(0, 0, 0), 1, 8, cv::noArray(), INT_MAX, cv::Point(0, 0));
		cv::drawContours(object, contours, i + 1, cv::Scalar(255, 255, 255), -1, 8, cv::noArray(), INT_MAX, cv::Point(0, 0));
	}

	return object;
}

float DpCore::Object::compareObjects(const cv::Mat &object1_src, const cv::Mat &object2_src)
{
	cv::Mat object1 = DpCore::Common::normalizeBinaryImage(object1_src);
	cv::Mat object2 = DpCore::Common::normalizeBinaryImage(object2_src);
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

	int diffPointsCounter = 0;
	for (int y = 0; y < diffImage.rows; y++) {
		for (int x = 0; x < diffImage.cols; x++) {
			if (diffImage.at < uchar > (cv::Point(x,y))) {
				diffPointsCounter++;
			}
		}
	}

	int initAreaCounter = 0;
	for (int y = 0; y < object1.rows; y++) {
		for (int x = 0; x < object1.cols; x++) {
			if (!object1.at < uchar > (cv::Point(x,y))) {
				initAreaCounter++;
			}
		}
	}

	return static_cast < float > (diffPointsCounter) * 100 / static_cast < float > (initAreaCounter);
}
