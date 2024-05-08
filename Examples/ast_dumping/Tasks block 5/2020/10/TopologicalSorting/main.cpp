#include <iostream>
#include <fstream>
#include "Windows.h"
#include "sortGraphFromFile.h"

int main()
{
		std::ifstream in("input.txt");
	try {
		if (!in) {
			throw std::exception();
		}
	}
	catch (std::exception& e) {
		std::cout << "File not found or was empty\n" << std::endl;
		return 1;
	}
	std::ofstream out("output.txt");
	try {
	if (!out) {
		throw std::exception();
	}
	}
	catch (std::exception& e) {
		std::cout << "Cannot open file for writing!" << std::endl;
		return 1;
	}
	sortGraphFfromFile(in, out);
	return 0;
}