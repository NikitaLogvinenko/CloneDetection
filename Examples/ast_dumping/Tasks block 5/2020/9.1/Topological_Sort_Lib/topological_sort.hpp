#pragma once
#include "graph.hpp"

class topological_sort final
{
public:
	topological_sort() = delete;
	static graph::vertices sort(const graph& g);
private:
	enum class colors { white, gray, black };
	
	typedef std::unordered_map<graph::vertex, colors> visited_vertices;
	static void depth_sorting_traversal(const graph& g, const graph::vertex& v, visited_vertices& visited, graph::vertices& ordered);
};