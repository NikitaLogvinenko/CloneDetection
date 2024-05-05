#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Graph final {
public:
	Graph() = default;
	~Graph() = default;

	typedef std::string vertex;
	typedef std::vector<vertex> vertexes;
	const vertexes& dependent_vertices(const vertex& dest) const;
	const vertexes& get_vertices() const;
	void add_vertex(const vertex& dest, const vertexes& src);

private:
	std::unordered_map<vertex, vertexes> data_;
	std::unordered_set<vertex> vertexesSet;
	vertexes _vertexes;
	const static  inline vertexes empty;
};