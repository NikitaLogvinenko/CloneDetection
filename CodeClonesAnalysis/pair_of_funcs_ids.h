#pragma once
#include "func_id.h"

namespace code_clones_analysis
{
	class pair_of_funcs_ids final
	{
		const code_analysis::func_id first_id_{};
		const code_analysis::func_id second_id_{};

	public:
		constexpr pair_of_funcs_ids() noexcept = default;

		constexpr pair_of_funcs_ids(const code_analysis::func_id first_id, const code_analysis::func_id second_id) noexcept :
			first_id_(first_id), second_id_(second_id) {}

		[[nodiscard]] constexpr code_analysis::func_id first_id() const noexcept
		{
			return first_id_;
		}

		[[nodiscard]] constexpr code_analysis::func_id second_id() const noexcept
		{
			return second_id_;
		}

		[[nodiscard]] constexpr bool operator==(const pair_of_funcs_ids&) const noexcept = default;
	};
}
