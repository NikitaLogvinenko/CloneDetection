﻿#pragma once
#include "clang-c/Index.h"

namespace clang_c_adaptation
{
	class cxindex_wrapper final
	{
		CXIndex index_{};

	public:
		explicit cxindex_wrapper(const bool exclude_decls_from_pch = true, const bool display_diagnostics = true)
			: index_{ clang_createIndex(exclude_decls_from_pch, display_diagnostics)} {}

		cxindex_wrapper(const cxindex_wrapper& other) = delete;
		cxindex_wrapper& operator=(const cxindex_wrapper& other) = delete;
		cxindex_wrapper(cxindex_wrapper&& other) noexcept : index_(other.index_)
		{
			other.index_ = nullptr;
		}
		cxindex_wrapper& operator=(cxindex_wrapper&& other) noexcept
		{
			if (&other == this)
			{
				return *this;
			}
			index_ = other.index_;
			other.index_ = nullptr;
			return *this;
		}
		~cxindex_wrapper()
		{
			clang_disposeIndex(index_);
		}

		[[nodiscard]] const CXIndex& index() const noexcept
		{
			return index_;
		}
	};
}