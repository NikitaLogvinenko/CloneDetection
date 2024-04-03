#pragma once
#include "clang-c/Index.h"
#include "func_id.h"
#include <vector>

namespace clang_funcs_analysis
{
	class funcs_definitions_finder final
	{
	public:
		funcs_definitions_finder() = delete;

		[[nodiscard]] static std::vector<code_analysis::func_id> find_nested(const CXCursor& root_cursor);
	};
}
