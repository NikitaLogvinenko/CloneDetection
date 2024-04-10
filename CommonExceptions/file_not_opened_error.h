#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class file_not_opened_error final : std::runtime_error
	{
	public:
		explicit file_not_opened_error(const std::string& what_arg) : std::runtime_error(what_arg) {}
	};
}
