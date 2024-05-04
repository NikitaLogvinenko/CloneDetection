#include "image_resampler.h"

bmp::Bitmap image_resampler::resample(const bmp::Bitmap& initial_image, const std::int32_t resampled_width,
	const std::int32_t resampled_height, const point_on_image_abstract_interpolation& interpolation)
{
	if (initial_image.width() == 0 && initial_image.height() == 0 && resampled_width == 0 && resampled_height == 0)
	{
		return {};
	}
	params_validation(initial_image, resampled_width, resampled_height);

	bmp::Bitmap resampled_image(resampled_width, resampled_height);
	for (std::int32_t resampled_x = 0; resampled_x < resampled_width; ++resampled_x)
	{
		for (std::int32_t resampled_y = 0; resampled_y < resampled_height; ++resampled_y)
		{
			const double x_in_old_scale = calc_coordinate_in_old_scale(initial_image.width(), resampled_width, resampled_x);
			const double y_in_old_scale = calc_coordinate_in_old_scale(initial_image.height(), resampled_height, resampled_y);
			resampled_image.get(resampled_x, resampled_y) = interpolation(initial_image, point_2d(x_in_old_scale, y_in_old_scale));
		}
	}
	return resampled_image;
}

double image_resampler::calc_coordinate_in_old_scale(const std::int32_t initial_side_length, const std::int32_t resampled_side_length,
                                                     const double resampled_coordinate)
{
	constexpr double min_border = -0.5;
	const double resampled_shift_from_min_border = resampled_coordinate - min_border;
	const double stretching_factor = static_cast<double>(resampled_side_length) / initial_side_length;
	const double initial_shift_from_min_border = resampled_shift_from_min_border / stretching_factor;
	const double initial_coordinate = min_border + initial_shift_from_min_border;
	return initial_coordinate;
}

void image_resampler::params_validation(const bmp::Bitmap& initial_image, const std::int32_t resampled_width, const std::int32_t resampled_height)
{
	if (resampled_height < 0 || resampled_width < 0)
	{
		throw std::invalid_argument("Image can not have negative sizes\n");
	}
	if (resampled_height == 0 || resampled_width == 0)
	{
		throw std::invalid_argument("Can not resample to empty image\n");
	}
	if (initial_image.width() == 0 || initial_image.height() == 0)
	{
		throw std::invalid_argument("Can not resample empty image\n");
	}
}
