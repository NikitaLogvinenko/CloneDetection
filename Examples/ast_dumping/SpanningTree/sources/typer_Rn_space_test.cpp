#include "pch.h"
#include "typer_Rn_space.h"

using namespace graph_io_ns;
using namespace graph_ns;

TEST(Typer, NoSpanningTreeNoInvalidLines)
{
	std::ostringstream output;
	const std::vector<edge<vertex_Rn_space<std::string, 2>>> empty_spanning_tree{};
	const std::vector<graph_Rn_space_ctor::invalid_line> empty_invalid_lines{};

	constexpr typer_Rn_space typer;
	typer(output, empty_spanning_tree, empty_invalid_lines);
	ASSERT_EQ(output.str(), "It is impossibly to build a spanning tree. Try to change input data\n");
}

TEST(Typer, NoSpanningTreeExistInvalidLines)
{
	std::string v1("id1, 2, 3\n");
	std::string v1_redefinition_1("id1, 2, 3\n");
	std::string v1_redefinition_2("id1, 3, 4\n");
	std::string v2(",,, ;;;\n");
	std::string v3("id3\n");
	std::string v4("  \n"); // just will be skipped
	std::string v5("id5, -2, 30, 3\n");
	std::string v6("id6, 3\n");
	std::string v7("id7, 1e1000, 7\n");
	std::string v8("id8, 1, 1e-1000\n");
	std::string v9("id9, 1, a2_2,\n");
	std::string v1_redefinition_3("id1, 1, 1\n");

	std::istringstream input(v1 + v1_redefinition_1 + v1_redefinition_2 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v1_redefinition_3);

	std::string expected_output{};
	expected_output += "It is impossibly to build a spanning tree. Try to change input data\n";
	expected_output += "\nInvalid lines:\n";
	expected_output += "[2] vertex redefinition attempt\n";
	expected_output += "[3] vertex redefinition attempt\n";
	expected_output += "[4] invalid id\n";
	expected_output += "[5] coordinates are absent\n";
	expected_output += "[6] dimension of the vertex is wrong\n";
	expected_output += "[7] dimension of the vertex is wrong\n";
	expected_output += "[8] invalid coordinate\n";
	expected_output += "[9] invalid coordinate\n";
	expected_output += "[10] invalid coordinate\n";
	expected_output += "[11] vertex redefinition attempt\n";


	graph_Rn_space_ctor ctor;
	ctor.create<2>(input);

	const std::vector<edge<vertex_Rn_space<std::string, 2>>> empty_spanning_tree{};
	const auto& invalid_lines = ctor.invalid_lines_encountered();
	std::ostringstream output;

	constexpr typer_Rn_space typer;
	typer(output, empty_spanning_tree, invalid_lines);

	ASSERT_EQ(output.str(), expected_output);
}

TEST(Typer, ExistsSpanningTreeNoInvalidLines)
{
	std::vector<vertex_Rn_space<std::string, 3>> vertices_vector{};
	vertices_vector.reserve(5);
	vertices_vector.emplace_back(std::to_string(1), point_Rn_space(std::array<double, 3>{ 0, 0, 0 }));
	vertices_vector.emplace_back(std::to_string(2), point_Rn_space(std::array<double, 3>{ 0, 0, 0 }));
	vertices_vector.emplace_back(std::to_string(3), point_Rn_space(std::array<double, 3>{ 0, 0, 0 }));
	vertices_vector.emplace_back(std::to_string(4), point_Rn_space(std::array<double, 3>{ 0, 0, 0 }));
	vertices_vector.emplace_back(std::to_string(5), point_Rn_space(std::array<double, 3>{ 0, 0, 0 }));

	std::vector<edge<vertex_Rn_space<std::string, 3>>> edges{};
	edges.emplace_back(vertices_vector[0], vertices_vector[1], 0, false);
	edges.emplace_back(vertices_vector[1], vertices_vector[2], 0, false);
	edges.emplace_back(vertices_vector[2], vertices_vector[3], 0, false);
	edges.emplace_back(vertices_vector[3], vertices_vector[4], 0, false);

	std::ostringstream output;
	const std::vector<graph_Rn_space_ctor::invalid_line> empty_invalid_lines{};

	constexpr typer_Rn_space typer;
	typer(output, edges, empty_invalid_lines);
	ASSERT_EQ(output.str(), "Spanning tree edges:\n1 - 2\n2 - 3\n3 - 4\n4 - 5\n");
}

TEST(Typer, ExistsSpanningTreeExistInvalidLines)
{
	std::vector<vertex_Rn_space<std::string, 3>> vertices_vector{};
	vertices_vector.reserve(5);
	vertices_vector.emplace_back(std::to_string(1), point_Rn_space(std::array<double, 3>{ 0, 0, 0 }));
	vertices_vector.emplace_back(std::to_string(2), point_Rn_space(std::array<double, 3>{ 0, 0, 0 }));
	vertices_vector.emplace_back(std::to_string(3), point_Rn_space(std::array<double, 3>{ 0, 0, 0 }));
	vertices_vector.emplace_back(std::to_string(4), point_Rn_space(std::array<double, 3>{ 0, 0, 0 }));
	vertices_vector.emplace_back(std::to_string(5), point_Rn_space(std::array<double, 3>{ 0, 0, 0 }));

	std::vector<edge<vertex_Rn_space<std::string, 3>>> edges{};
	edges.emplace_back(vertices_vector[0], vertices_vector[1], 0, false);
	edges.emplace_back(vertices_vector[1], vertices_vector[2], 0, false);
	edges.emplace_back(vertices_vector[2], vertices_vector[3], 0, false);
	edges.emplace_back(vertices_vector[3], vertices_vector[4], 0, false);


	std::string v1("id1, 2, 3\n");
	std::string v1_redefinition_1("id1, 2, 3\n");
	std::string v1_redefinition_2("id1, 3, 4\n");
	std::string v2(",,, ;;;\n");
	std::string v3("id3\n");
	std::string v4("  \n"); // just will be skipped
	std::string v5("id5, -2, 30, 3\n");
	std::string v6("id6, 3\n");
	std::string v7("id7, 1e1000, 7\n");
	std::string v8("id8, 1, 1e-1000\n");
	std::string v9("id9, 1, a2_2,\n");
	std::string v1_redefinition_3("id1, 1, 1\n");

	std::istringstream input(v1 + v1_redefinition_1 + v1_redefinition_2 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v1_redefinition_3);

	graph_Rn_space_ctor ctor;
	ctor.create<2>(input);
	const auto& invalid_lines = ctor.invalid_lines_encountered();

	std::ostringstream output;
	constexpr typer_Rn_space typer;
	typer(output, edges, invalid_lines);

	std::string expected_output{};
	expected_output += "Spanning tree edges:\n";
	expected_output += "1 - 2\n2 - 3\n3 - 4\n4 - 5\n";
	expected_output += "\nInvalid lines:\n";
	expected_output += "[2] vertex redefinition attempt\n";
	expected_output += "[3] vertex redefinition attempt\n";
	expected_output += "[4] invalid id\n";
	expected_output += "[5] coordinates are absent\n";
	expected_output += "[6] dimension of the vertex is wrong\n";
	expected_output += "[7] dimension of the vertex is wrong\n";
	expected_output += "[8] invalid coordinate\n";
	expected_output += "[9] invalid coordinate\n";
	expected_output += "[10] invalid coordinate\n";
	expected_output += "[11] vertex redefinition attempt\n";

	ASSERT_EQ(output.str(), expected_output);
}