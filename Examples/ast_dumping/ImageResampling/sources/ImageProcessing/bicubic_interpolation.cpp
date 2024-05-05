#include "bicubic_interpolation.h"

double bicubic_interpolation::weight(const double distance_along_axis) const noexcept
{
	const double module_of_distance = std::abs(distance_along_axis);
	if (module_of_distance >= half_side)
	{
		return 0;
	}
	if (module_of_distance < specific_distance)
	{
		return (module_of_distance * module_of_distance - 1) * (module_of_distance - 2) / 2;
	}
	return (module_of_distance - 1) * (module_of_distance - 2) * (module_of_distance - 3) / - 6;
}
