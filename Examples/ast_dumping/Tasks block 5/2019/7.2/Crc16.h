#pragma once
#include <string>
#include <vector>

class Crc16 final {
private:
	std::string filename = "untitled";
	int summ = 0;
public:
	Crc16() = delete;
	Crc16(std::string fileName);
	Crc16(std::istream& in);
	~Crc16();

	static int checkSumm(std::istream& stream);
	static std::vector<std::string> strToWords(std::string command, std::string delim);
	static std::map<std::string, int> readManifest(std::string filename);
	static void writeCheckToManifest(std::vector<std::string> commands);
};