#pragma once
#include "typed_string.h"
#include "clang-c/Index.h"
#include "cxstring_wrapper.h"

namespace clang_c_adaptation
{
	class code_entity_spelling : public typed_string
	{
	public:
		code_entity_spelling() noexcept = default;
		explicit code_entity_spelling(const CXCursor& cursor) :
			typed_string(cxstring_wrapper(clang_getCursorSpelling(cursor)).c_str()) {}

		[[nodiscard]] bool operator==(const code_entity_spelling& other) const noexcept = default;
	};
}