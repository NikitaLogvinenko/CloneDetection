#pragma once
#include "clang-c/Index.h"

namespace clang_c_adaptation
{
	class cxindex_raii final
	{
		CXIndex index_{};

	public:
		explicit cxindex_raii(const bool exclude_decls_from_pch = true, const bool display_diagnostics = true)
			: index_{ clang_createIndex(exclude_decls_from_pch, display_diagnostics)} {}

		cxindex_raii(const cxindex_raii& other) = delete;
		cxindex_raii& operator=(const cxindex_raii& other) = delete;
		cxindex_raii(cxindex_raii&& other) noexcept : index_(other.index_)
		{
			other.index_ = nullptr;
		}
		cxindex_raii& operator=(cxindex_raii&& other) noexcept
		{
			if (&other == this)
			{
				return *this;
			}
			index_ = other.index_;
			other.index_ = nullptr;
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