#include "HotKeyIO.hpp"
#include "HotKeyMap.hpp"

#include <istream>
#include <sstream>

namespace {
	std::string readKey(std::istream& is)
	{
		std::string key;
		if (is >> key) is.ignore(std::numeric_limits<std::streamsize>::max(), '=');
		return key;
	}


	std::string readCmd(std::istream& is)
	{
		std::string cmd;
		is >> cmd;
		return cmd;
	}
}

HotkeyMap readMap(std::istream& is)
{
	HotkeyMap map;
	std::string str;
	
	while(std::getline(is, str))
	{
		std::istringstream iss(str);
		std::string key = readKey(iss);
		std::string cmd = readCmd(iss);
		map.add_pair(key, cmd);
	}

	return map;
}

std::vector<std::string> readRemoveList(std::istream& is)
{
	std::vector<std::string> rlist;
	std::string str;

	while (std::getline(is, str))
	{
		rlist.push_back(str);
	}

	return rlist;
}
