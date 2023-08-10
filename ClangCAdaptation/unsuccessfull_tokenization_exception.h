#pragma once
#include <string_view>
#include <stdexcept>

namespace clang_c_adaptation
{
	class unsuccessfull_tokenization_exception final : std::runtime_error
	{
		inline static const std::string cursor_tokenization_error_msg{};

	public:
		explicit unsuccessfull_tokenization_exception(const std::string_view what_arg)
		: std::runtime_error(what_arg.data()) {}
	};
}