#include "stdafx.h"
#include <string>
#include <vector>

std::vector<int> parse_string(std::string s)
{
	std::vector <int> numbers;
	for (size_t pos = 0;;)
	{
		pos = s.find_first_not_of(",\0 ", pos);
		if (pos == std::string::npos)
			break;

		size_t pos1 = s.find_first_of(",\0 ", pos);
		if (pos1 + 1 == 0)//возвращает (size_t)-1 если не нашёл
			pos1 = s.length();
		size_t len = (pos1 == std::string::npos) ? std::string::npos : pos1 - pos;

		std::string word(s.substr(pos, len));

		if (pos1 == std::string::npos)
			break;

		pos = pos1;
		numbers.push_back(stoi(word));
	}
	return numbers;
}
