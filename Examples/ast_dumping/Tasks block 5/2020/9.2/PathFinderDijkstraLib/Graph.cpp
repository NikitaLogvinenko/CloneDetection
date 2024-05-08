#include "Graph.hpp"

void Graph::AddEdge(const Vertex& src, const Weight& len, const Vertex& dest)
{
	if (data[src].empty()) {
		vertices_list.push_back(src);
	}
	if (data[dest].empty()) {
		vertices_list.push_back(dest);
	}
	data[src].push_back(std::make_pair(dest, len));
	data[dest].push_back(std::make_pair(src, len));
}

const Graph::AdjList& Graph::GetAdjList(const Vertex& src) const
{
	if (data.find(src) == data.end()) {
		return empty_adj_list;
	}
	return data.at(src);
}

const Graph::Vertices& Graph::GetVerticesList() const
{
	if (vertices_list.empty()) {
		return empty_vertices_list;
	}
	return vertices_list;
}