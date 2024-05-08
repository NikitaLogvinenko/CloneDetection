#include "interpolation_by_neighborhood_with_weight.h"
#include "continuous_pixel.h"
#include <format>

interpolation_by_neighborhood_with_weight::interpolation_by_neighborhood_with_weight(
	const double neighborhood_half_side) : neighborhood_half_side_{neighborhood_half_side}
{
	if (neighborhood_half_side < min_half_side)
	{
		throw std::invalid_argument(std::format("half_side of square neighborhood must be equal or more than {} in order to always capture at least one pixel\n", 
			min_half_side));
	}
}

bmp::Pixel interpolation_by_neighborhood_with_weight::operator()(const bmp::Bitmap& image, const point_2d interpolated_point) const
{
	const neighborhood_bounds neighborhood = define_neighborhood_bounds(image, interpolated_point);
	if (neighborhood.x_begin() == neighborhood.x_end() || neighborhood.y_begin() == neighborhood.y_end())
	{
		throw std::invalid_argument("Image interpolation by square neighborhood with weight: no pixels in neighborhood, interpolated point is too far from image borders\n");
	}

	continuous_pixel interpolated_pixel{};

	for (std::int32_t x = neighborhood.x_begin(); x < neighborhood.x_end(); ++x)
	{
		for (std::int32_t y = neighborhood.y_begin(); y < neighborhood.y_end(); ++y)
		{
			continuous_pixel pixel_in_neighborhood(image.get(x, y));
			const double distance_along_x = interpolated_point.x() - x;
			const double distance_along_y = interpolated_point.y() - y;
			pixel_in_neighborhood *= weight(distance_along_x) * weight(distance_along_y);
			interpolated_pixel += pixel_in_neighborhood;
		}
	}
	return static_cast<bmp::Pixel>(interpolated_pixel);
}

interpolation_by_neighborhood_with_weight::neighborhood_bounds interpolation_by_neighborhood_with_weight::
define_neighborhood_bounds(const bmp::Bitmap& image, const point_2d interpolated_point) const noexcept
{
	if (interpolated_point.x() + neighborhood_half_side_ < 0 || interpolated_point.x() - neighborhood_half_side_ > image.width() - 1
		|| interpolated_point.y() + neighborhood_half_side_ < 0 || interpolated_point.y() - neighborhood_half_side_ > image.height() - 1)
	{
		return {};
	}

	const point_2d left_top_point = interpolated_point.shift_by(-neighborhood_half_side_, -neighborhood_half_side_);
	const point_2d right_bottom_point = interpolated_point.shift_by(neighborhood_half_side_, neighborhood_half_side_);
	const std::int32_t x_begin = std::max(static_cast<std::int32_t>(std::ceil(left_top_point.x())), 0);
	const std::int32_t x_end = std::min(static_cast<std::int32_t>(std::floor(right_bottom_point.x())), image.width() - 1) + 1;
	const std::int32_t y_begin = std::max(static_cast<std::int32_t>(std::ceil(left_top_point.y())), 0);
	const std::int32_t y_end = std::min(static_cast<std::int32_t>(std::floor(right_bottom_point.y())), image.height() - 1) + 1;

	return neighborhood_bounds{ x_begin, x_end, y_begin, y_end };
}
