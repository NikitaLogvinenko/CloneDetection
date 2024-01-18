#pragma once
#include "bipartite_graph_weights_matrix.h"
#include "bipartite_matching_result.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"

namespace graphs
{
	template <utility::non_const_arithmetic WeightT>
	class bipartite_matching_abstract
	{
	public:
		constexpr bipartite_matching_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(bipartite_matching_abstract)

		[[nodiscard]] virtual bipartite_matching_result<WeightT> match_parts(
			const bipartite_graph_weights_matrix<WeightT>& bipartite_graph_weights) const = 0;
	};
}
