#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class overflow_error final : public std::range_error
	{
	public:
		explicit overflow_error(const std::string& what_arg) : std::range_error(what_arg) {}
	};
}
