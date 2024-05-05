#pragma once
#include "BitmapPlusPlus.h"
#include "point_2d.h"

class point_on_image_abstract_interpolation
{
public:
	virtual ~point_on_image_abstract_interpolation() noexcept = default;

	[[nodiscard]] virtual bmp::Pixel operator()(const bmp::Bitmap& image, point_2d interpolated_point) const = 0;
};
