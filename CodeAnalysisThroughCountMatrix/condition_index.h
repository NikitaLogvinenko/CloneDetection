#pragma once
#include "index_of_counted_value.h"

namespace code_analysis_through_cm
{
	template <size_t ConditionsCount> requires cm::count_vector_length<ConditionsCount>
	class condition_index final
	{
		cm::index_of_counted_value<ConditionsCount> index_{};

	public:
		constexpr condition_index() noexcept = default;

		constexpr explicit condition_index(const size_t index) : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_.to_size_t();
		}
	};
}
