#include <iostream>
#include <fstream>

#include "FileIO.hpp"



std::ostream& FileIO::setOutStream(std::ofstream& fout) const
{
	if (getOutputName().empty())
	{
		std::cout << "Writing to console:" << std::endl;
	}
	else
	{
		std::cout << "Writing to file: " << getOutputName() << std::endl;
		fout.open(getOutputName());
	}
	std::ostream& outStream = (getOutputName().empty()) ? std::cout : fout;
	return outStream;
}

std::istream& FileIO::setFirstInStream(std::ifstream& fin) const
{
	if (getFirstInputName().empty()) {
		std::cout << "Reading from console:" << std::endl;
	}
	else
	{
		std::cout << "Reading from file: " << getFirstInputName() << std::endl;
		fin.open(getFirstInputName());
	}
	std::istream& inStream1 = (getFirstInputName().empty()) ? std::cin : fin;
	return inStream1;
}

std::istream& FileIO::setSecondInStream(std::ifstream& fin) const
{
	if (getSecondInputName().empty()) {
		std::cout << "Reading from console:" << std::endl;
	}
	else
	{
		std::cout << "Reading from file: " << getSecondInputName() << std::endl;
		fin.open(getSecondInputName());
	}
	std::istream& inStream2 = (getSecondInputName().empty()) ? std::cin : fin;
	return inStream2;
}
