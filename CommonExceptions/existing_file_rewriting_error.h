#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class existing_file_rewriting_error final : std::logic_error
	{
	public:
		explicit existing_file_rewriting_error(const std::string& what_arg) : std::logic_error(what_arg) {}
	};
}
