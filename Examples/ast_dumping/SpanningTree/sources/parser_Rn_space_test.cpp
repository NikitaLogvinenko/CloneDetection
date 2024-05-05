#include "pch.h"
#include "parser_Rn_space.h"

using namespace graph_io_ns;

TEST(InvalidLines, EmptyLine)
{
	std::istringstream empty_iss{};
	const auto empty_params = parser_Rn_space().get_vertex_params<1>(empty_iss);
	ASSERT_TRUE(empty_params.id.empty());
}

TEST(InvalidLines, InvalidId)
{
	std::istringstream iss{",,, ;;;"};
	ASSERT_THROW(parser_Rn_space().get_vertex_params<1>(iss), invalid_id_exception);
}

TEST(InvalidLines, NoCoordinates)
{
	std::istringstream iss{"id"};
	ASSERT_THROW(parser_Rn_space().get_vertex_params<1>(iss), no_coords_exception);
}

TEST(InvalidLines, ExcessiveCoordinates)
{
	std::istringstream iss{ "id, 1, 2, 3, 4, 5" };
	ASSERT_THROW(parser_Rn_space().get_vertex_params<3>(iss), wrong_dim_exception);
}

TEST(InvalidLines, FewCoordinates)
{
	std::istringstream iss{ "id, 1, 2," };
	ASSERT_THROW(parser_Rn_space().get_vertex_params<3>(iss), wrong_dim_exception);
}

TEST(InvalidLines, InvalidCoordinates)
{
	std::istringstream iss{ "	id1  ,	  1.abs, 2, 3\n	id2,    1, a2_2, 3,\n	id3,    1, 2, -+3-\n" };
	const auto params = parser_Rn_space().get_vertex_params<3>(iss);
	ASSERT_EQ(params.id, "id1  ");
	ASSERT_DOUBLE_EQ(params.coordinates[0], 1);
	ASSERT_DOUBLE_EQ(params.coordinates[1], 2);
	ASSERT_DOUBLE_EQ(params.coordinates[2], 3);
	ASSERT_THROW(parser_Rn_space().get_vertex_params<3>(iss), invalid_coord_exception);
	ASSERT_THROW(parser_Rn_space().get_vertex_params<3>(iss), invalid_coord_exception);
	ASSERT_TRUE(parser_Rn_space().get_vertex_params<3>(iss).id.empty());
}

TEST(InvalidLines, TooBigCoordinates)
{
	std::istringstream iss{ "id1, 1e1000, 2, 3" };
	ASSERT_THROW(parser_Rn_space().get_vertex_params<3>(iss), invalid_coord_exception);
}

TEST(InvalidLines, TooFewCoordinates)
{
	std::istringstream iss{ "id1, 1e-1000, 2, 3" };
	ASSERT_THROW(parser_Rn_space().get_vertex_params<3>(iss), invalid_coord_exception);
}

TEST(InvalidLines, ValidLines)
{
	std::istringstream iss{ "	id1	, 1.1	,	 2.2	, 3.3	\n id2, -1.2   , -2_a, -3+\n id3, 1e2, 2.1e1, -10e-1\n" };
	auto params = parser_Rn_space().get_vertex_params<3>(iss);
	ASSERT_EQ(params.id, "id1	");
	ASSERT_DOUBLE_EQ(params.coordinates[0], 1.1);
	ASSERT_DOUBLE_EQ(params.coordinates[1], 2.2);
	ASSERT_DOUBLE_EQ(params.coordinates[2], 3.3);

	params = parser_Rn_space().get_vertex_params<3>(iss);
	ASSERT_EQ(params.id, "id2");
	ASSERT_DOUBLE_EQ(params.coordinates[0], -1.2);
	ASSERT_DOUBLE_EQ(params.coordinates[1], -2);
	ASSERT_DOUBLE_EQ(params.coordinates[2], -3);

	params = parser_Rn_space().get_vertex_params<3>(iss);
	ASSERT_EQ(params.id, "id3");
	ASSERT_DOUBLE_EQ(params.coordinates[0], 100);
	ASSERT_DOUBLE_EQ(params.coordinates[1], 21);
	ASSERT_DOUBLE_EQ(params.coordinates[2], -1);
}