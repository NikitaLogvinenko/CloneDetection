#pragma once
#include "clang-c/Index.h"
#include "create_cxindex_exception.h"

namespace clang_c_adaptation
{
	class cxindex_raii final
	{
		CXIndex index_{};

		static constexpr auto invalid_index = nullptr;

	public:
		explicit cxindex_raii(const bool exclude_decls_from_pch = true, const bool display_diagnostics = true)
			: index_{ clang_createIndex(exclude_decls_from_pch, display_diagnostics)}
		{
			if (index_ == invalid_index)
			{
				throw create_cxindex_exception("Failure during CXIndex creating.");
			}
		}

		cxindex_raii(const cxindex_raii& other) = delete;
		cxindex_raii& operator=(const cxindex_raii& other) = delete;
		cxindex_raii(cxindex_raii&& other) noexcept
		{
			std::swap(*this, other);
		}
		cxindex_raii& operator=(cxindex_raii&& other) noexcept
		{
			if (&other == this)
			{
				return *this;
			}
			std::swap(*this, other);
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