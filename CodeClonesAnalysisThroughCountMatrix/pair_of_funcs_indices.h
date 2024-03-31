#pragma once
#include "func_index.h"

namespace code_clones_analysis_through_cm
{
	class pair_of_funcs_indices final
	{
		const func_index first_index_{};
		const func_index second_index_{};

	public:
		constexpr pair_of_funcs_indices() noexcept = default;

		constexpr pair_of_funcs_indices(const func_index first_index, const func_index second_index) noexcept :
		first_index_(first_index), second_index_(second_index) {}

		[[nodiscard]] constexpr func_index first_index() const noexcept
		{
			return first_index_;
		}

		[[nodiscard]] constexpr func_index second_index() const noexcept
		{
			return second_index_;
		}

		[[nodiscard]] constexpr bool operator==(const pair_of_funcs_indices&) const noexcept = default;

		[[nodiscard]] constexpr auto operator<=>(const pair_of_funcs_indices&) const noexcept = default;
	};
}
