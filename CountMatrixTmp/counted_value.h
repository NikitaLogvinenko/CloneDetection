#pragma once

namespace cm
{
	class counted_value final
	{
		size_t value_{};

	public:
		constexpr counted_value() noexcept = default;

		constexpr explicit counted_value(const size_t value) noexcept : value_(value) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return value_;
		}

		constexpr counted_value& operator++() noexcept
		{
			++value_;
			return *this;
		}
	};
}
