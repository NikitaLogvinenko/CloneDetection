#pragma once
#include <string_view>
#include <stdexcept>

namespace clang_c_adaptation
{
	class wrong_cursor_kind_error final : std::invalid_argument
	{
	public:
		explicit wrong_cursor_kind_error(const std::string_view what_msg) : std::invalid_argument(what_msg.data()) {}
	};
}
