#pragma once
#include <string>
#include <fstream>

namespace CommandArgs {
	struct CommandArgs {
		std::string input_file_name;
		std::string output_file_name;
	};

	CommandArgs ParseCommandArgs(const int argc, char* argv[]);
}