#pragma once
#include "point_on_image_abstract_interpolation.h"

class interpolation_by_neighborhood_with_weight : public point_on_image_abstract_interpolation
{
	class neighborhood_bounds final
	{
		std::int32_t x_begin_{};
		std::int32_t x_end_{};
		std::int32_t y_begin_{};
		std::int32_t y_end_{};
	public:
		neighborhood_bounds() noexcept = default;
		explicit neighborhood_bounds(
			const std::int32_t x_begin, const std::int32_t x_end, const std::int32_t y_begin, const std::int32_t y_end) noexcept
			: x_begin_(x_begin), x_end_(x_end), y_begin_(y_begin), y_end_(y_end) { }

		[[nodiscard]] std::int32_t x_begin() const noexcept
		{
			return x_begin_;
		}
		[[nodiscard]] std::int32_t x_end() const noexcept
		{
			return x_end_;
		}
		[[nodiscard]] std::int32_t y_begin() const noexcept
		{
			return y_begin_;
		}
		[[nodiscard]] std::int32_t y_end() const noexcept
		{
			return y_end_;
		}
	};

protected:
	double neighborhood_half_side_{};

private:
	static constexpr double min_half_side = 0.5;

public:
	explicit interpolation_by_neighborhood_with_weight(double neighborhood_half_side);

	[[nodiscard]] bmp::Pixel operator()(const bmp::Bitmap& image, point_2d interpolated_point) const override;

	[[nodiscard]] double neighborhood_half_side() const
	{
		return neighborhood_half_side_;
	}

protected:
	[[nodiscard]] virtual double weight(double distance_along_axis) const noexcept = 0;

private:
	[[nodiscard]] neighborhood_bounds define_neighborhood_bounds(const bmp::Bitmap& image, point_2d interpolated_point) const noexcept;
};
