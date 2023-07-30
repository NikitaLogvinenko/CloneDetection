#pragma once
#include <string>
#include <stdexcept>

namespace clang_c_adaptation
{
	class unsuccessfull_tokenization_exception final : std::runtime_error
	{
		inline static const std::string cursor_tokenization_error_msg{"Failure during CXCursor tokenization. Memory leak was prevented. Exception: "};

	public:
		explicit unsuccessfull_tokenization_exception(const std::exception& ex)
		: std::runtime_error(cursor_tokenization_error_msg + ex.what()) {}
	};
}