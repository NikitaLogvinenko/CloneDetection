#include "PathFinderDijkstra.hpp"
#include <vector>
#include <queue>
#include <algorithm>

void PathFinderDijkstra::Find(const Graph& graph, const Graph::Vertex& src)
{
    Clear();
    typedef std::pair<Graph::Weight, Graph::Vertex> WeightVertexPair;
    std::priority_queue<WeightVertexPair, std::vector<WeightVertexPair>, std::greater<>> queue;
    for (const auto& it: graph.GetVerticesList()) {
        dist_map[it] = infinity_distance;
    }

    queue.push(make_pair(0, src));
    dist_map[src] = 0;

    while (!queue.empty()){
        auto current_vertex = queue.top().second;
        queue.pop();

        for (const auto & it : graph.GetAdjList(current_vertex)) {
            const auto& vertex = it.first;
            const auto& weight = it.second;
        	
            if (dist_map[current_vertex] + weight < dist_map[vertex]){
                dist_map[vertex] = dist_map[current_vertex] + weight;
                queue.push(make_pair(dist_map[vertex], vertex));
            	
                path_map[vertex].clear();
                std::transform(path_map[current_vertex].begin(), path_map[current_vertex].end(), 
                    std::back_inserter(path_map[vertex]), [](Graph::Vertex v) {return v; });
            	
                path_map[vertex].push_back(current_vertex);
            }
        }
    }
}

const Graph::Vertices& PathFinderDijkstra::GetShortestPath(const Graph::Vertex& dest) const
{
    if (path_map.find(dest) == path_map.end()) {
        return empty_path_list;
    }
    return path_map.at(dest);
}

Graph::Weight PathFinderDijkstra::GetShortestDistance(const Graph::Vertex& dest) const
{
    if (path_map.find(dest) == path_map.end()) {
        return infinity_distance;
    }
    return dist_map.at(dest);
}

void PathFinderDijkstra::Clear()
{
    dist_map.clear();
    path_map.clear();
}