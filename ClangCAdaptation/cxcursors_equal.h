#pragma once
#include "clang-c/Index.h"

namespace clang_c_adaptation
{
	class cxcursors_equal final
	{
	public:
		[[nodiscard]] bool operator()(const CXCursor& first_cursor, const CXCursor& second_cursor) const noexcept
		{
			return clang_equalCursors(first_cursor, second_cursor) != 0;
		}
	};
}
