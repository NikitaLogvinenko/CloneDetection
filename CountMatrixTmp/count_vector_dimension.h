#pragma once

namespace cm
{
	constexpr size_t max_dimension = 100;

	template <size_t Dimension>
	concept count_vector_dimension = Dimension != 0 && Dimension <= max_dimension;
}
