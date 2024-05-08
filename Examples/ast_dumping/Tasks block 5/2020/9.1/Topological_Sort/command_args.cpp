#include "command_args.hpp"

command_args command_args::parse_command_args(const int argc, char* argv[]) {
	command_args args;
	if (argc > 1) {
		args.input_file_name = argv[1];
	}
	if (argc > 2) {
		args.output_file_name = argv[2];
	}
	return args;
}