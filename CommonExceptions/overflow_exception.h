#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class overflow_exception final : std::range_error
	{
	public:
		explicit overflow_exception(const std::string& what_arg) : std::range_error(what_arg) {}
	};
}
