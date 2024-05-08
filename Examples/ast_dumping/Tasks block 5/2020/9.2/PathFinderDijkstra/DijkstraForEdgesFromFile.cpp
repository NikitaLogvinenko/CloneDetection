#include "DijkstraForEdgesFromFile.hpp"
#include "PathFinderDijkstra.hpp"
#include <algorithm>
#include <iterator>

void DijkstraForEdgesFromFile(std::istream& in_stream, std::ostream& out_stream) 
{
	Graph graph;
	std::string str, src, dest;
	unsigned int weight;
	if (!getline(in_stream, str)) {
		return;
	}
	auto src_vertex = str.substr(2);
	while (in_stream >> src && in_stream >> dest && in_stream >> weight){
		graph.AddEdge(src, weight, dest);
	}
	
	PathFinderDijkstra finder;
	finder.Find(graph, src_vertex);
	
	for (const auto& vertex : graph.GetVerticesList()){
		if (src_vertex == vertex) {
			continue;
		}
		out_stream << vertex << " - {";
		auto path = finder.GetShortestPath(vertex);
		std::copy(path.begin(), path.end(), std::ostream_iterator<std::string>(out_stream, ", "));
		out_stream << vertex << "} - " << finder.GetShortestDistance(vertex) << std::endl;
	}
}