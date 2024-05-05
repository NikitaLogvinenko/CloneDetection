#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class types_conversion_error final : public std::runtime_error
	{
	public:
		explicit types_conversion_error(const std::string& what_arg) : std::runtime_error(what_arg) {}
	};
}
