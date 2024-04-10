#pragma once
#include "continuous_similarity_bounded_below.h"

namespace utility
{
	template <class T>
	concept continuous_similarity_bounded = continuous_similarity_bounded_below<T> &&
		requires ()
	{
		{ T::max_similarity_value() } noexcept -> std::same_as<double>;
	};
}
