#pragma once
#include "code_entities_pairwise_comparing_result.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"

namespace code_clones_analysis
{
	template <utility::id_concept IdT, utility::continuous_similarity_bounded_below SimilarityT>
	class code_entities_pairwise_comparer_abstract
	{
	protected:
		constexpr code_entities_pairwise_comparer_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(code_entities_pairwise_comparer_abstract)

		[[nodiscard]] virtual code_entities_pairwise_comparing_result<IdT, SimilarityT> compare(
			const std::vector<IdT>& first_set_of_funcs,
			const std::vector<IdT>& second_set_of_funcs) = 0;
	};
}