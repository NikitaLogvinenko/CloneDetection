#pragma once
#include "matrices_comparing_traits.h"
#include "strict_weak_order.h"

namespace code_clones_analysis_through_cm
{
	template <class Traits>
	concept funcs_comparing_traits = cm::matrices_comparing_traits<Traits> && requires
	{
		typename Traits::similarity_greater;
		requires utility::strict_weak_order<typename Traits::similarity_t, typename Traits::similarity_greater>;
	};
}
