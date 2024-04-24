#include "ast_dumping_app.h"
#include <iostream>
#include <exception>

int main(const int argc, const char* const* argv)
{
	try
	{
		clang_ast_dumping::ast_dumping_app::exec(argc, argv);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
		return 1;
	}

	return 0;
}
