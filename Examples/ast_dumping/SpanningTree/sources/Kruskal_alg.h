#pragma once
#include "abstract_spanning_tree_alg.h"
#include "disjoint_set.h"
#include <algorithm>

namespace spanning_tree_ns
{
	template <typename VertexT>
   class Kruskal_alg final : abstract_spanning_tree_alg<VertexT>
	{
	public:
		[[nodiscard]] std::vector<graph_ns::edge<VertexT>> operator()(const graph_ns::graph<VertexT>& graph) const override;
	};

	template <typename VertexT>
	std::vector<graph_ns::edge<VertexT>> Kruskal_alg<VertexT>::operator()(const graph_ns::graph<VertexT>& graph) const
	{
		if (graph.directed() || graph.vertices_count() <= 1 || graph.edges_count() < graph.vertices_count() - 1)
		{
			return {};
		}

		std::vector<graph_ns::edge<VertexT>> spanning_tree{};
		disjoint_set<VertexT> vertices_disjoint;
		const auto& edges = graph.edges();
		std::vector<graph_ns::edge<VertexT>> edges_sorted{ edges.cbegin(), edges.cend() };
		std::sort(edges_sorted.begin(), edges_sorted.end(), abstract_spanning_tree_alg<VertexT>::edges_weights_cmp);

		size_t edges_remain = graph.edges_count();
		for (auto edge_it = edges_sorted.cbegin();
			edge_it != edges_sorted.cend() && edges_remain + spanning_tree.size() >= graph.vertices_count() - 1;
			++edge_it, --edges_remain)
		{
			const auto& vertex_begin = edge_it->vertex_begin();
			const auto& vertex_end = edge_it->vertex_end();
			const typename disjoint_set<VertexT>::subset vertex_begin_subset = vertices_disjoint.insert(vertex_begin);
			const typename disjoint_set<VertexT>::subset vertex_end_subset = vertices_disjoint.insert(vertex_end);

			if (vertex_begin_subset == vertex_end_subset)
			{
				continue;
			}

			spanning_tree.push_back(*edge_it);
			vertices_disjoint.join(vertex_begin_subset, vertex_end_subset);

			if (spanning_tree.size() == graph.vertices_count() - 1)
			{
				return spanning_tree;
			}
		}
		return {};
	}
}
