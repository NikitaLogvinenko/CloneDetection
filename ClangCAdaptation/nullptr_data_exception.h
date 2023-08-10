#pragma once
#include <string_view>
#include <stdexcept>

namespace clang_c_adaptation
{
	class nullptr_data_exception final : std::invalid_argument
	{
	public:
		explicit nullptr_data_exception(const std::string_view what_msg)
		: std::invalid_argument(what_msg.data()) {}
	};
}