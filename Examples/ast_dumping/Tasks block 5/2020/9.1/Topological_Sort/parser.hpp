#pragma once
#include <string>
#include <vector>
#include "graph.hpp"

class parser
{
public:
	parser() = delete;
	static void parse_string_on_verices(const std::string& s, graph::vertex& vertex, std::vector<graph::vertex>& dependent_nodes);
};
