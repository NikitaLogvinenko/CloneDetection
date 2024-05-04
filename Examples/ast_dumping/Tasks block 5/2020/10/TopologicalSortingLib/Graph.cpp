#include "Graph.hpp"

void Graph::add_vertex(const vertex& dest, const vertexes& src)
{
	for (const auto& n : src) {
		data_[n].push_back(dest);

		if (vertexesSet.find(n) == vertexesSet.end()) {
			_vertexes.push_back(n);
			vertexesSet.insert(n);
		}
	}
	vertexesSet.insert(dest);
	if (vertexesSet.find(dest) == vertexesSet.end()) {
		_vertexes.push_back(dest);
		vertexesSet.insert(dest);
	}
}

const Graph::vertexes& Graph::dependent_vertices(const vertex& dest) const
{
	const auto& found = data_.find(dest);
	if (found == data_.end()) {
		return empty;
	}
	return found->second;
}

const Graph::vertexes& Graph::get_vertices() const
{
	return _vertexes;
}