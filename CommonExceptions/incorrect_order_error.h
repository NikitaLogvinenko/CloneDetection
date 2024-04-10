#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class incorrect_order_error final : std::invalid_argument
	{
	public:
		explicit incorrect_order_error(const std::string& what_arg) : std::invalid_argument(what_arg) {}
	};
}
