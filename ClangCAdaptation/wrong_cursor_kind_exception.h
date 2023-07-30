#pragma once
#include <string>
#include <stdexcept>

namespace clang_c_adaptation
{
	class wrong_cursor_kind_exception final : std::invalid_argument
	{
	public:
		explicit wrong_cursor_kind_exception(const std::string& what_msg) : std::invalid_argument(what_msg) {}
	};
}