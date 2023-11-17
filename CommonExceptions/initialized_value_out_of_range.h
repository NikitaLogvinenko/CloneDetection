#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class initialized_value_out_of_range final : std::domain_error
	{
	public:
		explicit initialized_value_out_of_range(const std::string& what_arg) : std::domain_error(what_arg) {}
	};
}
