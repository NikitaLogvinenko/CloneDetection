#include "parser.hpp"

void parser::parse_string_on_verices(const std::string& s, graph::vertex& vertex, std::vector<graph::vertex>& dependent_nodes)
{
	auto first_pos = s.find('=');
	if (first_pos == std::string::npos) {
		return;
	}
	if (s[first_pos - 1] == ' ') {
		vertex = s.substr(0, first_pos - 1);
	}
	else {
		vertex = s.substr(0, first_pos);
	}

	while (!s.substr(first_pos).empty()) {
		first_pos = s.find_first_not_of(' ', first_pos + 1);
		if (first_pos == std::string::npos) {
			break;
		}
		auto second_pos = s.find_first_of(',', first_pos);
		size_t len;
		if (second_pos == std::string::npos) {
			second_pos = s.length();
		}
		if (s[second_pos - 1] == ' ') {
			len = second_pos - first_pos - 1;
		}
		else {
			len = second_pos - first_pos;
		}
		dependent_nodes.push_back(s.substr(first_pos, len));
		first_pos = second_pos;
	}
}
