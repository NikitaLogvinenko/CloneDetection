﻿#pragma once
#include <string_view>
#include <stdexcept>

namespace clang_c_adaptation
{
	class clang_range_exception final : std::runtime_error
	{
	public:
		explicit clang_range_exception(const std::string_view what_msg) : std::runtime_error(what_msg.data()) {}
	};
}