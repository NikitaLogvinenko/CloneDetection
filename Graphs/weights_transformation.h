#pragma once
#include "non_const_integral.h"
#include "non_const_arithmetic.h"
#include <concepts>

namespace graphs
{
	template <class WeightsTransform, utility::non_const_arithmetic WeightT, utility::non_const_integral CalculationsT>
	concept weights_transformation = (std::is_copy_constructible_v<WeightsTransform> 
		|| std::is_default_constructible_v<WeightsTransform>)
		&& requires(WeightsTransform transform, const WeightT weight_t, const CalculationsT calculations_t)
	{
		{ transform.to_weight_t(calculations_t) } -> std::same_as<WeightT>;
		{ transform.to_calculations_t(weight_t) } -> std::same_as<CalculationsT>;
	};
}
