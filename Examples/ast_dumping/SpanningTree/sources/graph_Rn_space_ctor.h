#pragma once
#include "graph.h"
#include "parser_Rn_space.h"
#include "vertex_Rn_space.h"

namespace graph_io_ns
{
	class graph_Rn_space_ctor final
	{
	public:
		struct invalid_line final
		{
			size_t line_index{};
			std::string clarifying_msg{};
		};

	private:
		parser_Rn_space parser_{};
		std::vector<invalid_line> invalid_lines_encountered_{};

	public:
		template <size_t Dim>
		graph_ns::graph<graph_ns::vertex_Rn_space<std::string, Dim>> create(std::istream& input);

		[[nodiscard]] const std::vector<invalid_line>& invalid_lines_encountered() const;


	private:
		template <size_t Dim>
		std::unordered_set<graph_ns::vertex_Rn_space<std::string, Dim>> read_vertices(std::istream& input);

		template <size_t Dim>
		[[nodiscard]] std::unordered_set<graph_ns::edge<graph_ns::vertex_Rn_space<std::string, Dim>>> create_edges(
			const std::unordered_set<graph_ns::vertex_Rn_space<std::string, Dim>>& vertices) const;
	};

	inline const std::vector<graph_Rn_space_ctor::invalid_line>& graph_Rn_space_ctor::invalid_lines_encountered() const
	{
		return invalid_lines_encountered_;
	}


	template <size_t Dim>
	graph_ns::graph<graph_ns::vertex_Rn_space<std::string, Dim>> graph_Rn_space_ctor::create(std::istream& input)
	{
		invalid_lines_encountered_.clear();

		const auto vertices = read_vertices<Dim>(input);
		if (vertices.empty())
		{
			return {};
		}
		const auto edges = create_edges<Dim>(vertices);
		return graph_ns::graph(vertices, edges, false);
	}

	template <size_t Dim>
	std::unordered_set<graph_ns::vertex_Rn_space<std::string, Dim>> graph_Rn_space_ctor::read_vertices(std::istream& input)
	{
		std::unordered_set<graph_ns::vertex_Rn_space<std::string, Dim>> vertices{};
		for (size_t line_index = 0; ; ++line_index)
		{
			try
			{
				const auto params = parser_.get_vertex_params<Dim>(input);
				if (params.id.empty())
				{
					return vertices;
				}
				const auto& [_, inserted] = vertices.emplace(params.id, graph_ns::point_Rn_space<Dim>(params.coordinates));
				if (!inserted)
				{
					invalid_lines_encountered_.emplace_back(line_index + 1, "vertex redefinition attempt");
				}
			}
			catch (const invalid_line_exception& ex)
			{
				invalid_lines_encountered_.emplace_back(line_index + 1, ex.what());
			}
		}
	}

	template <size_t Dim>
	std::unordered_set<graph_ns::edge<graph_ns::vertex_Rn_space<std::string, Dim>>> graph_Rn_space_ctor::create_edges(
		const std::unordered_set<graph_ns::vertex_Rn_space<std::string, Dim>>& vertices) const
	{
		std::unordered_set<graph_ns::edge<graph_ns::vertex_Rn_space<std::string, Dim>>> edges{};
		for (auto vertex_begin_it = vertices.cbegin(); vertex_begin_it != vertices.cend(); ++vertex_begin_it)
		{
			for (auto vertex_end_it = vertex_begin_it; vertex_end_it != vertices.cend(); ++vertex_end_it)
			{
				if (vertex_begin_it == vertex_end_it)
				{
					continue;
				}
				edges.emplace(*vertex_begin_it, *vertex_end_it,
					calc_Rn_distance(vertex_begin_it->value(), vertex_end_it->value()), false);
			}
		}
		return edges;
	}

}
