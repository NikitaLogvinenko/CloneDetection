#pragma once
#include "non_const_integral.h"
#include "pow_throwing.h"
#include "initialized_value_out_of_range.h"

namespace cm
{
	template <utility::non_const_arithmetic WeightT, utility::non_const_integral CalculationsT>
	class floating_weight_to_integral_transformation final
	{
		double weights_factor_;

		static constexpr size_t default_decimals = 3;
		static constexpr size_t max_decimals = 7;

	public:
		constexpr explicit floating_weight_to_integral_transformation(const size_t weight_decimals = default_decimals)
			: weights_factor_(utility::pow_throwing(10, weight_decimals, 
				"floating_weight_to_integral_transformation: too large weight_decimals. Try to set less precision."))
		{
			if (weight_decimals > max_decimals)
			{
				throw common_exceptions::initialized_value_out_of_range(
					std::format("floating_weight_to_integral_transformation: max permitted "
				 "weight_decimals is {}, but {} was passed.", std::to_string(max_decimals), std::to_string(weight_decimals))
				);
			}
		}

		[[nodiscard]] CalculationsT to_calculations_t(const WeightT weight) const
		{
			return static_cast<CalculationsT>(std::llround(weight * weights_factor_));
		}

		[[nodiscard]] WeightT to_weight_t(const CalculationsT transformed_weight) const
		{
			return static_cast<WeightT>(transformed_weight / weights_factor_);
		}

		[[nodiscard]] constexpr static size_t default_weight_decimals() noexcept
		{
			return default_decimals;
		}

		[[nodiscard]] constexpr static size_t max_permitted_decimals() noexcept
		{
			return max_decimals;
		}
	};

	template <utility::non_const_integral WeightT, utility::non_const_integral CalculationsT>
	class floating_weight_to_integral_transformation<WeightT, CalculationsT> final
	{
	public:
		[[nodiscard]] static constexpr CalculationsT to_calculations_t(const WeightT weight)
		{
			return static_cast<CalculationsT>(weight);
		}

		[[nodiscard]] static constexpr WeightT to_weight_t(const CalculationsT transformed_weight)
		{
			return static_cast<WeightT>(transformed_weight);
		}
	};
}