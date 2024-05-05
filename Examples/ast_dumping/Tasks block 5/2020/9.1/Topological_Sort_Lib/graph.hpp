#pragma once
#include <unordered_map>
#include <vector>
#include <unordered_set>

class graph final
{
public:
	graph() = default;

	typedef std::string vertex;
	typedef std::vector<vertex> vertices;

	void add_vertex(const vertex& dest, const vertices& src);
	const vertices& dependent_vertices(const vertex& dest) const;
	const vertices& get_vertices() const;

	~graph() = default;
	
private:
	std::unordered_map<vertex, vertices> data_;
	vertices vertices_;
	std::unordered_set<vertex> vertices_set_;
	const static inline vertices empty;
};

