#pragma once

namespace cm
{
	class matched_vectors_pair_index final
	{
		size_t index_{};

	public:
		constexpr matched_vectors_pair_index() noexcept = default;

		constexpr explicit matched_vectors_pair_index(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}