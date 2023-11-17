#pragma once

namespace cm
{
	class index_of_counted_value final
	{
		size_t index_{};

	public:
		constexpr index_of_counted_value() noexcept = default;

		constexpr explicit index_of_counted_value(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}
