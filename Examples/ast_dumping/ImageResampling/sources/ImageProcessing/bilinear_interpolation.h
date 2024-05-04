#pragma once
#include "interpolation_by_neighborhood_with_weight.h"

class bilinear_interpolation final : public interpolation_by_neighborhood_with_weight
{
	static constexpr size_t half_side = 1;

public:
	bilinear_interpolation() noexcept : interpolation_by_neighborhood_with_weight(half_side) {}
protected:
	[[nodiscard]] double weight(const double distance_along_axis) const noexcept override
	{
		return std::max(half_side - std::abs(distance_along_axis), 0.0);
	}
};
