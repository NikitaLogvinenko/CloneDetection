#pragma once
#include "bipartite_graph_weights_matrix.h"
#include "bipartite_matching_result.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
#include "continuous_similarity_bounded_below.h"

namespace cm
{
	template <utility::non_const_arithmetic WeightT, continuous_similarity_bounded_below SimilarityT>
	class similarity_estimator_abstract
	{
	public:
		constexpr similarity_estimator_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(similarity_estimator_abstract)

		[[nodiscard]] constexpr virtual SimilarityT operator()(
			const bipartite_graph_weights_matrix<WeightT>& weights_matrix,
			const bipartite_matching_result<WeightT>& matching_result) const = 0;
	};
}
