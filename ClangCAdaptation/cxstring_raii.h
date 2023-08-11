#pragma once
#include "clang-c/Index.h"
#include "nullptr_exception.h"
#include "copying_delete_move_through_swap.h"

namespace clang_c_adaptation::internal
{
	class cxstring_raii final
	{
		CXString cxstring_{};

	public:
		explicit cxstring_raii(CXString&& cxstring) noexcept
		{
			std::swap(cxstring_, cxstring);
		}

		COPYING_DELETE_MOVE_THROUGH_SWAP(cxstring_raii)

		~cxstring_raii()
		{
			clang_disposeString(cxstring_);
		}

		[[nodiscard]] std::string string() const
		{
			if (cxstring_.data == nullptr)
			{
				throw common_exceptions::nullptr_exception(
					"Attempt to get string from CXString with data = nullptr.");
			}
			return {clang_getCString(cxstring_) };
		}
	};
}
