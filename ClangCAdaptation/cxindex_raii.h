#pragma once
#include "clang-c/Index.h"
#include "cxindex_error.h"
#include "copying_delete.h"

namespace clang_c_adaptation
{
	class cxindex_raii final
	{
		CXIndex index_{};

		static constexpr auto invalid_index = nullptr;

	public:
		explicit cxindex_raii(const int exclude_declarations_from_pch = 1, const int display_diagnostics = 0)
			: index_{clang_createIndex(exclude_declarations_from_pch, display_diagnostics)}
		{
			if (index_ == invalid_index)
			{
				throw cxindex_error("cxindex_raii::cxindex_raii: failure during CXIndex creating.");
			}
		}

		COPYING_DELETE(cxindex_raii)

		cxindex_raii(cxindex_raii&& other) noexcept
		{
			std::swap(index_, other.index_);
		}

		cxindex_raii& operator=(cxindex_raii&& other) noexcept
		{
			std::swap(index_, other.index_);
			return *this;
		}

		~cxindex_raii()
		{
			clang_disposeIndex(index_);
		}

		[[nodiscard]] const CXIndex& index() const noexcept
		{
			return index_;
		}
	};
}
