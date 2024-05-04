#pragma once
#include <string>

class Person final
{
private:
	std::string name;
public:
	Person() = delete;
	explicit Person(const std::string& _name);
	~Person() = default;
	const std::string& getName() const;
	bool operator==(const Person& x) const;
	bool operator<(const Person& x) const;
};
