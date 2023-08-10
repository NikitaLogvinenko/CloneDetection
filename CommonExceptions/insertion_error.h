#pragma once
#include <string_view>
#include <stdexcept>

namespace common_exceptions
{
	class insertion_error final : std::range_error
	{
	public:
		explicit insertion_error(const std::string_view what_arg) : std::range_error(what_arg.data()) {}
	};
}