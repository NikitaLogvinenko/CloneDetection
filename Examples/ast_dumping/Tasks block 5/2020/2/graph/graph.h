#pragma once

#include <unordered_set>
#include <unordered_map>

using vertex = std::string;
using weight = size_t;

class edge final
{
	vertex first_;
	vertex second_;
	weight weight_ ;

public:
	edge(const vertex& first, const vertex& second, const weight& weight)
		: first_(first), second_(second), weight_(weight) {}

	const weight& weight() const { return weight_; }
	const vertex& first() const { return first_; }
	const vertex& second() const { return second_; }
};

class graph final
{
	std::unordered_map<vertex, std::unordered_map<vertex, weight>> adjacency_list_;

public:
	graph() = default;
	void add_edge(const vertex& first, const vertex& second, const weight& weight, bool directed = false);

	std::unordered_set<vertex> get_vertices() const;
	
	weight edge_weight(const vertex& first, const vertex& second) const;
	size_t vertex_count() const;

	bool has(const vertex& vertex) const;
	void clear();
};

