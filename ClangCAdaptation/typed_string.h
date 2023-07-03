#pragma once
#include <string>
#include <algorithm>

namespace clang_c_adaptation
{
	class typed_string
	{
		std::string string_{};
	public:
		typed_string() noexcept = default;
		explicit typed_string(std::string string) noexcept : string_(std::move(string)) {}

		virtual ~typed_string() = default;

		[[nodiscard]] const std::string& to_string() const noexcept
		{
			return string_;
		}

		[[nodiscard]] bool operator==(const typed_string& other) const noexcept = default;
	};
}
