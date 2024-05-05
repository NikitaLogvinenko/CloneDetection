#pragma once
#include "clang-c/Index.h"
#include "nullptr_error.h"
#include "copying_delete.h"

namespace clang_c_adaptation
{
	class cxstring_raii final
	{
		CXString cxstring_{};

	public:
		explicit cxstring_raii(CXString&& cxstring) noexcept
		{
			std::swap(cxstring_, cxstring);
		}

		COPYING_DELETE(cxstring_raii)

		cxstring_raii(cxstring_raii&& other) noexcept
		{
			std::swap(cxstring_, other.cxstring_);
		}

		cxstring_raii& operator=(cxstring_raii&& other) noexcept
		{
			std::swap(cxstring_, other.cxstring_);
			return *this;
		}

		~cxstring_raii()
		{
			clang_disposeString(cxstring_);
		}

		[[nodiscard]] bool is_valid() const
		{
			return clang_getCString(cxstring_) != nullptr;
		}

		[[nodiscard]] std::string string() const
		{
			const char* const c_string = clang_getCString(cxstring_);

			if (c_string == nullptr)
			{
				throw common_exceptions::nullptr_error(
					"cxstring_raii::string: initial CXString contains nullptr.");
			}

			return std::string{ c_string };
		}
	};
}
