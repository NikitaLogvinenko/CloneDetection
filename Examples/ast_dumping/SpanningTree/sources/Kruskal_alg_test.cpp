#include "pch.h"
#include "Kruskal_alg.h"
#include "vertex_Rn_space.h"
#include <numeric>

using namespace spanning_tree_ns;
using namespace graph_ns;

namespace
{
	template <typename VertexT>
	void Kruskal_alg_validation(const std::unordered_set<VertexT>& vertices, const std::unordered_set<edge<VertexT>>& edges, 
		const double correct_weight, const double acceptable_error = 1e-4)
	{
		const graph graph(vertices, edges, false);
		const auto spanning_tree_edges = Kruskal_alg<VertexT>()(graph);
		ASSERT_EQ(spanning_tree_edges.size(), vertices.size() - 1);

		disjoint_set<VertexT> ds_vertices{};
		std::vector<double> spanning_tree_weights{};
		spanning_tree_weights.reserve(spanning_tree_edges.size());
		for (const auto& edge : spanning_tree_edges)
		{
			spanning_tree_weights.push_back(edge.weight());
			const auto subset_begin = ds_vertices.insert(edge.vertex_begin());
			const auto subset_end = ds_vertices.insert(edge.vertex_end());
			ASSERT_NE(subset_begin, subset_end);
		}
		const double min_weight = std::accumulate(spanning_tree_weights.cbegin(), spanning_tree_weights.cend(), 0.0);
		ASSERT_NEAR(min_weight, correct_weight, acceptable_error);
	}
}

TEST(KruskalAlgInvalidCase, DirectedGraph)
{
	std::unordered_set<vertex<int, int>> vertices{};
	vertices.reserve(10);
	for (int i = 0; i < 10; ++i)
	{
		vertices.emplace(i, i);
	}

	std::unordered_set<edge<vertex<int, int>>> edges{};
	for (const auto& vertex_begin : vertices)
	{
		for (const auto& vertex_end : vertices)
		{
			edges.emplace(vertex_begin, vertex_end, vertex_end.value() - vertex_begin.value(), true);
		}
	}

	const graph directed_graph_without_edges(vertices, true);
	const graph directed_graph_all_edges(vertices, edges, true);
	const bool no_spanning_tree_directed_graph_without_edges = Kruskal_alg<vertex<int, int>>()(directed_graph_without_edges).empty();
	const bool no_spanning_tree_directed_graph_all_edges = Kruskal_alg<vertex<int, int>>()(directed_graph_all_edges).empty();
	ASSERT_TRUE(no_spanning_tree_directed_graph_without_edges);
	ASSERT_TRUE(no_spanning_tree_directed_graph_all_edges);
}

TEST(KruskalAlgInvalidCase, EmptyGraph)
{
	const graph<vertex<int, int>> empty_graph{};
	const bool no_spanning_tree = Kruskal_alg<vertex<int, int>>()(empty_graph).empty();
	ASSERT_TRUE(no_spanning_tree);
}

TEST(KruskalAlgInvalidCase, GraphSingleVertex)
{
	std::unordered_set<vertex<int, int>> vertices{};
	vertices.reserve(1);
	for (int i = 0; i < 1; ++i)
	{
		vertices.emplace(i, i);
	}

	const graph graph(vertices, false);
	const bool no_spanning_tree = Kruskal_alg<vertex<int, int>>()(graph).empty();
	ASSERT_TRUE(no_spanning_tree);
}

TEST(KruskalAlgInvalidCase, EdgesLessThanVerticesMinusOne)
{
	std::unordered_set<vertex<int, int>> vertices{};
	vertices.reserve(100);
	for (int i = 0; i < 100; ++i)
	{
		vertices.emplace(i, i);
	}

	std::unordered_set<edge<vertex<int, int>>> edges{};
	size_t vertex_begin_index = 0;
	for (auto vertex_begin_it = vertices.cbegin(); vertex_begin_index + 2 != vertices.size(); ++vertex_begin_index)
	{
		edges.emplace(*vertex_begin_it, *(++vertex_begin_it), 0, false);
	}

	const graph graph(vertices, edges, false);
	ASSERT_EQ(graph.vertices_count(), graph.edges_count() + 2);
	const bool no_spanning_tree = Kruskal_alg<vertex<int, int>>()(graph).empty();
	ASSERT_TRUE(no_spanning_tree);
}

TEST(KruskalAlgInvalidCase, DisconnectedGraph)
{
	std::unordered_set<vertex<int, int>> vertices{};
	vertices.reserve(20);
	for (int i = 0; i < 20; ++i)
	{
		vertices.emplace(i, i);
	}

	for (size_t max_vertex_first_subgraph = 0; max_vertex_first_subgraph + 1 < vertices.size(); ++max_vertex_first_subgraph)
	{
		// First connectivity component
		std::unordered_set<edge<vertex<int, int>>> edges{};
		auto vertex_begin_it = vertices.cbegin();
		for (size_t vertex_begin_index = 0; vertex_begin_index <= max_vertex_first_subgraph; ++vertex_begin_it, ++vertex_begin_index)
		{
			size_t vertex_end_index = vertex_begin_index;
			for (auto vertex_end_it = vertex_begin_it; vertex_end_index <= max_vertex_first_subgraph; ++vertex_end_it, ++vertex_end_index)
			{
				edges.emplace(*vertex_begin_it, *vertex_end_it, 0, false);
			}
		}

		// Second connectivity component
		for (; vertex_begin_it != vertices.cend(); ++vertex_begin_it)
		{
			for (auto vertex_end_it = vertex_begin_it; vertex_end_it != vertices.cend(); ++vertex_end_it)
			{
				edges.emplace(*vertex_begin_it, *vertex_end_it, 0, false);
			}
		}

		const graph graph(vertices, edges, false);
		const bool no_spanning_tree_two_connectivity_components = Kruskal_alg<vertex<int, int>>()(graph).empty();
		ASSERT_TRUE(no_spanning_tree_two_connectivity_components);
	}
}

TEST(KruskalAlgValidCase, TwoVertices)
{
	std::unordered_set<vertex<int, int>> vertices{};
	vertex vertex_1(1, 1);
	vertex vertex_2(2, 2);
	vertices.insert(vertex_1);
	vertices.insert(vertex_2);

	std::unordered_set<edge<vertex<int, int>>> edges{};
	edges.emplace(vertex_1, vertex_2, 777, false);
	
	const graph graph(vertices, edges, false);
	const auto spanning_tree_edges = Kruskal_alg<vertex<int, int>>()(graph);
	ASSERT_EQ(spanning_tree_edges.size(), 1);
	ASSERT_DOUBLE_EQ(spanning_tree_edges.begin()->weight(), 777);
}

TEST(KruskalAlgValidCase, FullyConnectedGraph1)
{
	std::unordered_set<vertex<int, int>> vertices{};
	std::vector<vertex<int, int>> vertices_vector{};
	vertices.reserve(7);
	vertices_vector.reserve(7);
	for (int i = 0; i < 7; ++i)
	{
		vertices.emplace(i, i);
		vertices_vector.emplace_back(i, i);
	}
	std::unordered_set<edge<vertex<int, int>>> edges{};
	edges.emplace(vertices_vector[0], vertices_vector[1], 18, false);
	edges.emplace(vertices_vector[0], vertices_vector[2], 22, false);
	edges.emplace(vertices_vector[0], vertices_vector[3], 17, false);
	edges.emplace(vertices_vector[0], vertices_vector[4], 19, false);
	edges.emplace(vertices_vector[0], vertices_vector[5], 13, false);
	edges.emplace(vertices_vector[0], vertices_vector[6], 23, false);

	edges.emplace(vertices_vector[1], vertices_vector[2], 23, false);
	edges.emplace(vertices_vector[1], vertices_vector[3], 20, false);
	edges.emplace(vertices_vector[1], vertices_vector[4], 21, false);
	edges.emplace(vertices_vector[1], vertices_vector[5], 25, false);
	edges.emplace(vertices_vector[1], vertices_vector[6], 11, false);

	edges.emplace(vertices_vector[2], vertices_vector[3], 18, false);
	edges.emplace(vertices_vector[2], vertices_vector[4], 24, false);
	edges.emplace(vertices_vector[2], vertices_vector[5], 12, false);
	edges.emplace(vertices_vector[2], vertices_vector[6], 28, false);

	edges.emplace(vertices_vector[3], vertices_vector[4], 18, false);
	edges.emplace(vertices_vector[3], vertices_vector[5], 21, false);
	edges.emplace(vertices_vector[3], vertices_vector[6], 14, false);

	edges.emplace(vertices_vector[4], vertices_vector[5], 19, false);
	edges.emplace(vertices_vector[4], vertices_vector[6], 25, false);

	edges.emplace(vertices_vector[5], vertices_vector[6], 15, false);

	Kruskal_alg_validation(vertices, edges, 83);
}

TEST(KruskalAlgValidCase, NotFullyConnectedGraph1)
{
	std::unordered_set<vertex<int, int>> vertices{};
	std::vector<vertex<int, int>> vertices_vector{};
	vertices.reserve(5);
	vertices_vector.reserve(5);
	for (int i = 0; i < 5; ++i)
	{
		vertices.emplace(i, i);
		vertices_vector.emplace_back(i, i);
	}
	std::unordered_set<edge<vertex<int, int>>> edges{};
	edges.emplace(vertices_vector[0], vertices_vector[1], 3, false);
	edges.emplace(vertices_vector[0], vertices_vector[4], 1, false);
	edges.emplace(vertices_vector[1], vertices_vector[2], 5, false);
	edges.emplace(vertices_vector[1], vertices_vector[4], 4, false);
	edges.emplace(vertices_vector[2], vertices_vector[3], 2, false);
	edges.emplace(vertices_vector[2], vertices_vector[4], 6, false);
	edges.emplace(vertices_vector[3], vertices_vector[4], 7, false);

	Kruskal_alg_validation(vertices, edges, 11);
}

TEST(KruskalAlgValidCase, NotFullyConnectedGraph2)
{
	std::unordered_set<vertex<int, int>> vertices{};
	std::vector<vertex<int, int>> vertices_vector{};
	vertices.reserve(6);
	vertices_vector.reserve(6);
	for (int i = 0; i < 6; ++i)
	{
		vertices.emplace(i, i);
		vertices_vector.emplace_back(i, i);
	}
	std::unordered_set<edge<vertex<int, int>>> edges{};
	edges.emplace(vertices_vector[0], vertices_vector[1], 7, false);
	edges.emplace(vertices_vector[0], vertices_vector[2], 8, false);
	edges.emplace(vertices_vector[1], vertices_vector[2], 11, false);
	edges.emplace(vertices_vector[1], vertices_vector[3], 2, false);
	edges.emplace(vertices_vector[2], vertices_vector[3], 6, false);
	edges.emplace(vertices_vector[2], vertices_vector[4], 9, false);
	edges.emplace(vertices_vector[3], vertices_vector[4], 11, false);
	edges.emplace(vertices_vector[3], vertices_vector[5], 9, false);
	edges.emplace(vertices_vector[4], vertices_vector[5], 10, false);

	Kruskal_alg_validation(vertices, edges, 33);
}

TEST(KruskalAlgValidCase, GraphR2Space1)
{
	std::unordered_set<vertex_Rn_space<std::string, 2>> vertices{};
	std::vector<vertex_Rn_space<std::string, 2>> vertices_vector{};
	vertices.reserve(7);
	vertices_vector.reserve(7);
	vertices_vector.emplace_back(std::to_string(1), point_Rn_space(std::array<double, 2>{ 2.5,	1	}));
	vertices_vector.emplace_back(std::to_string(2), point_Rn_space(std::array<double, 2>{ 1,	3	}));
	vertices_vector.emplace_back(std::to_string(3), point_Rn_space(std::array<double, 2>{ 4.3,	2	}));
	vertices_vector.emplace_back(std::to_string(4), point_Rn_space(std::array<double, 2>{ 6,	4	}));
	vertices_vector.emplace_back(std::to_string(5), point_Rn_space(std::array<double, 2>{ 3.5,	3.5 }));
	vertices_vector.emplace_back(std::to_string(6), point_Rn_space(std::array<double, 2>{ 0,	0	}));
	vertices_vector.emplace_back(std::to_string(7), point_Rn_space(std::array<double, 2>{ 0,	7	}));
	for (int i = 0; i < 7; ++i)
	{
		vertices.insert(vertices_vector[i]);
	}

	std::unordered_set<edge<vertex_Rn_space<std::string, 2>>> edges{};
	for (const auto& vertex_begin : vertices)
	{
		for (const auto& vertex_end : vertices)
		{
			edges.emplace(vertex_begin, vertex_end, calc_Rn_distance(vertex_begin.value(), vertex_end.value()), false);
		}
	}

	Kruskal_alg_validation(vertices, edges, 15.62, 1e-2);
}

TEST(KruskalAlgValidCase, GraphR3Space1)
{
	std::unordered_set<vertex_Rn_space<std::string, 3>> vertices{};
	std::vector<vertex_Rn_space<std::string, 3>> vertices_vector{};
	vertices.reserve(5);
	vertices_vector.reserve(5);
	vertices_vector.emplace_back(std::to_string(1), point_Rn_space(std::array<double, 3>{ 1,	1,	1	}));
	vertices_vector.emplace_back(std::to_string(2), point_Rn_space(std::array<double, 3>{ 2,	1,	3	}));
	vertices_vector.emplace_back(std::to_string(3), point_Rn_space(std::array<double, 3>{ 4,	5,	7	}));
	vertices_vector.emplace_back(std::to_string(4), point_Rn_space(std::array<double, 3>{ 9,	-5, -3	}));
	vertices_vector.emplace_back(std::to_string(5), point_Rn_space(std::array<double, 3>{ -1,	-1, -1	}));

	for (int i = 0; i < 5; ++i)
	{
		vertices.insert(vertices_vector[i]);
	}

	std::unordered_set<edge<vertex_Rn_space<std::string, 3>>> edges{};
	for (const auto& vertex_begin : vertices)
	{
		for (const auto& vertex_end : vertices)
		{
			edges.emplace(vertex_begin, vertex_end, calc_Rn_distance(vertex_begin.value(), vertex_end.value()), false);
		}
	}

	Kruskal_alg_validation(vertices, edges, 22.47, 1e-2);
}