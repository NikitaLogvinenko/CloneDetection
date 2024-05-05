#include "crc16.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace CRC;
int main(int argc, char* argv[]){
	if (argc < 3) return 0;
	std::string argument;
	argument.append(argv[1]);
	if (argument == "--calc") {
		std::vector<std::string> names;
		for (int i = 2; i <= argc - 1; i++) {
			names.push_back(argv[i]);
		}
		crc16::create_manifest(names);
		return 1;
	}
	if (argument == "--check") {
		crc16::check(argv[2]);
		return 1;
	}
	std::cout << "Incorrect parameters!\n";
	return 0;
}