#pragma once
#include <unordered_map>
#include <map>
#include <ostream>

#include "graph.h"

class dijkstra final
{
	std::map<vertex, weight> min_dist_;
	std::unordered_map<vertex, vertex> parents_;
	vertex start_{};

	vertex find_min(const std::unordered_set<vertex>& unused);

	
public:
	dijkstra() = default;

	bool ready() const;
	void clear();
	void build(const graph& dist_graph, const vertex& start);
	
	std::vector<vertex> get_path(const vertex& dst) const;
	std::map<vertex, weight> get_min_distances() const;

	friend std::ostream& operator<< (std::ostream& output, const dijkstra& dijkstra);
};

