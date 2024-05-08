#include "CommandArgs.hpp"

CommandArgs::CommandArgs CommandArgs::ParseCommandArgs(const int argc, char* argv[]) {
	CommandArgs args;
	if (argc > 1) {
		args.input_file_name = argv[1];
	}
	if (argc > 2) {
		args.output_file_name = argv[2];
	}
	return args;
}