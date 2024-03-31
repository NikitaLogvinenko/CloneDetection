﻿#pragma once
#include "funcs_pairwise_comparing_result.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"

namespace code_clones_analysis
{
	template <utility::continuous_similarity_bounded_below SimilarityT>
	class funcs_pairwise_comparer_abstract
	{
	protected:
		constexpr funcs_pairwise_comparer_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(funcs_pairwise_comparer_abstract)

		[[nodiscard]] virtual funcs_pairwise_comparing_result<SimilarityT> compare(
			const std::vector<code_analysis::func_id>& first_set_of_funcs,
			const std::vector<code_analysis::func_id>& second_set_of_funcs) = 0;
	};
}