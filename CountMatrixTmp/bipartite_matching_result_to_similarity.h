#pragma once
#include "bipartite_graph_weights_matrix.h"
#include "bipartite_matching_result.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
#include "similarity_concept.h"

namespace cm
{
	template <utility::non_const_arithmetic WeightT, similarity_concept SimilarityT>
	class bipartite_matching_result_to_similarity
	{
	public:
		constexpr bipartite_matching_result_to_similarity() noexcept = default;
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(bipartite_matching_result_to_similarity)

		[[nodiscard]] constexpr virtual SimilarityT operator()(
			const bipartite_graph_weights_matrix<WeightT>& weights_matrix,
			const bipartite_matching_result<WeightT>& matching_result) const = 0;
	};
}