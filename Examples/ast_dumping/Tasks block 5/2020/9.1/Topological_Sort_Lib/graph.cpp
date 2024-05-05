#include "graph.hpp"

void graph::add_vertex(const vertex& dest, const vertices& src)
{
	for (const auto& n : src){
		if (vertices_set_.find(n) == vertices_set_.end()) {
			vertices_set_.insert(n);
			vertices_.push_back(n);
		}
		data_[n].push_back(dest);
	}
	if (vertices_set_.find(dest) == vertices_set_.end()) {
		vertices_set_.insert(dest);
		vertices_.push_back(dest);
	}
}

const graph::vertices& graph::dependent_vertices(const vertex& dest) const
{
	const auto& found = data_.find(dest);
	if (found == data_.end()){
		return empty;
	}
	return found->second;
}

const graph::vertices& graph::get_vertices() const
{
	return vertices_;
}