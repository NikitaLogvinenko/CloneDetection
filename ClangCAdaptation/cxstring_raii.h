#pragma once
#include "clang-c/Index.h"

namespace clang_c_adaptation::internal
{
	class cxstring_raii final
	{
		CXString cxstring_;
		const char* c_str_;
	public:
		explicit cxstring_raii(CXString&& cxstring) noexcept
		: cxstring_(cxstring), c_str_(clang_getCString(cxstring)) { }

		cxstring_raii(const cxstring_raii&) = delete;
		cxstring_raii& operator=(const cxstring_raii&) = delete;
		cxstring_raii(cxstring_raii&&) = delete;
		cxstring_raii& operator=(cxstring_raii&&) = delete;
		~cxstring_raii()
		{
			clang_disposeString(cxstring_);
		}

		[[nodiscard]] const char* c_str() const noexcept
		{
			return clang_getCString(cxstring_);
		}
	};
}