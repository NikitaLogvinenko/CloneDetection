#pragma once
#include "interpolation_by_neighborhood_with_weight.h"

class lanczos_interpolation final : public interpolation_by_neighborhood_with_weight
{
public:
	explicit lanczos_interpolation(const double a) : interpolation_by_neighborhood_with_weight(a) { }
protected:
	[[nodiscard]] double weight(double distance_along_axis) const noexcept override;
};
