#pragma once
#include "count_vector_length.h"
#include "invalid_index_exception.h"

namespace functions_analysis
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class condition_index final
	{
		size_t index_{};

	public:
		constexpr condition_index() noexcept = default;

		constexpr explicit condition_index(const size_t index) : index_(index)
		{
			if (index >= UsageConditionsCount)
			{
				throw common_exceptions::invalid_index_exception("condition_index: index must be less than UsageConditionsCount.");
			}
		}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}
