#pragma once
#include "clang-c/Index.h"
#include <vector>
#include <functional>

namespace clang_funcs_analysis
{
	class funcs_definitions_finder final
	{
	public:
		funcs_definitions_finder() = delete;

		[[nodiscard]] static std::vector<CXCursor> find_nested(const CXCursor& root_cursor);
	};
}
