#pragma once
#include "BitmapPlusPlus.h"
#include "point_on_image_abstract_interpolation.h"

class image_resampler final
{
public:
	static [[nodiscard]] bmp::Bitmap resample(const bmp::Bitmap& initial_image, 
		std::int32_t resampled_width, std::int32_t resampled_height, 
		const point_on_image_abstract_interpolation& interpolation);

private:
	[[nodiscard]] static double calc_coordinate_in_old_scale(std::int32_t initial_side_length, std::int32_t resampled_side_length, double resampled_coordinate);

	static void params_validation(const bmp::Bitmap& initial_image,	std::int32_t resampled_width, std::int32_t resampled_height);
};
