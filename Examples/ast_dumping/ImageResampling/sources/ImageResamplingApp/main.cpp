#include "main_executor.h"
#include <iostream>

int main(const int argc, const char** argv)
{
	try
	{
		return main_executor::execute(argc, argv);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what();
		return 1;
	}
}