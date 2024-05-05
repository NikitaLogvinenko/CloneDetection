#include "TopologicalSorting.h"
#include <functional>
#include <iostream>
#include <stack>

Graph::vertexes TopologicalSort::Sort(const Graph& g)
{
	visited_vertexes visited_vertexes;
	Graph::vertexes ordered;
	auto vertexes = g.get_vertices();
	std::reverse(vertexes.begin(), vertexes.end());
	for (const auto& v : vertexes) {
		visited_vertexes[v] = Colors::white;
	}

	for (const auto& v : vertexes) {
		if (visited_vertexes.empty()) {
			throw std::bad_function_call();
		}
		if (visited_vertexes[v] == Colors::white) {
			depthSorting(g, v, visited_vertexes, ordered);
		}
	}
	std::reverse(ordered.begin(), ordered.end());
	return ordered;
}

void TopologicalSort::depthSorting(const Graph& g, const Graph::vertex& v, visited_vertexes& visited_vertexes, Graph::vertexes& ordered)
{
	std::stack<std::pair<Graph::vertex, bool>> stack;
	stack.push(std::make_pair(v, false));
	visited_vertexes[v] = Colors::gray;
	while (!stack.empty()) {
		auto [vertex, det] = stack.top();
		stack.pop();

		if (det) {
			visited_vertexes[vertex] = Colors::black;
			ordered.push_back(vertex);
			continue;
		}
		stack.push(std::make_pair(vertex, true));
		for (const auto& i : g.dependent_vertices(vertex)) {
			if (visited_vertexes[i] == Colors::white) {
				stack.push(std::make_pair(i, false));
				visited_vertexes[i] = Colors::gray;
			}
			else if (visited_vertexes[i] == Colors::gray) {
				visited_vertexes.clear();
				return;
			}
		}
	}
}