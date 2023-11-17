#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class incorrect_order_exception final : std::invalid_argument
	{
	public:
		explicit incorrect_order_exception(const std::string& what_arg) : std::invalid_argument(what_arg) {}
	};
}
