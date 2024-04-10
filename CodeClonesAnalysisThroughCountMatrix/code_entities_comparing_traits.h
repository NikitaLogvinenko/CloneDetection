#pragma once
#include "matrices_comparing_traits.h"
#include "code_analysis_traits.h"
#include "strict_weak_order.h"

namespace code_clones_analysis_through_cm
{
	template <class Traits>
	concept code_entities_comparing_traits = requires
	{
		typename Traits::matrices_comparing_traits;
		requires cm::matrices_comparing_traits<typename Traits::matrices_comparing_traits>;

		typename Traits::code_analysis_traits;
		requires code_analysis_through_cm::code_analysis_traits<typename Traits::code_analysis_traits>;

		requires Traits::matrices_comparing_traits::count_vector_length == Traits::code_analysis_traits::conditions_count;

		//typename Traits::similarity_greater;
		//requires utility::strict_weak_order<typename Traits::matrices_comparing_traits::similarity_t, typename Traits::similarity_greater>;
	};
}
