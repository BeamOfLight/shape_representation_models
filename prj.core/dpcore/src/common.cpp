#include <dpcore/common.h>

int DpCore::Common::round(double value)
{
#ifdef __linux__
	return std::round(value);
#else
	return static_cast < int > (value < 0.0 ? ceil(value - 0.5) : floor(value + 0.5));
#endif
}

void DpCore::Common::showImage(const std::string &window_name, cv::Mat image, cv::Point pos)
{
	std::cout << "Image size: " << image.cols << " x " << image.rows << std::endl;
	const char* window = window_name.c_str();
	cv::namedWindow( window, CV_WINDOW_AUTOSIZE );
	cv::imshow( window, image );
	cv::moveWindow(window, pos.x, pos.y);
}

void DpCore::Common::showVideo(const std::string &window_name, const std::vector < cv::Mat > &video, cv::Point pos)
{
	const char* window = window_name.c_str();
	size_t frames_count = video.size();
	size_t frame_id = 0;
	while (cv::waitKey() != 27) {
		showImage(window, video[frame_id++ % frames_count]);
	}
}

void DpCore::Common::showBinaryImage(const std::string &window_name,  cv::Mat image, cv::Point pos)
{
	const char* window = window_name.c_str();
	cv::Mat tmp = image.mul(255);
	cv::namedWindow(window, CV_WINDOW_AUTOSIZE);
	cv::imshow(window, tmp);
	cv::moveWindow(window, pos.x, pos.y);
}


void DpCore::Common::showSingleContour(const std::string &window_name, std::vector < cv::Point > contour, size_t width, size_t height)
{
	const char* window = window_name.c_str();
	std::vector< std::vector < cv::Point > > dstContours = std::vector < std::vector < cv::Point> > ();
	dstContours.push_back(contour);
	cv::Mat dst = cv::Mat::zeros(width, height, CV_8UC3);
	cv::drawContours(dst, dstContours, -1, cv::Scalar(0, 255, 0), 1, 8);

	cv::namedWindow(window, CV_WINDOW_AUTOSIZE);
	cv::imshow(window, dst);
	cv::waitKey();
}

void DpCore::Common::showContours(const std::string &window_name, std::vector < std::vector < cv::Point > > contours, size_t width, size_t height)
{
	const char* window = window_name.c_str();
	cv::Mat dst = cv::Mat::zeros(width, height, CV_8UC3);
	cv::drawContours(dst, contours, -1, cv::Scalar(0, 255, 0), 1, 8);
	
	cv::namedWindow(window, CV_WINDOW_AUTOSIZE);
	cv::imshow(window, dst);
	cv::waitKey();
}

cv::Mat DpCore::Common::normalizeBinaryImage(const cv::Mat &image)
{
	cv::Mat result = cv::Mat::zeros(image.size(), CV_8UC1);
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			result.at < uchar > (cv::Point(x,y)) = (image.at < uchar > (cv::Point(x,y)) ? 255 : 0);
		}
	}

	return result;
}

cv::FileStorage DpCore::Common::loadConfig(int argc, char ** argv, const std::string &default_filename)
{
	std::string config_filename_src = default_filename;
	if (argc == 2) {
		config_filename_src = std::string(argv[1]);
	}
	
	std::string config_filename = DpCore::Filesystem::getCrossPlatformFileName(config_filename_src);

	if (!DpCore::Filesystem::isFileExist(config_filename)) {
		std::cerr << "[DpCore::Common::loadConfig] File does not exist: " << config_filename << std::endl;
		throw;
	}

	cv::FileStorage config_fs(config_filename.c_str(), cv::FileStorage::READ);
	if (!config_fs.isOpened()) {
		std::cerr << "[DpCore::Common::loadConfig] Cannot open configuration file: " << config_filename << std::endl;
		throw;
	}

	return config_fs;
}

void DpCore::Common::writeUCharVectorToFile(const std::string &filename, std::vector < unsigned char > data)
{
	FILE* f = fopen(filename.c_str(), "wb");
	if (f == NULL) {
		std::cerr << "[DpCore::Common::writeUCharVectorToFile] File fopen error" << std::endl;
		exit(1);
	}
	
	fwrite(&data[0], 1, data.size(), f);
	fclose(f);
}

std::vector < unsigned char > DpCore::Common::readUCharVectorFromFile(const std::string &filename)
{
	FILE* f = fopen(filename.c_str(), "rb");
	if (f == NULL) {
		std::cerr << "[DpCore::Common::readUCharVectorFromFile] File fopen error" << std::endl;
		exit(1);
	}
	
	// obtain file size
	fseek(f , 0 , SEEK_END);
	size_t size = ftell (f);
	rewind(f);
	std::vector < unsigned char > data = std::vector < unsigned char >(size);

	size_t result = fread(&data[0], 1, size, f);
	if (result != size) {
		std::cerr << "[DpCore::Common::readUCharVectorFromFile] File fread error" << std::endl;
		exit(1);
	}
	fclose(f);

	return data;
}

std::string DpCore::Common::toString(int value)
{
	std::stringstream ss;
	ss << value;

	return ss.str();
}

