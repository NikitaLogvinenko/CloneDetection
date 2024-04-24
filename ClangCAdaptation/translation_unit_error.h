#pragma once
#include <string_view>
#include <stdexcept>

namespace clang_c_adaptation
{
	class translation_unit_error final : public std::runtime_error
	{
	public:
		explicit translation_unit_error(const std::string_view what_arg) : std::runtime_error(what_arg.data()) {}
	};
}
