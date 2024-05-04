#ifndef __FRIEND_ADVISER_
#define __FRIEND_ADVISER_

#include <vector>

#include "parcer.h"

class friend_adviser final
{
public:
	friend_adviser(parcer& _prc): prc(_prc) {}

	std::vector<std::string> get_new_friends(const std::string& user_id, double threshold);

private:
	parcer& prc;
};

#endif