#include "funcs_clones_analysis_through_cm_app.h"
#include <iostream>
#include <exception>

int main(const int argc, const char* const* argv)
{
	try
	{
		code_clones_analysis_top_level::funcs_clones_analysis_through_cm_app::exec(argc, argv);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
		return 1;
	}

	return 0;
}