#pragma once
#include "non_const_integral.h"
#include "pow_throwing.h"
#include "initialized_value_out_of_range.h"

namespace cm
{
	template <utility::non_const_arithmetic WeightT, utility::non_const_integral CalculationsT>
	class weights_transformation final
	{
		double weights_factor_;

		static constexpr size_t default_decimals = 3;
		static constexpr size_t max_decimals = 7;

	public:
		constexpr explicit weights_transformation(const size_t weight_decimals = default_decimals)
			: weights_factor_(utility::pow_throwing(10, weight_decimals, 
				"weights_transformation: too large weight_decimals. Try to set less precision."))
		{
			if (weight_decimals > max_decimals)
			{
				throw common_exceptions::initialized_value_out_of_range(
					std::format("weights_transformation: max permitted weight_decimals is {}, but {} was passed.",
						std::to_string(max_decimals), std::to_string(weight_decimals))
				);
			}
		}

		[[nodiscard]] CalculationsT operator()(const WeightT weight) const
		{
			return static_cast<CalculationsT>(std::llround(weight * weights_factor_));
		}

		[[nodiscard]] WeightT operator()(const CalculationsT transformed_weight) const
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
	class weights_transformation<WeightT, CalculationsT> final
	{
	public:
		[[nodiscard]] constexpr CalculationsT operator()(const WeightT weight) const
		{
			return static_cast<CalculationsT>(weight);
		}

		[[nodiscard]] constexpr WeightT operator()(const CalculationsT transformed_weight) const
		{
			return static_cast<WeightT>(transformed_weight);
		}
	};
}