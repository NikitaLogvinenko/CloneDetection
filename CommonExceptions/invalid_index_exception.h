#pragma once
#include <string_view>
#include <stdexcept>

namespace common_exceptions
{
	class invalid_index_exception final : std::invalid_argument
	{
	public:
		explicit invalid_index_exception(const std::string_view what_arg)
			: std::invalid_argument(what_arg.data()) {}
	};
}