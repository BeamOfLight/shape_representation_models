/**
	https://github.com/BeamOfLight/shape_representation_models.git
    common.h

    @author Denis Borisoglebskiy
    @version 1.0 2016-10-04 
*/

#pragma once
#ifndef DPCORE_COMMON_H
#define DPCORE_COMMON_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <string>
#include <iostream>
#include <cmath>

#include <dpcore/filesystem.h>

namespace DpCore
{
  namespace Common
  {
	// Кроссплатформенное округление
	int round(double value);

	//Показывает изображение image в окне window
	void showImage(const std::string &window_name, cv::Mat image, cv::Point pos = cv::Point(0,0));

	//Показывает видео video в окне window
	void showVideo(const std::string &window_name, const std::vector < cv::Mat > &video, cv::Point pos = cv::Point(0,0));

	//Показывает изображение image в окне window (для бинарных картинок 0 / 1)
	void showBinaryImage(const std::string &window_name, cv::Mat image, cv::Point pos = cv::Point(100, 100));

	// отображение на экране одниночного контура
	void showSingleContour(const std::string &window_name, std::vector < cv::Point > contour, size_t width, size_t height);

	// отобрадение на экране нескольких контуров
	void showContours(const std::string &window_name, std::vector < std::vector < cv::Point > > contours, size_t width, size_t height);

	// Нормализация бинарного изображения к формату 0 / 255
	cv::Mat normalizeBinaryImage(const cv::Mat &image);

	// Загрузка файла конфигурации
	cv::FileStorage loadConfig(int argc, char ** argv, const std::string &default_filename);

	// запись вектора байт в файл
	void writeUCharVectorToFile(const std::string &filename, std::vector < unsigned char > data);

	// чтение вектора байт из файла
	std::vector < unsigned char > readUCharVectorFromFile(const std::string &filename);

	// конвертация числа в строку
	std::string toString(int value);
  }
}

#endif
