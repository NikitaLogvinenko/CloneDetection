#include "pch.h"
#include "vertex_Rn_space.h"

using namespace graph_ns;

TEST(VertexR10SpaceString, Id)
{
	const std::string id_1 = "id_1";
	const std::string id_2 = "id_2";

	constexpr size_t n = 10;
	const std::array<double, n> arr_1 { 1,2,3,4,5,6,7,8,9,0 };
	constexpr std::array<double, n> arr_2 {} ;

	const point_Rn_space point_1(arr_1);
	const point_Rn_space point_2(arr_2);
	const vertex_Rn_space vertex_1{ id_1, point_1 };
	const vertex_Rn_space vertex_2{ id_1, point_2 };
	const vertex_Rn_space vertex_3{ id_2, point_1 };

	ASSERT_EQ(vertex_1.id(), id_1);
	ASSERT_EQ(vertex_2.id(), id_1);
	ASSERT_EQ(vertex_3.id(), id_2);
	ASSERT_EQ(vertex_1.id(), vertex_2.id());
	ASSERT_NE(vertex_1.id(), vertex_3.id());
}

TEST(VertexR10SpaceString, Value)
{
	const std::string id_1 = "id_1";
	const std::string id_2 = "id_2";
	const std::string id_3 = "id_3";
	const std::string id_4 = "id_4";

	constexpr size_t n = 10;
	const std::array<double, n> arr_1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	const std::array<double, n> arr_2{ 1, 2, 3, 5, 5, 5, 5, 5, 9, 0 };
	const std::array<double, n> arr_3{ 1.1, 2.1, 3.01, 0, 0, 0, 0, 0, 0, 0 };
	const std::array<double, n> arr_4{ 1.1, 2.1, 3.01, 1e-3, 1e-3, 1e-3, 0, 0, 0, 0 };

	const point_Rn_space point_1(arr_1);
	const point_Rn_space point_2(arr_2);
	const point_Rn_space point_3(arr_3);
	const point_Rn_space point_4(arr_4);

	const vertex_Rn_space vertex_1{ id_1, point_1 };
	const vertex_Rn_space vertex_2{ id_2, point_2 };
	const vertex_Rn_space vertex_3{ id_3, point_3 };
	const vertex_Rn_space vertex_4{ id_4, point_4 };

	ASSERT_EQ(vertex_1.value(), point_1);
	ASSERT_EQ(vertex_2.value(), point_2);
	ASSERT_EQ(vertex_3.value(), point_3);
	ASSERT_EQ(vertex_4.value(), point_4);
	ASSERT_NE(vertex_3.value(), vertex_4.value());
	ASSERT_NE(vertex_1.value(), vertex_2.value());
	ASSERT_NE(vertex_1.value(), vertex_3.value());
}

TEST(VertexR10SpaceString, EqualOperator)
{
	const std::string id_1 = "id_1";
	const std::string id_2 = "id_2";
	const std::string id_3 = "id_3";
	const std::string id_4 = "id_4";

	constexpr size_t n = 10;
	const std::array<double, n> arr_1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	const std::array<double, n> arr_2{ 1, 2, 3, 5, 5, 5, 5, 5, 9, 0 };
	const std::array<double, n> arr_3{ 1.1, 2.1, 3.01, 0, 0, 0, 0, 0, 0, 0 };
	const std::array<double, n> arr_4{ 1.1, 2.1, 3.01, 1e-3, 1e-3, 1e-3, 0, 0, 0, 0 };

	const point_Rn_space point_1(arr_1);
	const point_Rn_space point_2(arr_2);
	const point_Rn_space point_3(arr_3);
	const point_Rn_space point_4(arr_4);

	const vertex_Rn_space vertex_1{ id_1, point_1 };
	const vertex_Rn_space vertex_2{ id_2, point_2 };
	const vertex_Rn_space vertex_3{ id_3, point_3 };
	const vertex_Rn_space vertex_4{ id_4, point_4 };
	const vertex_Rn_space vertex_5{ id_4, point_1 };

	ASSERT_NE(vertex_1, vertex_2);
	ASSERT_NE(vertex_1, vertex_3);
	ASSERT_NE(vertex_1, vertex_3);
	ASSERT_NE(vertex_1, vertex_4);
	ASSERT_NE(vertex_1, vertex_5);
	ASSERT_NE(vertex_3, vertex_4);
	ASSERT_EQ(vertex_4, vertex_5);
}

TEST(VertexR10SpaceString, Hash)
{
	for (int i = -100; i != 100; ++i)
	{
		const std::string id = "id_" + std::to_string(i);
		constexpr size_t n = 10;
		const std::array<double, n> arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
		const point_Rn_space point(arr);
		const vertex_Rn_space vertex{ id, point };

		const size_t vertex_hash = std::hash<vertex_Rn_space<std::string, n>>{}(vertex);
		const size_t id_hash = std::hash<std::string>{}(id);
		ASSERT_EQ(vertex_hash, id_hash);
	}
}

TEST(VertexR20SpaceInt, Id)
{
	constexpr int id_1 = 1;
	constexpr int id_2 = 2;

	constexpr size_t n = 20;
	const std::array<double, n> arr_1{ 1,2,3,4,5,6,7,8,9,0 };
	constexpr std::array<double, n> arr_2{};

	const point_Rn_space point_1(arr_1);
	const point_Rn_space point_2(arr_2);

	const vertex_Rn_space vertex_1{ id_1, point_1 };
	const vertex_Rn_space vertex_2{ id_1, point_2 };
	const vertex_Rn_space vertex_3{ id_2, point_1 };

	ASSERT_EQ(vertex_1.id(), id_1);
	ASSERT_EQ(vertex_2.id(), id_1);
	ASSERT_EQ(vertex_3.id(), id_2);
	ASSERT_EQ(vertex_1.id(), vertex_2.id());
	ASSERT_NE(vertex_1.id(), vertex_3.id());
}

TEST(VertexR20SpaceInt, Value)
{
	constexpr int id_1 = 1;
	constexpr int id_2 = 2;
	constexpr int id_3 = 3;
	constexpr int id_4 = 4;

	constexpr size_t n = 20;
	const std::array<double, n> arr_1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	const std::array<double, n> arr_2{ 1, 2, 3, 5, 5, 5, 5, 5, 9, 0 };
	const std::array<double, n> arr_3{ 1.1, 2.1, 3.01, 0, 0, 0, 0, 0, 0, 0 };
	const std::array<double, n> arr_4{ 1.1, 2.1, 3.01, 1e-3, 1e-3, 1e-3, 0, 0, 0, 0 };

	const point_Rn_space point_1(arr_1);
	const point_Rn_space point_2(arr_2);
	const point_Rn_space point_3(arr_3);
	const point_Rn_space point_4(arr_4);

	const vertex_Rn_space vertex_1{ id_1, point_1 };
	const vertex_Rn_space vertex_2{ id_2, point_2 };
	const vertex_Rn_space vertex_3{ id_3, point_3 };
	const vertex_Rn_space vertex_4{ id_4, point_4 };

	ASSERT_EQ(vertex_1.value(), point_1);
	ASSERT_EQ(vertex_2.value(), point_2);
	ASSERT_EQ(vertex_3.value(), point_3);
	ASSERT_EQ(vertex_4.value(), point_4);
	ASSERT_NE(vertex_3.value(), vertex_4.value());
	ASSERT_NE(vertex_1.value(), vertex_2.value());
	ASSERT_NE(vertex_1.value(), vertex_3.value());
}

TEST(VertexR20SpaceInt, EqualOperator)
{
	constexpr int id_1 = 1;
	constexpr int id_2 = 2;
	constexpr int id_3 = 3;
	constexpr int id_4 = 4;

	constexpr size_t n = 20;
	const std::array<double, n> arr_1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	const std::array<double, n> arr_2{ 1, 2, 3, 5, 5, 5, 5, 5, 9, 0 };
	const std::array<double, n> arr_3{ 1.1, 2.1, 3.01, 0, 0, 0, 0, 0, 0, 0 };
	const std::array<double, n> arr_4{ 1.1, 2.1, 3.01, 1e-3, 1e-3, 1e-3, 0, 0, 0, 0 };

	const point_Rn_space point_1(arr_1);
	const point_Rn_space point_2(arr_2);
	const point_Rn_space point_3(arr_3);
	const point_Rn_space point_4(arr_4);

	const vertex_Rn_space vertex_1{ id_1, point_1 };
	const vertex_Rn_space vertex_2{ id_2, point_2 };
	const vertex_Rn_space vertex_3{ id_3, point_3 };
	const vertex_Rn_space vertex_4{ id_4, point_4 };
	const vertex_Rn_space vertex_5{ id_4, point_1 };

	ASSERT_NE(vertex_1, vertex_2);
	ASSERT_NE(vertex_1, vertex_3);
	ASSERT_NE(vertex_1, vertex_3);
	ASSERT_NE(vertex_1, vertex_4);
	ASSERT_NE(vertex_1, vertex_5);
	ASSERT_NE(vertex_3, vertex_4);
	ASSERT_EQ(vertex_4, vertex_5);
}

TEST(VertexR20SpaceInt, VertexHash)
{
	for (int i = -100; i != 100; ++i)
	{
		const int id = i;
		constexpr size_t n = 20;
		const std::array<double, n> arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
		const point_Rn_space point(arr);
		const vertex_Rn_space vertex{ id, point };

		const size_t vertex_hash = std::hash<vertex_Rn_space<int, n>>{}(vertex);
		const size_t id_hash = std::hash<int>{}(id);
		ASSERT_EQ(vertex_hash, id_hash);
	}
}
