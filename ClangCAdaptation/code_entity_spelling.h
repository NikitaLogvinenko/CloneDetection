#pragma once
#include "clang-c/Index.h"
#include "cxstring_wrapper.h"

namespace clang_c_adaptation
{
	class code_entity_spelling
	{
		std::string string_{};
	public:
		code_entity_spelling() noexcept = default;
		explicit code_entity_spelling(const CXCursor& cursor) :
			string_(cxstring_wrapper(clang_getCursorSpelling(cursor)).c_str()) {}

		code_entity_spelling(const code_entity_spelling& other) = default;
		code_entity_spelling(code_entity_spelling&& other) noexcept = default;
		code_entity_spelling& operator=(const code_entity_spelling& other) = default;
		code_entity_spelling& operator=(code_entity_spelling&& other) noexcept = default;
		virtual ~code_entity_spelling() = default;

		[[nodiscard]] const std::string& to_string() const noexcept
		{
			return string_;
		}

		[[nodiscard]] bool operator==(const code_entity_spelling& other) const noexcept = default;
	};
}