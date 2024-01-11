#pragma once
#include "matrices_comparing_result.h"
#include "bipartite_matching_abstract.h"

namespace cm
{
	template <size_t CountVectorLength, utility::non_const_arithmetic WeightT, continuous_similarity_bounded_below SimilarityT>
		requires count_vector_length<CountVectorLength>
	class matrices_comparing_abstract
	{
	public:
		constexpr matrices_comparing_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(matrices_comparing_abstract)

		[[nodiscard]] virtual matrices_comparing_result<CountVectorLength, WeightT, SimilarityT> operator()(
			const count_matrix<CountVectorLength>& first_matrix, 
			const count_matrix<CountVectorLength>& second_matrix) const = 0;
	};
}
