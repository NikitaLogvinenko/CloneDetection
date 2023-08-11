#pragma once
#include <string_view>
#include <stdexcept>

namespace common_exceptions
{
	class insertion_error final : std::runtime_error
	{
	public:
		explicit insertion_error(const std::string_view what_arg) : std::runtime_error(what_arg.data()) {}
	};
}