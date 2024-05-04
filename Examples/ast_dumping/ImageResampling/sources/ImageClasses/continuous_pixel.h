#pragma once
#include "BitmapPlusPlus.h"

class continuous_pixel final
{
	double r_{};
	double g_{};
	double b_{};
public:
	continuous_pixel() noexcept = default;
	explicit continuous_pixel(const bmp::Pixel& pixel) noexcept : r_(pixel.r), g_(pixel.g), b_(pixel.b) { }
	explicit continuous_pixel(const double r, const double g, const double b) noexcept : r_(r), g_(g), b_(b) { }

	[[nodiscard]] explicit operator bmp::Pixel() const noexcept
	{
		return { clip(r_), clip(g_), clip(b_) };
	}

	continuous_pixel& operator+=(const continuous_pixel& other) noexcept;
	continuous_pixel& operator*=(double value) noexcept;

private:
	[[nodiscard]] static std::uint8_t clip(double color) noexcept;
};
