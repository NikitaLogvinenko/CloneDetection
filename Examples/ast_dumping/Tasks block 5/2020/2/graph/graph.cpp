#include "graph.h"

using namespace std;
static const auto infinity_weight = std::numeric_limits<weight>::max();

void graph::add_edge(const vertex& first, const vertex& second, const weight& weight, bool direcred)
{
	adjacency_list_[first].insert({ second, weight });

	if (!direcred)
	{
		adjacency_list_[second].insert({ first, weight });
	}
}

std::unordered_set<vertex> graph::get_vertices() const
{
	unordered_set<vertex> vertices(vertex_count());

	for (const auto& [vertex, dependencies] : adjacency_list_)
	{
		vertices.emplace(vertex);
	}	
	return vertices;
}

void graph::clear()
{
	adjacency_list_.clear();
}

weight graph::edge_weight(const vertex& first, const vertex& second) const
{
	const auto first_it = adjacency_list_.find(first);
	
	if (first_it != adjacency_list_.end())
	{
		const auto second_it = (*first_it).second.find(second);

		if (second_it != (*first_it).second.end())
		{
			return (*second_it).second;
		}
	}
	return infinity_weight;
}

size_t graph::vertex_count() const
{
	return adjacency_list_.size();
}

bool graph::has(const vertex& vertex) const
{
	return  adjacency_list_.end() != adjacency_list_.find(vertex);
}