#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class invalid_iterator_operation final : std::runtime_error
	{
	public:
		explicit invalid_iterator_operation(const std::string& what_arg) : std::runtime_error(what_arg) {}
	};
}
