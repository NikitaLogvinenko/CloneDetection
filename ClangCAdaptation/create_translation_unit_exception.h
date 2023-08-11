#pragma once
#include <string_view>
#include <stdexcept>

namespace clang_c_adaptation
{
	class create_translation_unit_exception final : std::runtime_error
	{
	public:
		explicit create_translation_unit_exception(const std::string_view what_arg)
		: std::runtime_error(what_arg.data()) {}
	};
}
