#pragma once
#include <string>
#include <stdexcept>

namespace common_exceptions
{
	class nullptr_error final : std::logic_error
	{
	public:
		explicit nullptr_error(const std::string& what_arg) : std::logic_error(what_arg) {}
	};
}
