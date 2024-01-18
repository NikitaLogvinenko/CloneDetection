#pragma once
#include "matrices_comparing_traits.h"
#include "comparator.h"

namespace funcs_comparing_through_cm
{
	template <class Traits>
	concept funcs_comparing_traits = cm::matrices_comparing_traits<Traits> && requires
	{
		Traits::similarity_greater;
		requires utility::comparator<typename Traits::similarity_t, typename Traits::similarity_greater>;
	};
}
