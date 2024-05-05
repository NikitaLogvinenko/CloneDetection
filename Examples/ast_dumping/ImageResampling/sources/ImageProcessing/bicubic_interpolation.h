#pragma once
#include "interpolation_by_neighborhood_with_weight.h"

class bicubic_interpolation final : public interpolation_by_neighborhood_with_weight
{
	static constexpr size_t specific_distance = 1;
	static constexpr size_t half_side = 2;

public:
	bicubic_interpolation() noexcept : interpolation_by_neighborhood_with_weight(half_side) {}
protected:
	[[nodiscard]] double weight(double distance_along_axis) const noexcept override;
};
