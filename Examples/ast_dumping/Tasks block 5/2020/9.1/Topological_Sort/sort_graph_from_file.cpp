#include "topological_sort.hpp"
#include <string>
#include <iterator>
#include "parser.hpp"

void sort_graph_from_file(std::istream& in, std::ostream& out)
{
	std::string s, vertex;
	graph g;
	
	while (std::getline(in, s)){
		std::vector<std::string> dependent_nodes;
		parser::parse_string_on_verices(s, vertex, dependent_nodes);
		g.add_vertex(vertex, dependent_nodes);
		dependent_nodes.clear();
	}

	const auto ordered = topological_sort::sort(g);
	std::copy(ordered.begin(), ordered.end(), 
		std::ostream_iterator<std::string>(out, "\n"));
}
