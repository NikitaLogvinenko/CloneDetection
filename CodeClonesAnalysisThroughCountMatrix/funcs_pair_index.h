#pragma once

namespace code_clones_analysis_through_cm
{
	class funcs_pair_index final
	{
		size_t index_{};

	public:
		constexpr funcs_pair_index() noexcept = default;

		constexpr explicit funcs_pair_index(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}
