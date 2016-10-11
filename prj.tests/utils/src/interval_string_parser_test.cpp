#include <vector>
#include <string>
#include <iostream>
#include <text/interval_string_parser.h>

int main()
{
	std::string input_data = "1-5,10,12,15-16,25-35,67,69,99-105";
	std::cout << "Input: " << input_data << std::endl;
	std::vector<int> result = Text::IntervalStringParser::parseIntervalString(input_data);
	std::cout << "Output: ";
	for (std::vector<int>::const_iterator it = result.begin(), end_it = result.end(); it != end_it; ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}