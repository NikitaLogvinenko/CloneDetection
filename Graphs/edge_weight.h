#pragma once
#include "param_out_of_range_error.h"
#include "non_const_arithmetic.h"
#include "non_const_unsigned_arithmetic.h"

namespace graphs
{
	template <utility::non_const_arithmetic WeightT>
	class edge_weight final
	{
		WeightT value_{};

	public:
		constexpr edge_weight() noexcept = default;

		constexpr explicit edge_weight(const WeightT value) : value_(value)
		{
			if (value < 0)
			{
				throw common_exceptions::param_out_of_range_error(
					"edge_weight::edge_weight: value can not be negative.");
			}
		}

		[[nodiscard]] constexpr WeightT value() const noexcept
		{
			return value_;
		}
	};

	template <utility::non_const_unsigned_arithmetic WeightT>
	class edge_weight<WeightT> final
	{
		WeightT value_{};

	public:
		constexpr edge_weight() noexcept = default;

		constexpr explicit edge_weight(const WeightT value) noexcept : value_(value) {}

		[[nodiscard]] constexpr WeightT value() const noexcept
		{
			return value_;
		}
	};
}
