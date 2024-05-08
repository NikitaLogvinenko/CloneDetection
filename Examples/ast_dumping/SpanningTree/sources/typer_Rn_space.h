#pragma once
#include "graph_Rn_space_ctor.h"
#include <ostream>

namespace graph_io_ns
{
	class typer_Rn_space final
	{
	public:
		template <size_t Dim>
		void operator()(std::ostream& output, 
			const std::vector<graph_ns::edge<graph_ns::vertex_Rn_space<std::string, Dim>>>& spanning_tree_edges, 
			const std::vector<graph_Rn_space_ctor::invalid_line>& invalid_lines) const;
	};

	template <size_t Dim>
	void typer_Rn_space::operator()(std::ostream& output,
		const std::vector<graph_ns::edge<graph_ns::vertex_Rn_space<std::string, Dim>>>& spanning_tree_edges,
		const std::vector<graph_Rn_space_ctor::invalid_line>& invalid_lines) const
	{
		if (spanning_tree_edges.empty())
		{
			output << "It is impossibly to build a spanning tree. Try to change input data\n";
		}
		else
		{
			output << "Spanning tree edges:\n";
			for (const auto& edge : spanning_tree_edges)
			{
				output << edge.vertex_begin().id() << " - " << edge.vertex_end().id() << "\n";
			}
		}
		if (invalid_lines.empty())
		{
			return;
		}

		output << "\nInvalid lines:\n";
		for (const auto& [line_index, clarifying_msg] : invalid_lines)
		{
			output << "[" << line_index << "] " << clarifying_msg << "\n";
		}
	}
}
