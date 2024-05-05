#pragma once

namespace cm
{
	class index_of_count_vector final
	{
		size_t index_{};

	public:
		constexpr index_of_count_vector() noexcept = default;

		constexpr explicit index_of_count_vector(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}
