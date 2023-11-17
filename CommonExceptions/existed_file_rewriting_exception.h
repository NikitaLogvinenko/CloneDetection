#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class existed_file_rewriting_exception final : std::logic_error
	{
	public:
		explicit existed_file_rewriting_exception(const std::string& what_arg) : std::logic_error(what_arg) {}
	};
}
