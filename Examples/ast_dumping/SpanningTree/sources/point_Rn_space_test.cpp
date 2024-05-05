#include "pch.h"
#include "vertex_Rn_space.h"

using namespace graph_ns;

TEST(PointR20Space, Ctor)
{
	constexpr size_t dim = 20;
	constexpr std::array<double, dim> coords{};
	const point_Rn_space point(coords);
	ASSERT_EQ(dim, point.dim());
	ASSERT_EQ(coords, point.coordinates());
}

TEST(PointR20Space, EqualOperator)
{
	constexpr size_t dim = 20;
	constexpr std::array<double, dim> coords_1{};
	constexpr std::array<double, dim> coords_2{ 1 };
	const point_Rn_space point_1(coords_1);
	const point_Rn_space point_2(coords_1);
	const point_Rn_space point_3(coords_2);
	ASSERT_EQ(point_1, point_2);
	ASSERT_NE(point_1, point_3);
}

TEST(PointR20Space, ApproximatelyEqual)
{
	constexpr size_t dim = 20;
	constexpr std::array<double, dim> coords_1{ 1, 2, 3 };
	constexpr double slight_mismatch = 1e-5;
	constexpr double negligible_difference = slight_mismatch * 2;
	constexpr std::array<double, dim> coords_2{ 1 + slight_mismatch, 2, 3 };
	constexpr std::array<double, dim> coords_3{ 1, 2 - slight_mismatch, 3 };
	constexpr std::array<double, dim> coords_4{ 1 + slight_mismatch + negligible_difference, 2, 3 };
	constexpr std::array<double, dim> coords_5{ 1, 2, 3 - slight_mismatch - negligible_difference };
	const point_Rn_space point_1(coords_1);
	const point_Rn_space point_2(coords_2);
	const point_Rn_space point_3(coords_3);
	const point_Rn_space point_4(coords_4);
	const point_Rn_space point_5(coords_5);
	ASSERT_TRUE(point_1.approximately_equal(point_2, negligible_difference));
	ASSERT_TRUE(point_1.approximately_equal(point_3, negligible_difference));
	ASSERT_FALSE(point_1.approximately_equal(point_4, negligible_difference));
	ASSERT_FALSE(point_1.approximately_equal(point_5, negligible_difference));
}

TEST(PointR20Space, CalcDistance)
{
	constexpr size_t dim = 3;
	constexpr std::array coords_1{ 1.1, 2.2, 3.3 };
	const point_Rn_space point_1(coords_1);
	constexpr std::array coords_2{ 4.7, 5.7, 7.7 };
	const point_Rn_space point_2(coords_2);
	const point_Rn_space point_3(coords_2);

	const double distance = sqrt(pow(coords_1[0] - coords_2[0], 2)
								+ pow(coords_1[1] - coords_2[1], 2)
								+ pow(coords_1[2] - coords_2[2], 2));
	const double distance_by_func = calc_Rn_distance(point_1, point_2);
	ASSERT_DOUBLE_EQ(distance, distance_by_func);
	ASSERT_DOUBLE_EQ(0, calc_Rn_distance(point_2, point_3));
}