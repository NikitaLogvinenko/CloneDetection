#pragma once
#include "count_vectors_metrics_abstract.h"
#include "pow_throwing.h"
#include <algorithm>
#include <numeric>

namespace cm
{
	template <size_t CountVectorLength> requires count_vector_length<CountVectorLength>
	class cv_euclidean_distance : public count_vectors_metrics_abstract<CountVectorLength, double>
	{
	public:
		[[nodiscard]] distance_between_count_vectors<double> calculate(
			const count_vector<CountVectorLength>& first_vector,
			const count_vector<CountVectorLength>& second_vector) const override
		{
			std::array<size_t, CountVectorLength> squared_differences{};

			std::transform(first_vector.begin(), first_vector.end(),
				second_vector.begin(), squared_differences.begin(),
				[](const counted_value& value_from_first, const counted_value& value_from_second)
				{
					const size_t value_1 = value_from_first.to_size_t();
					const size_t value_2 = value_from_second.to_size_t();

					const auto pow_error_msg = "cv_euclidean_distance::cv_euclidean_distance: pow failed.";
					if (value_1 > value_2)
					{
						return utility::pow_throwing(value_1 - value_2, 2, pow_error_msg);
					}
					return utility::pow_throwing(value_2 - value_1, 2, pow_error_msg);
				});

			const size_t sum_of_squared_differences = std::accumulate(
				squared_differences.cbegin(), squared_differences.cend(), size_t{});
			return distance_between_count_vectors(graphs::edge_weight(std::sqrt(sum_of_squared_differences)));
		}
	};
}
