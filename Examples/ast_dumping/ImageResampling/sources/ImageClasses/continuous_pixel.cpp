#include "continuous_pixel.h"
#include <cmath>
#include <limits>
#include <algorithm>

namespace
{
	constexpr double min_color_value = std::numeric_limits<std::uint8_t>::min();
	constexpr double max_color_value = std::numeric_limits<std::uint8_t>::max();
}

continuous_pixel& continuous_pixel::operator+=(const continuous_pixel& other) noexcept
{
	r_ += other.r_;
	g_ += other.g_;
	b_ += other.b_;
	return *this;
}

continuous_pixel& continuous_pixel::operator*=(const double value) noexcept
{
	r_ *= value;
	g_ *= value;
	b_ *= value;
	return *this;
}

std::uint8_t continuous_pixel::clip(const double color) noexcept
{
	return static_cast<std::uint8_t>(std::lround(std::clamp(color, min_color_value, max_color_value)));
}