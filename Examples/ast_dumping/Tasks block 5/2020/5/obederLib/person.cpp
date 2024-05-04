#include "person.h"


Person::Person(const std::string& _name)
{
	name = _name;
}

const std::string& Person::getName() const
{
	return name;
}

bool Person::operator==(const Person& x) const
{
	return name == x.name;
}

bool Person::operator<(const Person& x) const
{
	return name < x.name;
}