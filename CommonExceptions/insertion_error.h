#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class insertion_error final : public std::runtime_error
	{
	public:
		explicit insertion_error(const std::string& what_arg) : std::runtime_error(what_arg) {}
	};
}
