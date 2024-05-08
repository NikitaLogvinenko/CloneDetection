#pragma once
#include "Graph.hpp"
#include <unordered_map>

class PathFinderDijkstra final {
public:
	void Find(const Graph& graph, const Graph::Vertex& src);

	const Graph::Vertices& GetShortestPath(const Graph::Vertex& dest) const;
	Graph::Weight GetShortestDistance(const Graph::Vertex& dest) const;

	~PathFinderDijkstra() = default;
private:
	std::unordered_map<Graph::Vertex, Graph::Weight> dist_map;
	std::unordered_map<Graph::Vertex, Graph::Vertices> path_map;

	void Clear();
	
	const Graph::Vertices empty_path_list;
	const unsigned infinity_distance = std::numeric_limits<size_t>::max();
};