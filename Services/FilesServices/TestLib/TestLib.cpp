// TestLib.cpp : Defines the functions for the static library.
//

#include "pch.h"

#include <iostream>
#include "TestLib.h"
#include "framework.h"

// TODO: This is an example of a library function
int Increment(int a)
{
	return ++a;
}

int ::TestClass::GetA() const
{
	return a;
}

::TestClass::TestClass(int a)
{
	this->a = a;
}

const std::string& ::TestClass::GetName() const
{
	return  name;
}
