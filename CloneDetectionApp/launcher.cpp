#include "functions_cmcd_console_application.h"

int main(const size_t argc, const char** argv)
{
	clone_detection_io::functions_cmcd_console_application::execute({argv, argv + argc});
	return 0;
}
