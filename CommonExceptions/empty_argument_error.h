#pragma once
#include <string>
#include <stdexcept>

namespace common_exceptions
{
	class empty_argument_error final : public std::logic_error
	{
	public:
		explicit empty_argument_error(const std::string& what_arg) : std::logic_error(what_arg) {}
	};
}
