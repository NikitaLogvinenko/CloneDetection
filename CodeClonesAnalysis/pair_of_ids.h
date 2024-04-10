#pragma once
#include "id_concept.h"

namespace code_clones_analysis
{
	template <utility::id_concept IdT>
	class pair_of_ids final
	{
		const IdT first_id_{};
		const IdT second_id_{};

	public:
		constexpr pair_of_ids() noexcept = default;

		constexpr pair_of_ids(const IdT first_id, const IdT second_id) noexcept : first_id_(first_id), second_id_(second_id) {}

		[[nodiscard]] constexpr IdT first_id() const noexcept
		{
			return first_id_;
		}

		[[nodiscard]] constexpr IdT second_id() const noexcept
		{
			return second_id_;
		}

		[[nodiscard]] constexpr bool operator==(const pair_of_ids&) const noexcept = default;
	};
}
