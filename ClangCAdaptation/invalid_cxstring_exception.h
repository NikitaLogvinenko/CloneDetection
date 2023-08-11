#pragma once
#include <string_view>
#include <stdexcept>

namespace clang_c_adaptation::internal
{
	class invalid_cxstring_exception final : std::invalid_argument
	{
	public:
		explicit invalid_cxstring_exception(const std::string_view what_msg)
			: std::invalid_argument(what_msg.data()) {}
	};
}