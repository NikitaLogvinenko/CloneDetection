#pragma once
#include <stdexcept>

namespace graph_ns
{
	class graph_invalid_arg : public std::exception
	{
	public:
		[[nodiscard]] const char* what() const noexcept override = 0;
	};

	class graph_incompatible_edge_direction final : public graph_invalid_arg
	{
	public:
		[[nodiscard]] const char* what() const noexcept override { return "edge.direction() incompatible with graph.direction()"; }
	};

	class graph_non_existent_vertices final : public graph_invalid_arg
	{
	public:
		[[nodiscard]] const char* what() const noexcept override { return "graph doesn't contain edge's vertices"; }
	};
}
