#pragma once
#include "matrices_comparing_traits.h"

namespace funcs_comparing_through_cm
{
	template <class Traits>
	concept funcs_comparing_traits = cm::matrices_comparing_traits<Traits>;
}
