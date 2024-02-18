#pragma once

namespace funcs_comparing_through_cm
{
	class matched_vars_index final
	{
		size_t index_{};

	public:
		constexpr matched_vars_index() noexcept = default;

		constexpr explicit matched_vars_index(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}
