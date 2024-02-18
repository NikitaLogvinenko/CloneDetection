#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class param_out_of_range_error final : std::domain_error
	{
	public:
		explicit param_out_of_range_error(const std::string& what_arg) : std::domain_error(what_arg) {}
	};
}
