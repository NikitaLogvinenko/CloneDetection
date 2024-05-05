#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class nan_error final : public std::domain_error
	{
	public:
		explicit nan_error(const std::string& what_arg) : std::domain_error(what_arg) {}
	};
}
