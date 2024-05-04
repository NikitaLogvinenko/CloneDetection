#pragma once
#include "Graph.hpp"

class TopologicalSort final
{
public:
	TopologicalSort() = delete;
	~TopologicalSort() = default;

	static Graph::vertexes Sort(const Graph& g);

private:
	enum class Colors { white, gray, black };
	typedef std::unordered_map<Graph::vertex, Colors> visited_vertexes;
	static void depthSorting(const Graph& g, const Graph::vertex& v, visited_vertexes& visited, Graph::vertexes& ordered);
};

