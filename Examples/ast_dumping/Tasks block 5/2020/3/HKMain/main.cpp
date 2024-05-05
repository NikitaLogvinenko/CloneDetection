#include <iostream>
#include <fstream>
#include <string>

#include "FileIO.hpp"
#include "HotKeyMap.hpp"
#include "HotKeyIO.hpp"




int main(int argc, char* argv[])
{
	FileIO names(argc, argv);

	HotkeyMap map1, map2;
	std::ofstream fout;
	std::ifstream fin1, fin2;
	std::vector<std::string> rlist;

	std::istream& inStream1 = names.setFirstInStream(fin1);
	std::istream& inStream2 = names.setSecondInStream(fin2);
	std::ostream& outStream = names.setOutStream(fout);

	map1 = readMap(inStream1);
	map2 = readMap(inStream2);

	map1.merge(map2);

	outStream << map1;

	if (!names.getOutputName().empty())
		fout.close();
	return 0;
}
