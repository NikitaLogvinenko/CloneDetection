#pragma once
#include "cv_abstract_metrics.h"
#include "pow_throwing.h"
#include <algorithm>
#include <numeric>

namespace cm
{
	template <size_t CountVectorDimension> requires cv_dimension_concept<CountVectorDimension>
	class cv_euclidean_distance : public cv_abstract_metrics<CountVectorDimension, double>
	{
	public:
		[[nodiscard]] distance_between_cv<double> calculate(
			const count_vector<CountVectorDimension>& first_vector,
			const count_vector<CountVectorDimension>& second_vector) const override
		{
			std::array<size_t, CountVectorDimension> squared_differences{};

			std::transform(first_vector.begin(), first_vector.end(),
				second_vector.begin(), squared_differences.begin(),
				[](const counted_value& value_from_first, const counted_value& value_from_second)
				{
					const auto& value_1 = value_from_first.value();
					const auto& value_2 = value_from_second.value();
					if (value_1 > value_2)
					{
						return utility::pow_throwing(value_1 - value_2, 2, "euclidean_distance: pow failed");
					}
					return utility::pow_throwing(value_2 - value_1, 2, "euclidean_distance: pow failed");
				});

			const size_t sum_of_squared_differences = std::accumulate(
				squared_differences.cbegin(), squared_differences.cend(), size_t{});
			return distance_between_cv(std::sqrt(sum_of_squared_differences));
		}
	};
}