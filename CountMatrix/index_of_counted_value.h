#pragma once
#include "count_vector_length.h"
#include "invalid_index_error.h"

namespace cm
{
	template <size_t CountVectorLength> requires cm::count_vector_length<CountVectorLength>
	class index_of_counted_value final
	{
		size_t index_{};

	public:
		constexpr index_of_counted_value() noexcept = default;

		constexpr explicit index_of_counted_value(const size_t index) : index_(index)
		{
			if (index >= CountVectorLength)
			{
				throw common_exceptions::invalid_index_error("index_of_counted_value::index_of_counted_value: "
					"index must be less than CountVectorLength.");
			}
		}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}
