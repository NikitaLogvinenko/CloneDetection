#pragma once
#include "matrices_comparing_result.h"
#include "bipartite_matching_abstract.h"

namespace cm
{
	template <size_t CountVectorDimension, utility::non_const_arithmetic WeightT, continuous_similarity_bounded_below SimilarityT>
		requires count_vector_dimension<CountVectorDimension>
	class matrices_comparing_abstract
	{
	protected:
		constexpr matrices_comparing_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(matrices_comparing_abstract)

		[[nodiscard]] virtual matrices_comparing_result<CountVectorDimension, WeightT, SimilarityT> operator()(
			const count_matrix<CountVectorDimension>& first_matrix, 
			const count_matrix<CountVectorDimension>& second_matrix) const = 0;
	};
}
