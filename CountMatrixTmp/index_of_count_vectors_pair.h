#pragma once

namespace cm::internal
{
	class index_of_count_vectors_pair final
	{
		size_t index_{};

	public:
		constexpr index_of_count_vectors_pair() noexcept = default;

		constexpr explicit index_of_count_vectors_pair(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}
