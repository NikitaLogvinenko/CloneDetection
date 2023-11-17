#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class nan_exception final : std::domain_error
	{
	public:
		explicit nan_exception(const std::string& what_arg) : std::domain_error(what_arg) {}
	};
}
