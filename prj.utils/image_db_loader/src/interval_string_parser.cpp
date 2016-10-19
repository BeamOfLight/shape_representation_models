/**
Code from http://stackoverflow.com/a/18764531

@author Neil Kirk	http://ideone.com/2W99Tt
*/
#include <image_db_loader/interval_string_parser.h>

/**
@author Neil Kirk	http://ideone.com/2W99Tt
*/
int ImageDbLoader::IntervalStringParser::convertString2Int(const std::string& str)
{
	std::stringstream ss(str);
	int x;
	if (! (ss >> x)) {
		std::cerr << "Error converting " << str << " to integer" << std::endl;
		abort();
	}

	return x;
}

/**
@author Neil Kirk	http://ideone.com/2W99Tt
*/
std::vector<std::string> ImageDbLoader::IntervalStringParser::splitStringToArray(const std::string& str, char splitter)
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string temp;

	// split into new "lines" based on character
	while (std::getline(ss, temp, splitter)) {
		tokens.push_back(temp);
	}

	return tokens;
}

/**
@author Neil Kirk	http://ideone.com/2W99Tt
*/
std::vector<int> ImageDbLoader::IntervalStringParser::parseIntervalString(const std::string& data)
{
	std::vector<std::string> tokens = splitStringToArray(data, ',');
	std::vector<int> result;
	for (std::vector<std::string>::const_iterator it = tokens.begin(), end_it = tokens.end(); it != end_it; ++it)	{
		const std::string& token = *it;
		std::vector<std::string> range = splitStringToArray(token, '-');
		if (range.size() == 1) {
			result.push_back(convertString2Int(range[0]));
		}	else if (range.size() == 2)	{
			int start = convertString2Int(range[0]);
			int stop = convertString2Int(range[1]);
			for (int i = start; i <= stop; i++)	{
				result.push_back(i);
			}
		}	else {
			std::cerr << "Error parsing token " << token << std::endl;
			abort();
		}
	}

	return result;
}