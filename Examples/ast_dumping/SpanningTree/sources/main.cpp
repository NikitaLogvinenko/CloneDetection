#include "spanning_tree_Rn_space.h"
#include <iostream>
#include <fstream>
#include <filesystem>

int main(int argc, const char** argv)
{
	if (argc != 3)
	{
		std::cout << "Spanning Tree Application\n";
		std::cout << "Wrong list of arguments.\n";
		std::cout << "Entry the path to the graph's vertices specification and path spanning tree record to.\n";
		return 0;
	}
	++argv;

	const std::string input_name(*argv++);
	const std::string output_name(*argv);
	std::ifstream input{ input_name, std::ios::in };
	if (!input.is_open())
	{
		std::cout << "Failed to open input file " << input_name << "\n";
		return 1;
	}

	if (std::filesystem::exists(output_name))
	{
		std::cout << "File " << output_name << " already exists. Change the output path\n";
		return 2;
	}

	std::ofstream output{ output_name, std::ios::out };
	if (!output.is_open())
	{
		std::cout << "Failed to open output file " << output_name << "\n";
		return 3;
	}

	graph_io_ns::spanning_tree_Rn_space<3>{}(input, output);
	return 0;
}