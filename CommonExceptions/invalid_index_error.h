#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class invalid_index_error final : public std::invalid_argument
	{
	public:
		explicit invalid_index_error(const std::string& what_arg) : std::invalid_argument(what_arg) {}
	};
}
