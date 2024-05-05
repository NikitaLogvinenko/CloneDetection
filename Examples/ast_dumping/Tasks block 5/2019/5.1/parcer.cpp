#include "parcer.h"

#include <string>
#include <sstream>

namespace
{
	const char PARCE_SYMBOL = ' ';
}

bool parcer::get_names(Friends&  names)
{
	std::string line;
	getline(ist, line);
	if (line.empty())
	{
		return false;
	}

	std::istringstream isst(line);
	getline(isst >> std::ws, names.first, PARCE_SYMBOL);
	getline(isst >> std::ws, names.second, PARCE_SYMBOL);
	
	return !names.first.empty() ||
		   !names.second.empty();
}