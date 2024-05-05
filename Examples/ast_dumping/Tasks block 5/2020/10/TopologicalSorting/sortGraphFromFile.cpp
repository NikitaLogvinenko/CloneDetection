#include "sortGraphFromFile.h"
#include "TopologicalSorting.h"
#include <string>
#include <iterator>
#include "Parser.hpp"
void sortGraphFfromFile(std::istream& in, std::ostream& out) {
	std::string str, vertex;
	Graph graph;
	while (std::getline(in, str)) {
		std::vector<std::string> dependent_nodes;
		Parser::parseStringByVerices(str, vertex, dependent_nodes);
		graph.add_vertex(vertex, dependent_nodes);
		dependent_nodes.clear();
	}
	const auto ordered = TopologicalSort::Sort(graph);
	std::copy(ordered.begin(), ordered.end(), std::ostream_iterator<std::string>(out, "\n"));
}