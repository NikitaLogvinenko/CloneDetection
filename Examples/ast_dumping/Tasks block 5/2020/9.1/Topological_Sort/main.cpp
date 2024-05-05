#include <iostream>
#include <fstream>
#include "Windows.h"
#include"command_args.hpp"
#include "sort_graph_from_file.hpp"

int main (int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	auto _command_args = command_args::parse_command_args(argc, argv);
	std::ifstream inp(_command_args.input_file_name);
	if (!inp) {
		std::cout << "Error: File not found/EmptyFile\n" << std::endl;
		return 1;
	}
	std::ofstream out(_command_args.output_file_name);
	if (!out) {
		std::cout << "Error: Cannot open file for writing!" << std::endl;
		return 1;
	}
	sort_graph_from_file(inp, out);
	return 0;
}