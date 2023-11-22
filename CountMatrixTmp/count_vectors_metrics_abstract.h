#pragma once
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
#include "count_vector.h"
#include "distance_between_count_vectors.h"

namespace cm
{
	template <size_t CountVectorDimension, utility::non_const_arithmetic DistanceT>
	requires count_vector_dimension<CountVectorDimension>
	class count_vectors_metrics_abstract
	{
	protected:
		constexpr count_vectors_metrics_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(count_vectors_metrics_abstract)

		[[nodiscard]] virtual distance_between_count_vectors<DistanceT> calculate(
			const count_vector<CountVectorDimension>& first_vector,
			const count_vector<CountVectorDimension>& second_vector) const = 0;
	};
}
