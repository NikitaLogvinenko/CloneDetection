#ifndef __PARCER_H_
#define	__PARCER_H_

#include <iostream>

typedef std::pair<std::string, std::string> Friends;

class parcer final
{
public:
	parcer(std::istream& _ist): ist(_ist) {}

	bool get_names(Friends& names);

private:
	std::istream &ist;
};

#endif