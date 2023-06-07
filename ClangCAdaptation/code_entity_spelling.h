#pragma once
#include "typed_string.h"
#include "clang-c/Index.h"
#include "clang_c_types_handling.h"

namespace clang_c_adaptation
{
	class code_entity_spelling : public typed_string
	{
	public:
		code_entity_spelling() noexcept = default;
		explicit code_entity_spelling(const CXCursor& cursor) : typed_string(cxstring_to_string(std::make_unique<CXString>(clang_getCursorSpelling(cursor)))) {}

		[[nodiscard]] bool operator==(const code_entity_spelling& other) const noexcept = default;
	};
}