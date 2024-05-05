#pragma once

class point_2d final
{
	double x_{};
	double y_{};
public:
	point_2d() noexcept  = default;
	explicit point_2d(const double x, const double y) noexcept : x_{ x }, y_{ y } { }

	[[nodiscard]] double x() const noexcept
	{
		return  x_;
	}
	[[nodiscard]] double y() const noexcept
	{
		return  y_;
	}

	[[nodiscard]] point_2d shift_by(const double delta_x, const double delta_y) const noexcept
	{
		return point_2d(x_ + delta_x, y_ + delta_y);
	}
};
