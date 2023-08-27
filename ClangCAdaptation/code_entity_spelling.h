#pragma once
#include "clang-c/Index.h"
#include "cxstring_raii.h"

namespace clang_c_adaptation
{
	class code_entity_spelling final
	{
		std::string string_{};

	public:
		code_entity_spelling() noexcept = default;

		explicit code_entity_spelling(const CXCursor& cursor) :
			string_(internal::cxstring_raii(clang_getCursorSpelling(cursor)).string()) {}

		[[nodiscard]] const std::string& to_string() const noexcept
		{
			return string_;
		}

		[[nodiscard]] bool operator==(const code_entity_spelling& other) const noexcept = default;
	};
}
