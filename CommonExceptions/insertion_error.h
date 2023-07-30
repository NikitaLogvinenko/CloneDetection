#pragma once
#include <string>
#include <stdexcept>

namespace common_exceptions
{
	class insertion_error final : std::range_error
	{
	public:
		explicit insertion_error(const std::string& what_arg) : std::range_error(what_arg) {}
	};
}