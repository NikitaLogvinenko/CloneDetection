#pragma once
#include "continuous_similarity_bounded_below.h"
#include "count_vector_length.h"
#include "non_const_arithmetic.h"

namespace cm
{
	template <class Traits>
	concept matrices_comparing_traits = requires
	{
		Traits::count_vector_length;
		requires count_vector_length<Traits::count_vector_length>;

		typename Traits::distance_t;
		requires utility::non_const_arithmetic<typename Traits::distance_t>;

		typename Traits::similarity_t;
		requires continuous_similarity_bounded_below<typename Traits::similarity_t>;
	};
}
