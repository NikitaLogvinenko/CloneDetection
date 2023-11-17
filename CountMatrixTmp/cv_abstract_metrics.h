#pragma once
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
#include "count_vector.h"
#include "distance_between_cv.h"

namespace cm
{
	template <size_t CountVectorDimension, utility::non_const_arithmetic DistanceT>
	requires cv_dimension_concept<CountVectorDimension>
	class cv_abstract_metrics
	{
	public:
		constexpr cv_abstract_metrics() noexcept = default;
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(cv_abstract_metrics)

		[[nodiscard]] virtual distance_between_cv<DistanceT> calculate(
			const count_vector<CountVectorDimension>& first_vector,
			const count_vector<CountVectorDimension>& second_vector) const = 0;
	};
}
