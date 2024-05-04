#include "DijkstraForEdgesFromFile.hpp"
#include "CommandArgs.hpp"

int main(int argc, char* argv[]) {
	{
		auto _CommandArgs = CommandArgs::ParseCommandArgs(argc, argv);
		std::ifstream inp(_CommandArgs.input_file_name);
		if (!inp) {
			std::cout << "Error: File not found/EmptyFile\n" << std::endl;
			return 1;
		}
		std::ofstream out(_CommandArgs.output_file_name);
		if (!out) {
			std::cout << "Error: Cannot open file for writing!" << std::endl;
			return 1;
		}
		DijkstraForEdgesFromFile(inp, out);
	}
	
	std::cout << _CrtDumpMemoryLeaks();
	return 0;
}