#pragma once
#include "index_of_counted_value.h"

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class condition_index final
	{
		cm::index_of_counted_value<UsageConditionsCount> index_{};

	public:
		constexpr condition_index() noexcept = default;

		constexpr explicit condition_index(const size_t index) : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_.to_size_t();
		}
	};
}
