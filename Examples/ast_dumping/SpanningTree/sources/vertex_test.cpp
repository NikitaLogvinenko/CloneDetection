#include "pch.h"
#include "vertex.h"
#include "custom_classes_for_tests.h"

TEST(VertexStringInt, VertexId)
{
	const std::string id_1 = "id_1";
	const std::string id_2 = "id_2";
	const graph_ns::vertex vertex_1{ id_1, 1 };
	const graph_ns::vertex vertex_2{ id_1, 2 };
	const graph_ns::vertex vertex_3{ id_2, 3 };

	ASSERT_EQ(vertex_1.id(), id_1);
	ASSERT_EQ(vertex_2.id(), id_1);
	ASSERT_EQ(vertex_3.id(), id_2);
	ASSERT_EQ(vertex_1.id(), vertex_2.id());
	ASSERT_NE(vertex_1.id(), vertex_3.id());
}

TEST(VertexStringInt, VertexValue)
{
	const std::string id_1 = "id_1";
	const std::string id_2 = "id_2";
	constexpr int value_1 = 1;
	constexpr int value_3 = 3;
	const graph_ns::vertex vertex_1{ id_1, value_1 };
	const graph_ns::vertex vertex_2{ id_2, value_1 };
	const graph_ns::vertex vertex_3{ id_2, value_3 };

	ASSERT_EQ(vertex_1.value(), value_1);
	ASSERT_EQ(vertex_2.value(), value_1);
	ASSERT_EQ(vertex_3.value(), value_3);
	ASSERT_EQ(vertex_1.value(), vertex_2.value());
	ASSERT_NE(vertex_2.value(), vertex_3.value());
}

TEST(VertexStringInt, VertexEqualOperator)
{
	const std::string id_1 = "id_1";
	const std::string id_2 = "id_2";
	constexpr int value_1 = 1;
	constexpr int value_3 = 3;
	const graph_ns::vertex vertex_1{ id_1, value_1 };
	const graph_ns::vertex vertex_2{ id_2, value_1 };
	const graph_ns::vertex vertex_3{ id_2, value_3 };

	ASSERT_FALSE(vertex_1 == vertex_2);
	ASSERT_FALSE(vertex_1 == vertex_3);
	ASSERT_TRUE(vertex_2 == vertex_3);
}

TEST(VertexStringInt, VertexHash)
{
	for (int i = -100; i != 100; ++i)
	{
		const std::string id = "id_" + std::to_string(i);
		const graph_ns::vertex vertex{ id, i };
		const size_t vertex_hash = std::hash<graph_ns::vertex<std::string, int>>{}(vertex);
		const size_t id_hash = std::hash<std::string>{}(id);
		ASSERT_EQ(vertex_hash, id_hash);
	}
}


TEST(VertexStringDouble, VertexId)
{
	const std::string id_1 = "id_1";
	const std::string id_2 = "id_2";
	const graph_ns::vertex vertex_1{ id_1, 1.0 };
	const graph_ns::vertex vertex_2{ id_1, 2.0 };
	const graph_ns::vertex vertex_3{ id_2, 3.0 };

	ASSERT_EQ(vertex_1.id(), id_1);
	ASSERT_EQ(vertex_2.id(), id_1);
	ASSERT_EQ(vertex_3.id(), id_2);
	ASSERT_EQ(vertex_1.id(), vertex_2.id());
	ASSERT_NE(vertex_1.id(), vertex_3.id());
}

TEST(VertexStringDouble, VertexValue)
{
	const std::string id_1 = "id_1";
	const std::string id_2 = "id_2";
	constexpr double value_1 = 1.0;
	constexpr double value_3 = 3.0;
	const graph_ns::vertex vertex_1{ id_1, value_1 };
	const graph_ns::vertex vertex_2{ id_2, value_1 };
	const graph_ns::vertex vertex_3{ id_2, value_3 };

	ASSERT_DOUBLE_EQ(vertex_1.value(), value_1);
	ASSERT_DOUBLE_EQ(vertex_2.value(), value_1);
	ASSERT_DOUBLE_EQ(vertex_3.value(), value_3);
	ASSERT_DOUBLE_EQ(vertex_1.value(), vertex_2.value());
	ASSERT_NE(vertex_2.value(), vertex_3.value());
}

TEST(VertexStringDouble, VertexEqualOperator)
{
	const std::string id_1 = "id_1";
	const std::string id_2 = "id_2";
	constexpr double value_1 = 1.0;
	constexpr double value_3 = 3.0;
	const graph_ns::vertex vertex_1{ id_1, value_1 };
	const graph_ns::vertex vertex_2{ id_2, value_1 };
	const graph_ns::vertex vertex_3{ id_2, value_3 };

	ASSERT_FALSE(vertex_1 == vertex_2);
	ASSERT_FALSE(vertex_1 == vertex_3);
	ASSERT_TRUE(vertex_2 == vertex_3);
}

TEST(VertexStringDouble, VertexHash)
{
	for (int i = -100; i != 100; ++i)
	{
		const double value = i;
		const std::string id = "id_" + std::to_string(i);
		const graph_ns::vertex vertex{ id, value };
		const size_t vertex_hash = std::hash<graph_ns::vertex<std::string, double>>{}(vertex);
		const size_t id_hash = std::hash<std::string>{}(id);
		ASSERT_EQ(vertex_hash, id_hash);
	}
}

TEST(VertexIntDouble, VertexId)
{
	constexpr int id_1 = 1;
	constexpr int id_2 = 2;
	const graph_ns::vertex vertex_1{ id_1, 1.0 };
	const graph_ns::vertex vertex_2{ id_1, 2.0 };
	const graph_ns::vertex vertex_3{ id_2, 3.0 };

	ASSERT_EQ(vertex_1.id(), id_1);
	ASSERT_EQ(vertex_2.id(), id_1);
	ASSERT_EQ(vertex_3.id(), id_2);
	ASSERT_EQ(vertex_1.id(), vertex_2.id());
	ASSERT_NE(vertex_1.id(), vertex_3.id());
}

TEST(VertexIntDouble, VertexValue)
{
	constexpr int id_1 = 1;
	constexpr int id_2 = 2;
	constexpr double value_1 = 1.0;
	constexpr double value_3 = 3.0;
	const graph_ns::vertex vertex_1{ id_1, value_1 };
	const graph_ns::vertex vertex_2{ id_2, value_1 };
	const graph_ns::vertex vertex_3{ id_2, value_3 };

	ASSERT_DOUBLE_EQ(vertex_1.value(), value_1);
	ASSERT_DOUBLE_EQ(vertex_2.value(), value_1);
	ASSERT_DOUBLE_EQ(vertex_3.value(), value_3);
	ASSERT_DOUBLE_EQ(vertex_1.value(), vertex_2.value());
	ASSERT_NE(vertex_2.value(), vertex_3.value());
}

TEST(VertexIntDouble, VertexEqualOperator)
{
	constexpr int id_1 = 1;
	constexpr int id_2 = 2;
	constexpr double value_1 = 1.0;
	constexpr double value_3 = 3.0;
	const graph_ns::vertex vertex_1{ id_1, value_1 };
	const graph_ns::vertex vertex_2{ id_2, value_1 };
	const graph_ns::vertex vertex_3{ id_2, value_3 };

	ASSERT_FALSE(vertex_1 == vertex_2);
	ASSERT_FALSE(vertex_1 == vertex_3);
	ASSERT_TRUE(vertex_2 == vertex_3);
}

TEST(VertexIntDouble, VertexHash)
{
	for (int i = -100; i != 100; ++i)
	{
		const double value = i + 1;
		const graph_ns::vertex vertex{ i, value };
		const size_t vertex_hash = std::hash<graph_ns::vertex<int, double>>{}(vertex);
		const size_t id_hash = std::hash<int>{}(i);
		ASSERT_EQ(vertex_hash, id_hash);
	}
}

TEST(VertexCustom, VertexId)
{
	const custom_id_for_tests id_1{ 1 };
	const custom_id_for_tests id_2{ 2 };
	const custom_value_for_tests value_1{ 1 };
	const custom_value_for_tests value_2{ 2 };
	const custom_value_for_tests value_3{ 3 };
	const graph_ns::vertex vertex_1{ id_1, value_1 };
	const graph_ns::vertex vertex_2{ id_1, value_2 };
	const graph_ns::vertex vertex_3{ id_2, value_3 };

	ASSERT_EQ(vertex_1.id(), id_1);
	ASSERT_EQ(vertex_2.id(), id_1);
	ASSERT_EQ(vertex_3.id(), id_2);
	ASSERT_EQ(vertex_1.id(), vertex_2.id());
	ASSERT_NE(vertex_1.id(), vertex_3.id());
}

TEST(VertexCustom, VertexValue)
{
	const custom_id_for_tests id_1{ 1 };
	const custom_id_for_tests id_2{ 2 };
	const custom_value_for_tests value_1{ 1 };
	const custom_value_for_tests value_3{ 3 };
		
	const graph_ns::vertex vertex_1{ id_1, value_1 };
	const graph_ns::vertex vertex_2{ id_2, value_1 };
	const graph_ns::vertex vertex_3{ id_2, value_3 };

	ASSERT_EQ(vertex_1.value(), value_1);
	ASSERT_EQ(vertex_2.value(), value_1);
	ASSERT_EQ(vertex_3.value(), value_3);
	ASSERT_EQ(vertex_1.value(), vertex_2.value());
	ASSERT_NE(vertex_2.value(), vertex_3.value());
}

TEST(VertexCustom, VertexEqualOperator)
{
	const custom_id_for_tests id_1{ 1 };
	const custom_id_for_tests id_2{ 2 };
	const custom_value_for_tests value_1{ 1 };
	const custom_value_for_tests value_3{ 3 };	
	
	const graph_ns::vertex vertex_1{ id_1, value_1 };
	const graph_ns::vertex vertex_2{ id_2, value_1 };
	const graph_ns::vertex vertex_3{ id_2, value_3 };

	ASSERT_FALSE(vertex_1 == vertex_2);
	ASSERT_FALSE(vertex_1 == vertex_3);
	ASSERT_TRUE(vertex_2 == vertex_3);
}

TEST(VertexCustom, VertexHash)
{
	for (int i = -100; i != 100; ++i)
	{
		const custom_id_for_tests id{ i };
		const custom_value_for_tests value{ i + 1 };
		const graph_ns::vertex vertex{ id, value };
		const size_t vertex_hash = std::hash<graph_ns::vertex<custom_id_for_tests, custom_value_for_tests>>{}(vertex);
		const size_t id_hash = std::hash<int>{}(i);
		ASSERT_EQ(vertex_hash, id_hash);
	}
}