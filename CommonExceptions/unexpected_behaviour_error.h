#pragma once
#include <string>
#include <stdexcept>

namespace common_exceptions
{
	class unexpected_behaviour_error final : public std::logic_error
	{
	public:
		explicit unexpected_behaviour_error(const std::string& what_arg) : std::logic_error(what_arg) {}
	};
}
