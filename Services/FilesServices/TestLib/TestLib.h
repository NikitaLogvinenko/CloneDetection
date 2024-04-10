#pragma once
#include <string>

int Increment(int a);

class TestClass
{
	int a;
	std::string name = "test";
public:
	TestClass(int a);

	int GetA() const;
	const std::string& GetName() const;
};