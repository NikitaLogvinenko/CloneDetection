#include "lanczos_interpolation.h"
#include <cmath>
#include <numbers>

namespace 
{
	constexpr double almost_zero = 1e-7;
	double sinc(const double x)
	{
		if (std::abs(x) < almost_zero)
		{
			return 1;
		}
		return std::sin(std::numbers::pi * x) / (std::numbers::pi * x);
	}
}

double lanczos_interpolation::weight(const double distance_along_axis) const noexcept
{
	if (std::abs(distance_along_axis) >= neighborhood_half_side_)
	{
		return 0;
	}
	
	return sinc(distance_along_axis) * sinc(distance_along_axis / neighborhood_half_side_);
}
