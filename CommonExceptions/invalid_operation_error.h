#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class invalid_operation_error final : std::runtime_error
	{
	public:
		explicit invalid_operation_error(const std::string& what_arg) : std::runtime_error(what_arg) {}
	};
}
