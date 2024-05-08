#include "Parser.hpp"

void Parser::parseStringByVerices(const std::string& s, Graph::vertex& vertex, std::vector<Graph::vertex>& dependens) {
	auto first_position = s.find('=');
	if (first_position == std::string::npos)
		return;
	if (s[first_position - 1] == ' ')
		vertex = s.substr(0, first_position - 1);
	else
		vertex = s.substr(0, first_position);
	while (!s.substr(first_position).empty()) {
		first_position = s.find_first_not_of(' ', first_position + 1);
		size_t lenght;
		if (first_position == std::string::npos)
			break;
		auto second_position = s.find_first_of(',', first_position);
		if (second_position == std::string::npos)
			second_position = s.length();
		if (s[second_position - 1] == ' ')
			lenght = second_position - first_position - 1;
		else
			lenght = second_position - first_position;
		dependens.push_back(s.substr(first_position, lenght));
		first_position = second_position;
	}
}