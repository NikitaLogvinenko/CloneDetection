#pragma once

namespace cm
{
	class matching_edge_index final
	{
		size_t index_{};

	public:
		constexpr matching_edge_index() noexcept = default;

		constexpr explicit matching_edge_index(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}
