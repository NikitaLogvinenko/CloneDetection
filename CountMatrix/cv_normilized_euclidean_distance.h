#pragma once
#include "cv_euclidean_distance.h"

namespace cm
{
	template <size_t CountVectorLength> requires count_vector_length<CountVectorLength>
	class cv_normilized_euclidean_distance : public cv_euclidean_distance<CountVectorLength>
	{
	public:
		[[nodiscard]] distance_between_count_vectors<double> calculate(
			const count_vector<CountVectorLength>& first_vector,
			const count_vector<CountVectorLength>& second_vector) const override
		{
			const distance_between_count_vectors<double> euclidean_distance = 
				cv_euclidean_distance<CountVectorLength>::calculate(first_vector, second_vector);

			std::array<size_t, CountVectorLength> max_squared_value_in_pair{};

			std::transform(first_vector.begin(), first_vector.end(), second_vector.begin(),
				max_squared_value_in_pair.begin(),
				[](const counted_value& value_from_first, const counted_value& value_from_second)
				{
					const size_t max_value_in_pair = std::max(value_from_first.to_size_t(), value_from_second.to_size_t());
					return utility::pow_throwing(max_value_in_pair, 2, "cv_normilized_euclidean_distance: pow failed");
				});

			const size_t sum_of_max_squared_values =
				std::accumulate(max_squared_value_in_pair.cbegin(), max_squared_value_in_pair.cend(), size_t{});

			if (sum_of_max_squared_values == 0)
			{
				return distance_between_count_vectors<double>{};
			}

			const double normilized_distance = euclidean_distance.to_edge_weight().value() / std::sqrt(sum_of_max_squared_values);
			return distance_between_count_vectors(graphs::edge_weight(
				std::clamp(normilized_distance, 0., 1.)));
		}
	};
}
