#pragma once
#include "clang-c/Index.h"
#include "cxstring_raii.h"
#include "code_entity_spelling.h"

namespace clang_code_analysis
{
	class code_entity_spelling_creator final
	{
	public:
		code_entity_spelling_creator() = delete;

		[[nodiscard]] static code_analysis::code_entity_spelling create(const CXCursor& cursor)
		{
			std::string spelling = clang_c_adaptation::cxstring_raii(clang_getCursorSpelling(cursor)).string();
			return code_analysis::code_entity_spelling{ std::move(spelling) };
		}
	};
}
