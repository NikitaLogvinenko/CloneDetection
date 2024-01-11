#pragma once

namespace cm
{
	class counted_value final
	{
		size_t value_{};

	public:
		counted_value() noexcept = default;

		explicit counted_value(const size_t value) noexcept : value_(value) {}

		[[nodiscard]] size_t to_size_t() const noexcept
		{
			return value_;
		}

		counted_value& operator++() noexcept
		{
			++value_;
			return *this;
		}
	};
}
