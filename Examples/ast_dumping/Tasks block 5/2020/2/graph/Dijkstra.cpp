#include "Dijkstra.h"

#include <algorithm>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <iterator>

using namespace std;

static const auto infinity_weight = std::numeric_limits<weight>::max();

bool dijkstra::ready() const
{
	return !min_dist_.empty();
}

void dijkstra::clear()
{
	min_dist_.clear();
	parents_.clear();
}

vector<vertex> dijkstra::get_path(const vertex& dst) const
{
	vector<vertex> path;

	if (min_dist_.at(dst) < infinity_weight)
	{
		for (auto i = dst; i != start_; i = parents_.at(i))
		{
			path.push_back(i);
		}
		path.push_back(start_);
		reverse(path.begin(), path.end());
	}
	
	return path;
}

vertex dijkstra::find_min(const std::unordered_set<vertex>& unused)
{
	auto min_vertex_name = *unused.begin();
	auto min_weight = min_dist_[min_vertex_name];

	for (const auto& vertex : unused)
	{
		const auto cur_weight = min_dist_[vertex];
		
		if ( cur_weight < min_weight)
		{
			min_weight = cur_weight;
			min_vertex_name = vertex;
		}
	}
	return min_vertex_name;
}

void dijkstra::build(const graph& dist_graph, const vertex& start)
{
	if (dist_graph.vertex_count() == 0 || !dist_graph.has(start))
		return;

	clear();
	start_ = start;
	
	auto unvisited(dist_graph.get_vertices());
	unvisited.erase(start);
	
	for (const auto& cur_vertex : unvisited)
	{
		min_dist_.emplace(cur_vertex, dist_graph.edge_weight(start, cur_vertex));
		parents_.emplace(cur_vertex, start_);
	}
	
	while(!unvisited.empty())
	{
		const auto min_vertex_name = find_min(unvisited);
		const auto min_weight = min_dist_[min_vertex_name];
		
		if (min_weight == infinity_weight)
		{
			break;
		}
		
		unvisited.erase(min_vertex_name);

		for (const auto& vertex : unvisited)
		{
			auto it = min_dist_.find(vertex);
			auto new_dist = dist_graph.edge_weight(min_vertex_name, vertex);
			new_dist += new_dist != infinity_weight ? min_weight : 0;
			
			if ((*it).second > new_dist)
			{
				(*it).second = new_dist;
				parents_[(*it).first] = min_vertex_name;
			}
		}
	}
}

std::map<vertex, weight> dijkstra::get_min_distances() const
{
	return min_dist_;
}

std::ostream& operator<<(std::ostream& output, const dijkstra& dijkstra)
{
	if (!output.good() || !dijkstra.ready())
		return output;

	for (const auto& [first, second] : dijkstra.min_dist_)
	{
		output << first << " - {";

		const auto path = dijkstra.get_path(first);

		if (!path.empty())
		{
			for (size_t j = 0; j < path.size(); ++j)
				output << path[j] << (j < path.size() - 1 ? ", " : "");

			output << "} - " << second << "\n";
		}
		else
		{
			output << "there is no way}\n";
		}
	}
	return output;
}