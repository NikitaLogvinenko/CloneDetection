#pragma once
#include "cv_euclidean_distance.h"

namespace cm
{
	template <size_t CountVectorDimension> requires cv_dimension_concept<CountVectorDimension>
	class cv_normilized_euclidean_distance : public cv_euclidean_distance<CountVectorDimension>
	{
	public:
		[[nodiscard]] distance_between_cv<double> calculate(
			const count_vector<CountVectorDimension>& first_vector,
			const count_vector<CountVectorDimension>& second_vector) const override
		{
			const distance_between_cv<double> euclidean_distance = cv_euclidean_distance<CountVectorDimension>::calculate(first_vector, second_vector);

			std::array<size_t, CountVectorDimension> max_squared_value_in_pair{};

			std::transform(first_vector.begin(), first_vector.end(), second_vector.begin(),
				max_squared_value_in_pair.begin(),
				[](const counted_value& value_from_first, const counted_value& value_from_second)
				{
					const auto& max_value_in_pair = std::max(value_from_first.value(), value_from_second.value());
					return utility::pow_throwing(max_value_in_pair, 2, "cv_normilized_euclidean_distance: pow failed");
				});

			const size_t sum_of_max_squared_values =
				std::accumulate(max_squared_value_in_pair.cbegin(), max_squared_value_in_pair.cend(), size_t{});

			if (sum_of_max_squared_values == 0)
			{
				return distance_between_cv<double>{};
			}

			const double normilized_distance = euclidean_distance.to_edge_weight().value() / std::sqrt(sum_of_max_squared_values);
			return distance_between_cv(std::clamp(normilized_distance, 0., 1.));
		}
	};
}