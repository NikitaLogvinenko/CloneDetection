#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class input_format_error final : std::runtime_error
	{
	public:
		explicit input_format_error(const std::string& what_arg) : std::runtime_error(what_arg) {}
	};
}
