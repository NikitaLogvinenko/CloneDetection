#pragma once
#include "clang-c/Index.h"

namespace clang_c_adaptation
{
	class cxstring_wrapper final
	{
		CXString cxstring_;
		const char* c_str_;
	public:
		explicit cxstring_wrapper(CXString&& cxstring) noexcept
		: cxstring_(cxstring), c_str_(clang_getCString(cxstring)) { }

		cxstring_wrapper(const cxstring_wrapper&) = delete;
		cxstring_wrapper& operator=(const cxstring_wrapper&) = delete;
		cxstring_wrapper(cxstring_wrapper&&) = delete;
		cxstring_wrapper& operator=(cxstring_wrapper&&) = delete;
		~cxstring_wrapper()
		{
			clang_disposeString(cxstring_);
		}

		[[nodiscard]] const char* c_str() const noexcept
		{
			return clang_getCString(cxstring_);
		}
	};
}