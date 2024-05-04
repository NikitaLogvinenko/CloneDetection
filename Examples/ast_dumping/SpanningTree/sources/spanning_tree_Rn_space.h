#pragma once
#include "graph_Rn_space_ctor.h"
#include "typer_Rn_space.h"
#include "Kruskal_alg.h"

namespace graph_io_ns
{
	template <size_t Dim>
	class spanning_tree_Rn_space final
	{
		graph_Rn_space_ctor ctor_{};
		typer_Rn_space typer_{};
	public:
		void operator()(std::istream& input, std::ostream& output);
	};

	template <size_t Dim>
	void spanning_tree_Rn_space<Dim>::operator()(std::istream& input, std::ostream& output)
	{
		const graph_ns::graph graph(ctor_.create<Dim>(input));
		const auto invalid_lines = ctor_.invalid_lines_encountered();
		const auto spanning_tree_edges = spanning_tree_ns::Kruskal_alg<graph_ns::vertex_Rn_space<std::string, Dim>>{}(graph);
		typer_(output, spanning_tree_edges, invalid_lines);
	}
}
