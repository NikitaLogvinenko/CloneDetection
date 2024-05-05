#pragma once

namespace cm
{
	class count_vector_index final
	{
		size_t index_{};

	public:
		constexpr count_vector_index() noexcept = default;

		constexpr explicit count_vector_index(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}
