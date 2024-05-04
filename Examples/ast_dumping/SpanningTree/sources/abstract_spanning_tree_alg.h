#pragma once
#include "graph.h"

namespace spanning_tree_ns
{
	 template <typename VertexT>
	 class abstract_spanning_tree_alg
	 {
	 public:
	 	[[nodiscard]] virtual std::vector<graph_ns::edge<VertexT>> operator()(const graph_ns::graph<VertexT>& graph) const = 0;
		virtual ~abstract_spanning_tree_alg() noexcept = default;

	protected:
		[[nodiscard]] static bool edges_weights_cmp(const graph_ns::edge<VertexT>& edge_1, const graph_ns::edge<VertexT>& edge_2) noexcept;
	};

	 template <typename VertexT>
	 bool abstract_spanning_tree_alg<VertexT>::edges_weights_cmp(const graph_ns::edge<VertexT>& edge_1,
		 const graph_ns::edge<VertexT>& edge_2) noexcept
	 {
    	 return edge_1.weight() < edge_2.weight();
	 }
}
