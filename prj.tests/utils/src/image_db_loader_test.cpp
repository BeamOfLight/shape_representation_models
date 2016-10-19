#include <vector>
#include <string>
#include <iostream>
#include <image_db_loader/interval_string_parser.h>
#include <image_db_loader/images_config_parser.h>
#include <dpcore/filesystem.h>

/**
Code from http://stackoverflow.com/a/18764531
Neil Kirk http://ideone.com/2W99Tt
*/
void testIntervalStringParser()
{
	std::cout << "===========[ testIntervalStringParser ]" << std::endl;
	std::string input_data = "1-5,10,12,15-16,25-35,67,69,99-105";
	std::cout << "Input: " << input_data << std::endl;
	std::vector<int> result = ImageDbLoader::IntervalStringParser::parseIntervalString(input_data);
	std::cout << "Output: ";
	for (std::vector<int>::const_iterator it = result.begin(), end_it = result.end(); it != end_it; ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	std::cout << "===========" << std::endl;
}

void testImagesConfigParser()
{
	std::cout << "===========[ testImagesConfigParser ]" << std::endl;
	std::string filename = DpCore::Filesystem::getCrossPlatformFileName("config/database_reader_default_config.yml");
	std::vector<std::string> result = ImageDbLoader::ImagesConfigParser::parseConfig(filename, true);
	std::cout << "Images count: " << result.size() << std::endl;
	for (std::vector<std::string>::const_iterator it = result.begin(), end_it = result.end(); it != end_it; ++it) {
		std::cout << *it << std::endl;
	}
	std::cout << "===========" << std::endl;
}

int main()
{
	testIntervalStringParser();
	testImagesConfigParser();

	return 0;
}