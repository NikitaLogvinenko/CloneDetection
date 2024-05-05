#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class incorrect_size_error final : public std::invalid_argument
	{
	public:
		explicit incorrect_size_error(const std::string& what_arg) : std::invalid_argument(what_arg) {}
	};
}
