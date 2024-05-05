#pragma once
#include "clang-c/Index.h"

namespace clang_c_adaptation
{
	class cxcursor_hash final
	{
	public:
		[[nodiscard]] size_t operator()(const CXCursor& cursor) const noexcept
		{
			return clang_hashCursor(cursor);
		}
	};
}
