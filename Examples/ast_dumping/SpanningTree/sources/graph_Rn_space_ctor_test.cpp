#include "pch.h"
#include "graph_Rn_space_ctor.h"

using namespace graph_io_ns;
using namespace graph_ns;

TEST(GraphR2SpaceCtor, EmptyInput)
{
	std::istringstream input{};

	graph_Rn_space_ctor ctor;
	const auto graph = ctor.create<2>(input);
	ASSERT_EQ(graph.directed(), false);
	ASSERT_EQ(graph.vertices_count(), 0);
	ASSERT_EQ(graph.edges_count(), 0);
	ASSERT_TRUE(ctor.invalid_lines_encountered().empty());
}

TEST(GraphR2SpaceCtor, ValidLinesCreate)
{
	std::string v1("id1, 2, 3\n");
	std::string v2("id2, 1, 1\n");
	std::string v3("id3, 21, 3\n");
	std::string v4("id4, 21, 31\n");
	std::string v5("id5, -2, 30\n");
	std::string v6("id6, 3, 33\n");
	std::string v7("id7, 7, 7\n");

	std::istringstream input(v1 + v2 + v3 + v4 + v5 + v6 + v7);

	graph_Rn_space_ctor ctor;
	const auto graph = ctor.create<2>(input);
	ASSERT_EQ(graph.directed(), false);
	ASSERT_EQ(graph.vertices_count(), 7);
	ASSERT_EQ(graph.edges_count(), 7 * 6 / 2);
	ASSERT_TRUE(ctor.invalid_lines_encountered().empty());
}

TEST(GraphR2SpaceCtor, InvalidLinesEncountered)
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

	graph_Rn_space_ctor ctor;
	const auto graph = ctor.create<2>(input);
	ASSERT_EQ(graph.directed(), false);
	ASSERT_EQ(graph.vertices_count(), 1);
	ASSERT_EQ(graph.edges_count(), 0);

	const auto& invalid_lines = ctor.invalid_lines_encountered();
	ASSERT_EQ(invalid_lines.size(), 10);
	ASSERT_EQ(invalid_lines[0].line_index, 2);
	ASSERT_EQ(invalid_lines[0].clarifying_msg, "vertex redefinition attempt");
	ASSERT_EQ(invalid_lines[1].line_index, 3);
	ASSERT_EQ(invalid_lines[1].clarifying_msg, "vertex redefinition attempt");
	ASSERT_EQ(invalid_lines[2].line_index, 4);
	ASSERT_EQ(invalid_lines[2].clarifying_msg, "invalid id");
	ASSERT_EQ(invalid_lines[3].line_index, 5);
	ASSERT_EQ(invalid_lines[3].clarifying_msg, "coordinates are absent");
	ASSERT_EQ(invalid_lines[4].line_index, 6);
	ASSERT_EQ(invalid_lines[4].clarifying_msg, "dimension of the vertex is wrong");
	ASSERT_EQ(invalid_lines[5].line_index, 7);
	ASSERT_EQ(invalid_lines[5].clarifying_msg, "dimension of the vertex is wrong");
	ASSERT_EQ(invalid_lines[6].line_index, 8);
	ASSERT_EQ(invalid_lines[6].clarifying_msg, "invalid coordinate");
	ASSERT_EQ(invalid_lines[7].line_index, 9);
	ASSERT_EQ(invalid_lines[7].clarifying_msg, "invalid coordinate");
	ASSERT_EQ(invalid_lines[8].line_index, 10);
	ASSERT_EQ(invalid_lines[8].clarifying_msg, "invalid coordinate");
	ASSERT_EQ(invalid_lines[9].line_index, 11);
	ASSERT_EQ(invalid_lines[9].clarifying_msg, "vertex redefinition attempt");
}

TEST(GraphR2SpaceCtor, InvalidLinesEncounteredUpdatedEachCreating)
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

	std::istringstream input_1(v1 + v1_redefinition_1 + v1_redefinition_2 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v1_redefinition_3);
	std::istringstream input_2(v1 + v1_redefinition_1 + v1_redefinition_2 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v1_redefinition_3);

	graph_Rn_space_ctor ctor;
	ctor.create<2>(input_1);
	ASSERT_EQ(ctor.invalid_lines_encountered().size(), 10);
	ctor.create<2>(input_2);
	ASSERT_EQ(ctor.invalid_lines_encountered().size(), 10);

	const auto& invalid_lines = ctor.invalid_lines_encountered();
	ASSERT_EQ(invalid_lines[0].line_index, 2);
	ASSERT_EQ(invalid_lines[0].clarifying_msg, "vertex redefinition attempt");
	ASSERT_EQ(invalid_lines[1].line_index, 3);
	ASSERT_EQ(invalid_lines[1].clarifying_msg, "vertex redefinition attempt");
	ASSERT_EQ(invalid_lines[2].line_index, 4);
	ASSERT_EQ(invalid_lines[2].clarifying_msg, "invalid id");
	ASSERT_EQ(invalid_lines[3].line_index, 5);
	ASSERT_EQ(invalid_lines[3].clarifying_msg, "coordinates are absent");
	ASSERT_EQ(invalid_lines[4].line_index, 6);
	ASSERT_EQ(invalid_lines[4].clarifying_msg, "dimension of the vertex is wrong");
	ASSERT_EQ(invalid_lines[5].line_index, 7);
	ASSERT_EQ(invalid_lines[5].clarifying_msg, "dimension of the vertex is wrong");
	ASSERT_EQ(invalid_lines[6].line_index, 8);
	ASSERT_EQ(invalid_lines[6].clarifying_msg, "invalid coordinate");
	ASSERT_EQ(invalid_lines[7].line_index, 9);
	ASSERT_EQ(invalid_lines[7].clarifying_msg, "invalid coordinate");
	ASSERT_EQ(invalid_lines[8].line_index, 10);
	ASSERT_EQ(invalid_lines[8].clarifying_msg, "invalid coordinate");
	ASSERT_EQ(invalid_lines[9].line_index, 11);
	ASSERT_EQ(invalid_lines[9].clarifying_msg, "vertex redefinition attempt");
}
