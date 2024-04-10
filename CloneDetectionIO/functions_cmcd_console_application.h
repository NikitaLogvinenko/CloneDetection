#pragma once
#include <string>
#include <vector>

namespace clone_detection_io
{
	class functions_cmcd_console_application final
	{
	public:
		functions_cmcd_console_application() = delete;

		static void execute(const std::vector<std::string>& argv);
	};
}
