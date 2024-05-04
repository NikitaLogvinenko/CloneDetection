#include "pch.h"
#include "edge.h"
#include "custom_classes_for_tests.h"
#include "vertex_Rn_space.h"

using namespace graph_ns;

TEST(EdgeStringInt, Ctor4Args)
{
	const vertex<std::string, int> vertex_1("v1", 1);
	const vertex<std::string, int> vertex_2("v2", 2);
	constexpr double weight = 10;

	const edge edge_1(vertex_1, vertex_2, weight, true);
	const edge edge_2(vertex_2, vertex_1, weight, false);

	ASSERT_EQ(edge_1.vertex_begin(), vertex_1);
	ASSERT_EQ(edge_1.vertex_end(), vertex_2);
	ASSERT_EQ(edge_1.weight(), weight);
	ASSERT_TRUE(edge_1.directed());

	ASSERT_EQ(edge_2.vertex_begin(), vertex_2);
	ASSERT_EQ(edge_2.vertex_end(), vertex_1);
	ASSERT_EQ(edge_2.weight(), weight);
	ASSERT_FALSE(edge_2.directed());	
}

TEST(EdgeStringInt, CtorDefaultDirected)
{
	const vertex<std::string, int> vertex_1("v1", 1);
	const vertex<std::string, int> vertex_2("v2", 2);
	constexpr double weight = 10;

	const edge edge(vertex_1, vertex_2, weight);

	ASSERT_EQ(edge.vertex_begin(), vertex_1);
	ASSERT_EQ(edge.vertex_end(), vertex_2);
	ASSERT_EQ(edge.weight(), weight);
	ASSERT_FALSE(edge.directed());
}

TEST(EdgeStringInt, CtorVertex1IsVertex2)
{
	const vertex<std::string, int> vertex("v1", 1);
	constexpr double weight = 10;

	const edge edge(vertex, vertex, weight);

	ASSERT_EQ(edge.vertex_begin(), vertex);
	ASSERT_EQ(edge.vertex_end(), vertex);
	ASSERT_EQ(edge.weight(), weight);
	ASSERT_FALSE(edge.directed());
}

TEST(EdgeStringInt, EqualOperatorEqual)
{
	const vertex<std::string, int> vertex_1("v1", 1);
	const vertex<std::string, int> vertex_2("v2", 2);
	constexpr double weight = 10;

	const edge edge_1(vertex_1, vertex_2, weight, true);
	const edge edge_2(vertex_1, vertex_2, weight, true);
	ASSERT_TRUE(edge_1 == edge_2);

	const edge edge_3(vertex_1, vertex_2, weight, false);
	const edge edge_4(vertex_1, vertex_2, weight, false);
	ASSERT_TRUE(edge_3 == edge_4);
}

TEST(EdgeStringInt, EqualOperatorDirected)
{
	const vertex<std::string, int> vertex_1("v1", 1);
	const vertex<std::string, int> vertex_2("v2", 2);
	constexpr double weight = 10;
	
	const edge edge_1(vertex_1, vertex_2, weight, true);
	const edge edge_2(vertex_1, vertex_2, weight, false);

	ASSERT_FALSE(edge_1 == edge_2);
}

TEST(EdgeStringInt, EqualOperatorWeight)
{
	const vertex<std::string, int> vertex_1("v1", 1);
	const vertex<std::string, int> vertex_2("v2", 2);
	constexpr double weight_1 = 10;
	constexpr double weight_2 = 20;

	const edge edge_1_directed(vertex_1, vertex_2, weight_1, true);
	const edge edge_2_directed(vertex_1, vertex_2, weight_2, true);
	ASSERT_FALSE(edge_1_directed == edge_2_directed);

	const edge edge_1_not_directed(vertex_1, vertex_2, weight_1, false);
	const edge edge_2_not_directed(vertex_1, vertex_2, weight_2, false);
	ASSERT_FALSE(edge_1_not_directed == edge_2_not_directed);
}

TEST(EdgeStringInt, EqualOperatorVertexes)
{
	const vertex<std::string, int> vertex_1("v1", 1);
	const vertex<std::string, int> vertex_2("v2", 2);
	const vertex<std::string, int> vertex_2_copy = vertex_2;
	const vertex<std::string, int> vertex_3("v3", 3);
	constexpr double weight = 10;
	
	const edge edge_1_directed(vertex_1, vertex_2, weight, true);
	const edge edge_2_directed(vertex_2, vertex_1, weight, true);
	ASSERT_FALSE(edge_1_directed == edge_2_directed);
	const edge edge_3_directed(vertex_1, vertex_2_copy, weight, true);
	ASSERT_TRUE(edge_1_directed == edge_3_directed);
	const edge edge_4_directed(vertex_1, vertex_3, weight, true);
	ASSERT_FALSE(edge_1_directed == edge_4_directed);
	const edge edge_5_directed(vertex_3, vertex_2_copy, weight, true);
	ASSERT_FALSE(edge_1_directed == edge_5_directed);

	const edge edge_1_not_directed(vertex_1, vertex_2, weight, false);
	const edge edge_2_not_directed(vertex_2_copy, vertex_1, weight, false);
	ASSERT_TRUE(edge_1_not_directed == edge_2_not_directed);
	const edge edge_3_not_directed(vertex_1, vertex_2_copy, weight, false);
	ASSERT_TRUE(edge_1_not_directed == edge_3_not_directed);
	const edge edge_4_not_directed(vertex_1, vertex_3, weight, false);
	ASSERT_FALSE(edge_1_not_directed == edge_4_not_directed);
	const edge edge_5_not_directed(vertex_3, vertex_2_copy, weight, false);
	ASSERT_FALSE(edge_1_not_directed == edge_5_not_directed);
}

TEST(EdgeStringInt, Hash)
{
	const vertex<std::string, int> vertex_1("v1", 1);
	const vertex<std::string, int> vertex_2("v2", 2);
	constexpr double weight = 10;
	const edge edge_directed(vertex_1, vertex_2, weight, true);
	const edge edge_not_directed(vertex_1, vertex_2, weight, false);

	const size_t sum_of_vertexes_hashes = std::hash<vertex<std::string, int>>{}(vertex_1)+std::hash<vertex<std::string, int>>{}(vertex_2);
	const size_t directed_hash = std::hash<edge<vertex<std::string, int>>>{}(edge_directed);
	const size_t not_directed_hash = std::hash<edge<vertex<std::string, int>>>{}(edge_not_directed);

	ASSERT_EQ(sum_of_vertexes_hashes, directed_hash);
	ASSERT_EQ(sum_of_vertexes_hashes, not_directed_hash);
}


TEST(EdgeCustomCustom, Ctor4Args)
{
	const vertex vertex_1(custom_id_for_tests(1), custom_value_for_tests(1));
	const vertex vertex_2(custom_id_for_tests(2), custom_value_for_tests(2));
	constexpr double weight = 10;

	const edge edge_1(vertex_1, vertex_2, weight, true);
	const edge edge_2(vertex_2, vertex_1, weight, false);

	ASSERT_EQ(edge_1.vertex_begin(), vertex_1);
	ASSERT_EQ(edge_1.vertex_end(), vertex_2);
	ASSERT_EQ(edge_1.weight(), weight);
	ASSERT_TRUE(edge_1.directed());

	ASSERT_EQ(edge_2.vertex_begin(), vertex_2);
	ASSERT_EQ(edge_2.vertex_end(), vertex_1);
	ASSERT_EQ(edge_2.weight(), weight);
	ASSERT_FALSE(edge_2.directed());
}

TEST(EdgeCustomCustom, CtorDefaultDirected)
{
	const vertex vertex_1(custom_id_for_tests(1), custom_value_for_tests(1));
	const vertex vertex_2(custom_id_for_tests(2), custom_value_for_tests(2));
	constexpr double weight = 10;

	const edge edge(vertex_1, vertex_2, weight);

	ASSERT_EQ(edge.vertex_begin(), vertex_1);
	ASSERT_EQ(edge.vertex_end(),  vertex_2);
	ASSERT_EQ(edge.weight(), weight);
	ASSERT_FALSE(edge.directed());
}

TEST(EdgeCustomCustom, CtorVertex1IsVertex2)
{
	const vertex vertex(custom_id_for_tests(1), custom_value_for_tests(1));
	constexpr double weight = 10;

	const edge edge(vertex, vertex, weight);

	ASSERT_EQ(edge.vertex_begin(), vertex);
	ASSERT_EQ(edge.vertex_end(),  vertex);
	ASSERT_EQ(edge.weight(), weight);
	ASSERT_FALSE(edge.directed());
}

TEST(EdgeCustomCustom, EqualOperatorEqual)
{
	const vertex vertex_1(custom_id_for_tests(1), custom_value_for_tests(1));
	const vertex vertex_2(custom_id_for_tests(2), custom_value_for_tests(2));
	constexpr double weight = 10;

	const edge edge_1(vertex_1, vertex_2, weight, true);
	const edge edge_2(vertex_1, vertex_2, weight, true);
	ASSERT_TRUE(edge_1 == edge_2);

	const edge edge_3(vertex_1, vertex_2, weight, false);
	const edge edge_4(vertex_1, vertex_2, weight, false);
	ASSERT_TRUE(edge_3 == edge_4);
}

TEST(EdgeCustomCustom, EqualOperatorDirected)
{
	const vertex vertex_1(custom_id_for_tests(1), custom_value_for_tests(1));
	const vertex vertex_2(custom_id_for_tests(2), custom_value_for_tests(2));
	constexpr double weight = 10;

	const edge edge_1(vertex_1, vertex_2, weight, true);
	const edge edge_2(vertex_1, vertex_2, weight, false);

	ASSERT_FALSE(edge_1 == edge_2);
}

TEST(EdgeCustomCustom, EqualOperatorWeight)
{
	const vertex vertex_1(custom_id_for_tests(1), custom_value_for_tests(1));
	const vertex vertex_2(custom_id_for_tests(2), custom_value_for_tests(2));
	constexpr double weight_1 = 10;
	constexpr double weight_2 = 20;

	const edge edge_1_directed(vertex_1, vertex_2, weight_1, true);
	const edge edge_2_directed(vertex_1, vertex_2, weight_2, true);
	ASSERT_FALSE(edge_1_directed == edge_2_directed);

	const edge edge_1_not_directed(vertex_1, vertex_2, weight_1, false);
	const edge edge_2_not_directed(vertex_1, vertex_2, weight_2, false);
	ASSERT_FALSE(edge_1_not_directed == edge_2_not_directed);
}

TEST(EdgeCustomCustom, EqualOperatorVertexes)
{
	const vertex vertex_1(custom_id_for_tests(1), custom_value_for_tests(1));
	const vertex vertex_2(custom_id_for_tests(2), custom_value_for_tests(2));
	const vertex vertex_2_copy = vertex_2;
	const vertex vertex_3(custom_id_for_tests(3), custom_value_for_tests(3));
	constexpr double weight = 10;

	const edge edge_1_directed(vertex_1, vertex_2, weight, true);
	const edge edge_2_directed(vertex_2, vertex_1, weight, true);
	ASSERT_FALSE(edge_1_directed == edge_2_directed);
	const edge edge_3_directed(vertex_1, vertex_2_copy, weight, true);
	ASSERT_TRUE(edge_1_directed == edge_3_directed);
	const edge edge_4_directed(vertex_1, vertex_3, weight, true);
	ASSERT_FALSE(edge_1_directed == edge_4_directed);
	const edge edge_5_directed(vertex_3, vertex_2_copy, weight, true);
	ASSERT_FALSE(edge_1_directed == edge_5_directed);

	const edge edge_1_not_directed(vertex_1, vertex_2, weight, false);
	const edge edge_2_not_directed(vertex_2_copy, vertex_1, weight, false);
	ASSERT_TRUE(edge_1_not_directed == edge_2_not_directed);
	const edge edge_3_not_directed(vertex_1, vertex_2_copy, weight, false);
	ASSERT_TRUE(edge_1_not_directed == edge_3_not_directed);
	const edge edge_4_not_directed(vertex_1, vertex_3, weight, false);
	ASSERT_FALSE(edge_1_not_directed == edge_4_not_directed);
	const edge edge_5_not_directed(vertex_3, vertex_2_copy, weight, false);
	ASSERT_FALSE(edge_1_not_directed == edge_5_not_directed);
}

TEST(EdgeCustomCustom, Hash)
{
	const vertex vertex_1(custom_id_for_tests(1), custom_value_for_tests(1));
	const vertex vertex_2(custom_id_for_tests(2), custom_value_for_tests(2));
	constexpr double weight = 10;
	const edge edge_directed(vertex_1, vertex_2, weight, true);
	const edge edge_not_directed(vertex_1, vertex_2, weight, false);

	const size_t sum_of_vertexes_hashes = std::hash<vertex<custom_id_for_tests, custom_value_for_tests>>{}(vertex_1)+std::hash<vertex<custom_id_for_tests, custom_value_for_tests>>{}(vertex_2);
	const size_t directed_hash = std::hash<edge<vertex<custom_id_for_tests, custom_value_for_tests>>>{}(edge_directed);
	const size_t not_directed_hash = std::hash<edge<vertex<custom_id_for_tests, custom_value_for_tests>>>{}(edge_not_directed);

	ASSERT_EQ(sum_of_vertexes_hashes, directed_hash);
	ASSERT_EQ(sum_of_vertexes_hashes, not_directed_hash);
}


TEST(EdgeStringPointR3Space, Ctor4Args)
{
	constexpr size_t dim = 3;
	const vertex vertex_1(std::string("v1"), point_Rn_space<dim>({1,2,3}));
	const vertex vertex_2(std::string("v2"), point_Rn_space<dim>({7,8,9}));

	const double weight = calc_Rn_distance(vertex_1.value(), vertex_2.value());
	const edge edge_1(vertex_1, vertex_2, weight, true);
	const edge edge_2(vertex_2, vertex_1, weight, false);

	ASSERT_EQ(edge_1.vertex_begin(), vertex_1);
	ASSERT_EQ(edge_1.vertex_end(), vertex_2);
	ASSERT_DOUBLE_EQ(edge_1.weight(), weight);
	ASSERT_TRUE(edge_1.directed());

	ASSERT_EQ(edge_2.vertex_begin(), vertex_2);
	ASSERT_EQ(edge_2.vertex_end(), vertex_1);
	ASSERT_DOUBLE_EQ(edge_2.weight(), weight);
	ASSERT_FALSE(edge_2.directed());
}

TEST(EdgeStringPointR3Space, CtorDefaultDirected)
{
	constexpr size_t dim = 3;
	const vertex vertex_1(std::string("v1"), point_Rn_space<dim>({ 1,2,3 }));
	const vertex vertex_2(std::string("v2"), point_Rn_space<dim>({ 7,8,9 }));
	const double weight = calc_Rn_distance(vertex_1.value(), vertex_2.value());
	const edge edge(vertex_1, vertex_2, weight);

	ASSERT_EQ(edge.vertex_begin(), vertex_1);
	ASSERT_EQ(edge.vertex_end(), vertex_2);
	ASSERT_DOUBLE_EQ(edge.weight(), weight);
	ASSERT_FALSE(edge.directed());
}

TEST(EdgeStringPointR3Space, CtorVertex1IsVertex2)
{
	constexpr size_t dim = 3;
	const vertex vertex(std::string("v1"), point_Rn_space<dim>({ 1,2,3 }));
	const double weight = calc_Rn_distance(vertex.value(), vertex.value());
	const edge edge(vertex, vertex, weight);

	ASSERT_EQ(edge.vertex_begin(), vertex);
	ASSERT_EQ(edge.vertex_end(), vertex);
	ASSERT_DOUBLE_EQ(edge.weight(), weight);
	ASSERT_DOUBLE_EQ(edge.weight(), 0);
	ASSERT_FALSE(edge.directed());
}

TEST(EdgeStringPointR3Space, EqualOperatorEqual)
{
	constexpr size_t dim = 3;
	const vertex vertex_1(std::string("v1"), point_Rn_space<dim>({ 1,2,3 }));
	const vertex vertex_2(std::string("v2"), point_Rn_space<dim>({ 7,8,9 }));
	const double weight = calc_Rn_distance(vertex_1.value(), vertex_2.value());

	const edge edge_1(vertex_1, vertex_2, weight, true);
	const edge edge_2(vertex_1, vertex_2, weight, true);
	ASSERT_TRUE(edge_1 == edge_2);

	const edge edge_3(vertex_1, vertex_2, weight, false);
	const edge edge_4(vertex_1, vertex_2, weight, false);
	ASSERT_TRUE(edge_3 == edge_4);
}

TEST(EdgeStringPointR3Space, EqualOperatorDirected)
{
	constexpr size_t dim = 3;
	const vertex vertex_1(std::string("v1"), point_Rn_space<dim>({ 1,2,3 }));
	const vertex vertex_2(std::string("v2"), point_Rn_space<dim>({ 7,8,9 }));
	const double weight = calc_Rn_distance(vertex_1.value(), vertex_2.value());

	const edge edge_1(vertex_1, vertex_2, weight, true);
	const edge edge_2(vertex_1, vertex_2, weight, false);

	ASSERT_FALSE(edge_1 == edge_2);
}

TEST(EdgeStringPointR3Space, EqualOperatorVertexes)
{
	constexpr size_t dim = 3;
	const vertex vertex_1(std::string("v1"), point_Rn_space<dim>({ 1,2,3 }));
	const vertex vertex_2(std::string("v2"), point_Rn_space<dim>({ 7,8,9 }));
	const vertex vertex_2_copy = vertex_2;
	const double weight_12 = calc_Rn_distance(vertex_1.value(), vertex_2.value());
	
	const edge edge_1_directed(vertex_1, vertex_2, weight_12, true);
	const edge edge_2_directed(vertex_2, vertex_1, weight_12, true);
	ASSERT_FALSE(edge_1_directed == edge_2_directed);

	const edge edge_3_directed(vertex_1, vertex_2_copy, weight_12, true);
	ASSERT_TRUE(edge_1_directed == edge_3_directed);

	const edge edge_1_not_directed(vertex_1, vertex_2, weight_12, false);
	const edge edge_2_not_directed(vertex_2_copy, vertex_1, weight_12, false);
	ASSERT_TRUE(edge_1_not_directed == edge_2_not_directed);
	const edge edge_3_not_directed(vertex_1, vertex_2_copy, weight_12, false);
	ASSERT_TRUE(edge_1_not_directed == edge_3_not_directed);
}

TEST(EdgeStringPointR3Space, Hash)
{
	constexpr size_t dim = 3;
	const vertex vertex_1(std::string("v1"), point_Rn_space<dim>({ 1,2,3 }));
	const vertex vertex_2(std::string("v2"), point_Rn_space<dim>({ 7,8,9 }));
	const double weight = calc_Rn_distance(vertex_1.value(), vertex_2.value());
	const edge edge_directed(vertex_1, vertex_2, weight, true);
	const edge edge_not_directed(vertex_1, vertex_2, weight, false);

	const size_t string_hash = std::hash<std::string>{}(vertex_1.id()) + std::hash<std::string>{}(vertex_2.id());
	const size_t directed_hash = std::hash<edge<vertex_Rn_space<std::string, dim>>>{}(edge_directed);
	const size_t not_directed_hash = std::hash<edge<vertex_Rn_space<std::string, dim>>>{}(edge_not_directed);

	ASSERT_EQ(string_hash, directed_hash);
	ASSERT_EQ(string_hash, not_directed_hash);
}