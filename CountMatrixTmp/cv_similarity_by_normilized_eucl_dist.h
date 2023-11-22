﻿#pragma once
#include "cv_normilized_euclidean_distance.h"
#include "incorrect_order_exception.h"

namespace cm
{
	template <size_t CountVectorDimension> requires count_vector_dimension<CountVectorDimension>
	class cv_similarity_by_normilized_eucl_dist final : public cv_normilized_euclidean_distance<CountVectorDimension>
	{
		double min_value_{0};
		double max_value_{1};

	public:
		constexpr cv_similarity_by_normilized_eucl_dist() noexcept = default;

		constexpr explicit cv_similarity_by_normilized_eucl_dist(const double min_value, const double max_value) :
			min_value_(min_value), max_value_(max_value)
		{
			if (min_value_ > max_value_)
			{
				throw common_exceptions::incorrect_order_exception(
					"cv_normilized_euclidean_distance: min_value can not be greater than max_value.");
			}
		}

		[[nodiscard]] constexpr double min_value() const noexcept
		{
			return min_value_;
		}

		[[nodiscard]] constexpr double max_value() const noexcept
		{
			return max_value_;
		}

		[[nodiscard]] distance_between_count_vectors<double> calculate(
			const count_vector<CountVectorDimension>& first_vector,
			const count_vector<CountVectorDimension>& second_vector) const override
		{
			const distance_between_count_vectors<double> normilized_distance = cv_normilized_euclidean_distance<CountVectorDimension>::calculate(first_vector, second_vector);

			const double similarity_ratio = 1 - normilized_distance.to_edge_weight().value();
			const double scale = max_value_ - min_value_;
			const double similarity = min_value_ + scale * similarity_ratio;

			return distance_between_count_vectors(std::clamp(similarity, min_value_, max_value_));
		}
	};
}