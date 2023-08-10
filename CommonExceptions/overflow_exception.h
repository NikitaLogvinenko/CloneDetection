#pragma once
#include <string_view>
#include <stdexcept>

namespace common_exceptions
{
	class overflow_exception final : std::range_error
	{
	public:
		explicit overflow_exception(const std::string_view what_arg) : std::range_error(what_arg.data()) {}
	};
}