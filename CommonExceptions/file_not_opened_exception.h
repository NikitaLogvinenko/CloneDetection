#pragma once
#include <string_view>
#include <stdexcept>

namespace common_exceptions
{
	class file_not_opened_exception final : std::runtime_error
	{
	public:
		explicit file_not_opened_exception(const std::string_view what_arg) : std::runtime_error(what_arg.data()) {}
	};
}
