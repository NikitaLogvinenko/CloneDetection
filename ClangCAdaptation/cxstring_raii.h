#pragma once
#include "clang-c/Index.h"
#include "invalid_cxstring_exception.h"

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

		cxstring_raii(const cxstring_raii&) = delete;
		cxstring_raii& operator=(const cxstring_raii&) = delete;
		cxstring_raii(cxstring_raii&& other) noexcept
		{
			std::swap(*this, other);
		}
		cxstring_raii& operator=(cxstring_raii&& other) noexcept
		{
			if (&other == this)
			{
				return *this;
			}
			std::swap(*this, other);
			return *this;
		}
		~cxstring_raii()
		{
			clang_disposeString(cxstring_);
		}

		[[nodiscard]] std::string string() const
		{
			if (cxstring_.data == nullptr)
			{
				throw invalid_cxstring_exception("Attempt to get string from CXString with data = nullptr.");
			}
			return {clang_getCString(cxstring_) };
		}
	};
}
