#include "pch.h"
#include "spanning_tree_Rn_space.h"

using namespace graph_io_ns;
using namespace graph_ns;

TEST(SpanningTreeR2Space, EmptyInput)
{
	std::istringstream input{};
	std::ostringstream output{};
	spanning_tree_Rn_space<2>{}(input, output);
	ASSERT_EQ(output.str(), "It is impossibly to build a spanning tree. Try to change input data\n");
}

TEST(SpanningTreeR3Space, SingleVertex)
{
	std::istringstream input{"id1, 2.2, 3.3, 4.4"};
	std::ostringstream output{};
	spanning_tree_Rn_space<3>{}(input, output);
	ASSERT_EQ(output.str(), "It is impossibly to build a spanning tree. Try to change input data\n");
}

TEST(SpanningTreeR3Space, TwoVertices)
{
	std::istringstream input{ "id1, 2.2, 3.3, 4.4\nid2, 2.2, 2.2, 777\n" };
	std::ostringstream output{};
	spanning_tree_Rn_space<3>{}(input, output);
	ASSERT_TRUE(output.str() == "Spanning tree edges:\nid1 - id2\n" || output.str() == "Spanning tree edges:\nid2 - id1\n");
}

TEST(SpanningTreeR2Space, Test1)
{
	const std::string line1{ "id1, 2.5, 1\n" };
	const std::string line2{ "id2, 1, 3\n" };
	const std::string line3{ "id3, 4.3, 2\n" };
	const std::string line4{ "id4, 6, 4\n" };
	const std::string line5{ "id5, 3.5, 3.5\n" };
	const std::string line6{ "id6, 0, 0\n" };
	const std::string line7_invalid{ "id7_invalid, 0, 7, 3\n" };
	const std::string line8{ "id7, 0, 7\n" };

	std::string input_string{};
	input_string += line1;
	input_string += line2;
	input_string += line3;
	input_string += line4;
	input_string += line5;
	input_string += line6;
	input_string += line7_invalid;
	input_string += line8;

	std::istringstream input{ input_string  };
	std::stringstream output{};
	spanning_tree_Rn_space<2>{}(input, output);

	std::string output_line{};
	std::getline(output, output_line);
	ASSERT_EQ(output_line, "Spanning tree edges:");
	std::getline(output, output_line);
	ASSERT_TRUE(output_line == "id3 - id5" || output_line == "id5 - id3");
	std::getline(output, output_line);
	ASSERT_TRUE(output_line == "id1 - id3" || output_line == "id3 - id1");
	std::getline(output, output_line);
	ASSERT_TRUE(output_line == "id1 - id2" || output_line == "id2 - id1");
	std::getline(output, output_line);
	ASSERT_TRUE(output_line == "id4 - id5" || output_line == "id5 - id4");
	std::getline(output, output_line);
	ASSERT_TRUE(output_line == "id1 - id6" || output_line == "id6 - id1");
	std::getline(output, output_line);
	ASSERT_TRUE(output_line == "id2 - id7" || output_line == "id7 - id2");
	std::getline(output, output_line);
	ASSERT_EQ(output_line, "");
	std::getline(output, output_line);
	ASSERT_EQ(output_line, "Invalid lines:");
	std::getline(output, output_line);
	ASSERT_EQ(output_line, "[7] dimension of the vertex is wrong");
}

TEST(SpanningTreeR3Space, Test1)
{
	const std::string line1{ "id1,    1,    1,   1\n" };
	const std::string line2_invalid{ "invalid, 2, 1, 3, 4\n" };
	const std::string line_empty{ "\n" };
	const std::string line3{ "id2,   2,   1,   3\n" };
	const std::string line4{ "id3,  4,   5,   7\n" };
	const std::string line5{ "id4,  9,   -5,   -3\n" };
	const std::string line6_invalid{ "id,   0, \n" };
	const std::string line7{ "id5,   -1,   -1,   -1\n" };

	std::string input_string{};
	input_string += line1;
	input_string += line2_invalid;
	input_string += line_empty;
	input_string += line3;
	input_string += line4;
	input_string += line5;
	input_string += line6_invalid;
	input_string += line7;

	std::istringstream input{ input_string };
	std::stringstream output{};
	spanning_tree_Rn_space<3>{}(input, output);

	std::string output_line{};
	std::getline(output, output_line);
	ASSERT_EQ(output_line, "Spanning tree edges:");
	std::getline(output, output_line);
	ASSERT_TRUE(output_line == "id1 - id2" || output_line == "id2 - id1");
	std::getline(output, output_line);
	ASSERT_TRUE(output_line == "id1 - id5" || output_line == "id5 - id1");
	std::getline(output, output_line);
	ASSERT_TRUE(output_line == "id2 - id3" || output_line == "id3 - id2");
	std::getline(output, output_line);
	ASSERT_TRUE(output_line == "id1 - id4" || output_line == "id4 - id1");
	std::getline(output, output_line);
	ASSERT_EQ(output_line, "");
	std::getline(output, output_line);
	ASSERT_EQ(output_line, "Invalid lines:");
	std::getline(output, output_line);
	ASSERT_EQ(output_line, "[2] dimension of the vertex is wrong");
	std::getline(output, output_line);
	ASSERT_EQ(output_line, "[6] dimension of the vertex is wrong");
}