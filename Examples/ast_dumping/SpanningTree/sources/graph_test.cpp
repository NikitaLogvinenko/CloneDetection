#include "pch.h"
#include "graph.h"
#include "vertex_Rn_space.h"

using namespace graph_ns;

class GraphR3SpaceTestFixing : public ::testing::Test {
protected:
	void SetUp() override
	{
		const std::array<double, 3> coords_1{ 1, 1, 1 };
		const std::array<double, 3> coords_2{ 2, 2, 2 };
		const std::array<double, 3> coords_3{ 3, 3, 3 };
		const std::array<double, 3> coords_4{ 4, 4, 4 };
		const std::array<double, 3> coords_5{ 5, 5, 5 };

		const point_Rn_space point_1(coords_1);
		const point_Rn_space point_2(coords_2);
		const point_Rn_space point_3(coords_3);
		const point_Rn_space point_4(coords_4);
		const point_Rn_space point_5(coords_5);

		const vertex_Rn_space<std::string, 3> v1("v1", point_1);
		const vertex_Rn_space<std::string, 3> v2("v2", point_2);
		const vertex_Rn_space<std::string, 3> v3("v3", point_3);
		const vertex_Rn_space<std::string, 3> v4("v4", point_4);
		const vertex_Rn_space<std::string, 3> v5("v5", point_5);

		const std::vector vertices_vector{ v1, v2, v3, v4, v5 };
		vertices_.insert(vertices_vector.cbegin(), vertices_vector.cend());

		for (auto vertex_begin_it = vertices_vector.cbegin(); vertex_begin_it != vertices_vector.cend(); ++vertex_begin_it)
		{
			for (auto vertex_end_it = vertices_vector.cbegin(); vertex_end_it != vertices_vector.cend(); ++vertex_end_it)
			{
				const edge edge_directed(*vertex_begin_it, *vertex_end_it, calc_Rn_distance(vertex_begin_it->value(), vertex_end_it->value()), true);
				const edge edge_not_directed(*vertex_begin_it, *vertex_end_it, calc_Rn_distance(vertex_begin_it->value(), vertex_end_it->value()), false);
				edges_directed_.insert(edge_directed);
				edges_not_directed_.insert(edge_not_directed);
			}
		}
		ASSERT_EQ(vertices_.size(), 5);
		ASSERT_EQ(edges_directed_.size(), 5 * 5);
		ASSERT_EQ(edges_not_directed_.size(), 15);
	}

	std::unordered_set<vertex_Rn_space<std::string, 3>> vertices_{};
	std::unordered_set<edge<vertex_Rn_space<std::string, 3>>> edges_directed_{};
	std::unordered_set<edge<vertex_Rn_space<std::string, 3>>> edges_not_directed_{};
};

TEST_F(GraphR3SpaceTestFixing, CtorWithoutArgs)
{
	const graph<vertex_Rn_space<std::string, 3>> graph{};
	ASSERT_TRUE(graph.vertices().empty());
	ASSERT_TRUE(graph.edges().empty());
	ASSERT_FALSE(graph.directed());
	ASSERT_EQ(graph.vertices_count(), 0);
	ASSERT_EQ(graph.edges_count(), 0);
}

TEST_F(GraphR3SpaceTestFixing, CtorDirected)
{
	const graph<vertex_Rn_space<std::string, 3>> graph_1{};
	ASSERT_TRUE(graph_1.vertices().empty());
	ASSERT_TRUE(graph_1.edges().empty());
	ASSERT_FALSE(graph_1.directed());
	ASSERT_EQ(graph_1.vertices_count(), 0);
	ASSERT_EQ(graph_1.edges_count(), 0);

	const graph<vertex_Rn_space<std::string, 3>> graph_2(true);
	ASSERT_TRUE(graph_2.vertices().empty());
	ASSERT_TRUE(graph_2.edges().empty());
	ASSERT_TRUE(graph_2.directed());
	ASSERT_EQ(graph_2.vertices_count(), 0);
	ASSERT_EQ(graph_2.edges_count(), 0);
}

TEST_F(GraphR3SpaceTestFixing, CtorVerticesEmpty)
{
	const graph<vertex_Rn_space<std::string, 3>> empty_vertices{};
	const graph empty_graph{ empty_vertices };
	ASSERT_TRUE(empty_graph.vertices().empty());
	ASSERT_TRUE(empty_graph.edges().empty());
	ASSERT_FALSE(empty_graph.directed());
	ASSERT_EQ(empty_graph.vertices_count(), 0);
	ASSERT_EQ(empty_graph.edges_count(), 0);
}

TEST_F(GraphR3SpaceTestFixing, CtorVerticesFilled)
{
	const graph filled_graph(vertices_);
	ASSERT_FALSE(filled_graph.vertices().empty());
	ASSERT_TRUE(filled_graph.edges().empty());
	ASSERT_FALSE(filled_graph.directed());
	ASSERT_EQ(filled_graph.vertices_count(), vertices_.size());
	ASSERT_EQ(filled_graph.edges_count(), 0);
}

TEST_F(GraphR3SpaceTestFixing, CtorVerticesDirectedAndNotDirected)
{
	const graph graph_directed(vertices_, true);
	ASSERT_FALSE(graph_directed.vertices().empty());
	ASSERT_TRUE(graph_directed.edges().empty());
	ASSERT_TRUE(graph_directed.directed());
	ASSERT_EQ(graph_directed.vertices_count(), vertices_.size());
	ASSERT_EQ(graph_directed.edges_count(), 0);

	const graph graph_not_directed(vertices_, false);
	ASSERT_FALSE(graph_not_directed.vertices().empty());
	ASSERT_TRUE(graph_not_directed.edges().empty());
	ASSERT_FALSE(graph_not_directed.directed());
	ASSERT_EQ(graph_not_directed.vertices_count(), vertices_.size());
	ASSERT_EQ(graph_not_directed.edges_count(), 0);
}

TEST_F(GraphR3SpaceTestFixing, CtorVerticesEmptyEdgesEmpty)
{
	const std::unordered_set<vertex_Rn_space<std::string, 3>> empty_vertices{};
	const std::unordered_set<edge<vertex_Rn_space<std::string, 3>>> empty_edges{};
	const graph empty_graph(empty_vertices, empty_edges);
	ASSERT_TRUE(empty_graph.vertices().empty());
	ASSERT_TRUE(empty_graph.edges().empty());
	ASSERT_FALSE(empty_graph.directed());
	ASSERT_EQ(empty_graph.vertices_count(), 0);
	ASSERT_EQ(empty_graph.edges_count(), 0);
}

TEST_F(GraphR3SpaceTestFixing, CtorValidVerticesValidEdgesNotDirected)
{
	const graph graph(vertices_, edges_not_directed_);
	ASSERT_FALSE(graph.vertices().empty());
	ASSERT_FALSE(graph.edges().empty());
	ASSERT_FALSE(graph.directed());
	ASSERT_EQ(graph.vertices_count(), vertices_.size());
	ASSERT_EQ(graph.edges_count(), edges_not_directed_.size());
}

TEST_F(GraphR3SpaceTestFixing, CtorValidVerticesValidEdgesDirected)
{
	const graph graph(vertices_, edges_directed_, true);
	ASSERT_FALSE(graph.vertices().empty());
	ASSERT_FALSE(graph.edges().empty());
	ASSERT_TRUE(graph.directed());
	ASSERT_EQ(graph.vertices_count(), vertices_.size());
	ASSERT_EQ(graph.edges_count(), edges_directed_.size());
}

TEST_F(GraphR3SpaceTestFixing, CtorEdgesIncompatibleDirection)
{
	edges_directed_.insert(*edges_not_directed_.begin());
	edges_not_directed_.insert(*edges_directed_.begin());

	ASSERT_THROW(const graph graph_directed(vertices_, edges_directed_, true), graph_incompatible_edge_direction);
	ASSERT_THROW(const graph graph_not_directed(vertices_, edges_not_directed_, false), graph_incompatible_edge_direction);
}

TEST_F(GraphR3SpaceTestFixing, CtorNonExistentVerticesInEdges)
{
	vertices_.erase(vertices_.begin());

	ASSERT_THROW(const graph graph_directed(vertices_, edges_directed_, true), graph_non_existent_vertices);
	ASSERT_THROW(const graph graph_not_directed(vertices_, edges_not_directed_, false), graph_non_existent_vertices);
}

TEST_F(GraphR3SpaceTestFixing, InsertVertices)
{
	graph graph_directed(vertices_, edges_directed_, true);
	graph graph_not_directed(vertices_, edges_not_directed_, false);
	const size_t initial_vertices_count = graph_directed.vertices_count();
	for (size_t vertices_added = 1; vertices_added != 100; ++vertices_added)
	{
		const std::array<double, 3> coords{ 0, 0, 0 };
		const point_Rn_space point(coords);
		const vertex_Rn_space new_vertex("new_vertex_" + std::to_string(vertices_added), point);

		ASSERT_TRUE(graph_directed.insert(new_vertex));
		ASSERT_TRUE(graph_not_directed.insert(new_vertex));
		ASSERT_EQ(initial_vertices_count + vertices_added, graph_directed.vertices_count());
		ASSERT_EQ(initial_vertices_count + vertices_added, graph_not_directed.vertices_count());

		ASSERT_FALSE(graph_directed.insert(new_vertex));
		ASSERT_FALSE(graph_not_directed.insert(new_vertex));
		ASSERT_EQ(initial_vertices_count + vertices_added, graph_directed.vertices_count());
		ASSERT_EQ(initial_vertices_count + vertices_added, graph_not_directed.vertices_count());
	}
}

TEST_F(GraphR3SpaceTestFixing, InsertEdgesExistedVerticesValidDirection)
{
	graph graph_directed(vertices_, true);
	size_t edges_added = 0;
	for (auto edge_directed_it = edges_directed_.cbegin(); edge_directed_it != edges_directed_.cend(); ++edge_directed_it)
	{
		ASSERT_TRUE(graph_directed.insert(*edge_directed_it, false));
		ASSERT_EQ(++edges_added, graph_directed.edges_count());
		ASSERT_FALSE(graph_directed.insert(*edge_directed_it, false));
		ASSERT_EQ(edges_added, graph_directed.edges_count());
	}

	graph graph_not_directed(vertices_, false);
	edges_added = 0;
	for (auto edge_not_directed_it = edges_not_directed_.cbegin(); edge_not_directed_it != edges_not_directed_.cend(); ++edge_not_directed_it)
	{
		ASSERT_TRUE(graph_not_directed.insert(*edge_not_directed_it, false));
		ASSERT_EQ(++edges_added, graph_not_directed.edges_count());
		ASSERT_FALSE(graph_not_directed.insert(*edge_not_directed_it, false));
		ASSERT_EQ(edges_added, graph_not_directed.edges_count());
	}
}

TEST_F(GraphR3SpaceTestFixing, InsertEdgesExistedVerticesInvalidDirection)
{
	graph graph_directed(vertices_, true);
	graph graph_not_directed(vertices_, false);
	for (auto edge_directed_it = edges_directed_.cbegin(); edge_directed_it != edges_directed_.cend(); ++edge_directed_it)
	{
		ASSERT_FALSE(graph_not_directed.insert(*edge_directed_it, false));
		ASSERT_EQ(0, graph_not_directed.edges_count());
	}

	for (auto edge_not_directed_it = edges_not_directed_.cbegin(); edge_not_directed_it != edges_not_directed_.cend(); ++edge_not_directed_it)
	{
		ASSERT_FALSE(graph_directed.insert(*edge_not_directed_it, false));
		ASSERT_EQ(0, graph_directed.edges_count());
	}
}

TEST_F(GraphR3SpaceTestFixing, InsertEdgesNotInsertNonExistentVertices)
{
	graph<vertex_Rn_space<std::string, 3>> graph_directed(true);
	graph<vertex_Rn_space<std::string, 3>> graph_not_directed(false);
	for (auto edge_directed_it = edges_directed_.cbegin(); edge_directed_it != edges_directed_.cend(); ++edge_directed_it)
	{
		ASSERT_FALSE(graph_directed.insert(*edge_directed_it, false));
		ASSERT_EQ(0, graph_directed.edges_count());
	}

	for (auto edge_not_directed_it = edges_not_directed_.cbegin(); edge_not_directed_it != edges_not_directed_.cend(); ++edge_not_directed_it)
	{
		ASSERT_FALSE(graph_not_directed.insert(*edge_not_directed_it, false));
		ASSERT_EQ(0, graph_not_directed.edges_count());
	}
}

TEST_F(GraphR3SpaceTestFixing, InsertEdgesInsertNonExistentVertices)
{
	graph<vertex_Rn_space<std::string, 3>> graph_directed(true);
	size_t edges_added = 0;
	for (auto edge_directed_it = edges_directed_.cbegin(); edge_directed_it != edges_directed_.cend(); ++edge_directed_it)
	{
		ASSERT_TRUE(graph_directed.insert(*edge_directed_it, true));
		ASSERT_EQ(++edges_added, graph_directed.edges_count());
		ASSERT_FALSE(graph_directed.insert(*edge_directed_it, true));
		ASSERT_EQ(edges_added, graph_directed.edges_count());
	}

	graph<vertex_Rn_space<std::string, 3>> graph_not_directed(false);
	edges_added = 0;
	for (auto edge_not_directed_it = edges_not_directed_.cbegin(); edge_not_directed_it != edges_not_directed_.cend(); ++edge_not_directed_it)
	{
		ASSERT_TRUE(graph_not_directed.insert(*edge_not_directed_it, true));
		ASSERT_EQ(++edges_added, graph_not_directed.edges_count());
		ASSERT_FALSE(graph_not_directed.insert(*edge_not_directed_it, true));
		ASSERT_EQ(edges_added, graph_not_directed.edges_count());
	}
}

