#pragma once
#include "clang-c/Index.h"
#include "create_cxindex_exception.h"
#include "copying_delete_move_through_swap.h"

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

		COPYING_DELETE_MOVE_THROUGH_SWAP(cxindex_raii)

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