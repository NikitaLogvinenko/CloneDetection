#pragma once
#include "cm_comparing_result.h"
#include "bipartite_matching_abstract.h"

namespace cm
{
	template <size_t CountVectorDimension, utility::non_const_arithmetic WeightT, similarity_concept SimilarityT>
		requires cv_dimension_concept<CountVectorDimension>
	class cm_comparing_abstract
	{
	public:
		constexpr cm_comparing_abstract() noexcept = default;
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(cm_comparing_abstract)

		[[nodiscard]] virtual cm_comparing_result<CountVectorDimension, WeightT, SimilarityT> operator()(
			const count_matrix<CountVectorDimension>& first_cm, 
			const count_matrix<CountVectorDimension>& second_cm) const = 0;
	};
}
