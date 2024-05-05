#include "crc16.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace CRC;
unsigned short crc16::calc(std::istream& stream) {
	char c;
	unsigned short reg = 0xFFFF;
	char index;
	c = stream.get();
	while (!stream.eof()) {
		reg = (reg >> 8) ^ Crc16Table[(reg & 0xFF) ^ c];
		c = stream.get();
	}
	return reg;
}
void crc16::create_manifest(std::vector<std::string> files) {
	std::string buffer;
	std::ofstream manifest;
	std::stringstream ss;
	crc16::calc(ss);
	manifest.open("manifest.txt");
	std::ifstream file;
	for(auto it=files.cbegin();it!=files.cend();++it) {
		buffer.append(*it);
		buffer.append(":");
		file.open(*it);
		if (!file.is_open()) {
			std::cout << "\nCouldn't open file:"<<*it;
			return;
		}
		buffer.append(std::to_string(crc16::calc(file)));
		manifest << buffer << '\n';
		std::cout << "checksum for " << *it << " calculated\n";
		buffer.clear();
		file.close();
	}

	manifest.close();
}
void crc16::check(const char* manifest) {
	std::ifstream file(manifest);
	if (!file.is_open()) {
		std::cout << "\nCouldn't open file!";
		return;
	}
	std::string buffer;
	while (getline(file, buffer)) {
		unit_check(buffer);
	}

}
bool print_result(std::string filename, bool indicator) {
	if (indicator) {
		std::cout << filename << ": <OK>\n";
		return true;
	}
	std::cout << filename << ": <FAILED>\n";
	return false;
}
bool crc16::unit_check(std::string buffer) {
	size_t colon = buffer.find(':');
	std::string filename(buffer.substr(0,colon));
	std::string number = buffer.substr(colon+1, buffer.size() - colon);
	unsigned short file_crc = (unsigned short)std::stoi(number);
	std::ifstream file(filename);
	if (!file.is_open()) return print_result(filename,false);
	unsigned short right_crc=crc16::calc(file);
	if (right_crc != file_crc) return print_result(filename, false);
	return print_result(filename, true);

}
