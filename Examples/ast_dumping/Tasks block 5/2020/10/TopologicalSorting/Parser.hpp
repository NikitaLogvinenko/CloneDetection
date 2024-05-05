#pragma once
#include <string>
#include <vector>
#include "Graph.hpp"

class Parser {
public:
	Parser() = delete;
	static void parseStringByVerices(const std::string& str, Graph::vertex& vertex, std::vector<Graph::vertex>& dependens);
	~Parser() = default;
};
