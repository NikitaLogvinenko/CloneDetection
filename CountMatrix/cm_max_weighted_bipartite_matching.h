#pragma once
#include "cm_abstract_similarity.h"
#include "dlib/optimization/max_cost_assignment.h"

namespace count_matrix
{
	template <size_t CountVectorDimension>
	class cm_max_weighted_bipartite_matching final : public cm_abstract_similarity<CountVectorDimension>
	{
		size_t weights_decimal_precision_{};
		size_t weights_factor_{};

		static constexpr size_t default_precision = 3;
		static constexpr double min_not_zero_distance = 1e-7;
		static constexpr size_t index_of_row_in_pair = 0;
		static constexpr size_t index_of_column_in_pair = 1;

	public:
		cm_max_weighted_bipartite_matching() noexcept
			: cm_max_weighted_bipartite_matching(default_precision) {}
		explicit cm_max_weighted_bipartite_matching(const size_t weights_decimal_precision)
			: weights_decimal_precision_(weights_decimal_precision), weights_factor_(pow(10, weights_decimal_precision)) {}

		[[nodiscard]] cm_similarity_result calc_similarity(const count_matrix<CountVectorDimension>& first_cm,
			const count_matrix<CountVectorDimension>& second_cm) const override;

		[[nodiscard]] static size_t default_weights_decimal_precision() noexcept
		{
			return default_precision;
		}

	private:
		[[nodiscard]] dlib::matrix<size_t> create_assignment_matrix(
			const count_matrix<CountVectorDimension>& first_cm,
			const count_matrix<CountVectorDimension>& second_cm) const;

		[[nodiscard]] size_t calc_assignment_matrix_cell(
			const count_vector<CountVectorDimension>& first_vector,
			const count_vector<CountVectorDimension>& second_vector) const noexcept;

		[[nodiscard]] cm_similarity_result assign(
			const count_matrix<CountVectorDimension>& first_cm,
			const count_matrix<CountVectorDimension>& second_cm,
			const dlib::matrix<size_t>& assignment_matrix) const;

		[[nodiscard]] std::vector<cm_similar_vectors_indices> create_similar_vectors_pairs(
			const dlib::matrix<size_t>& assignment_matrix, const std::vector<long>& assignment,
			size_t min_vectors_count, size_t first_matrix_index_in_pair, size_t second_matrix_index_in_pair) const;

		[[nodiscard]] static double calc_vector_l2_norm(const count_vector<CountVectorDimension>& vector) noexcept;

		[[nodiscard]] static double calc_vectors_distance(
			const count_vector<CountVectorDimension>& first_vector,
			const count_vector<CountVectorDimension>& second_vector) noexcept;

		[[nodiscard]] static double normilize_distance(
			const count_vector<CountVectorDimension>& first_vector,
			const count_vector<CountVectorDimension>& second_vector,
			double distance) noexcept;
	};


	template <size_t CountVectorDimension>
	cm_similarity_result cm_max_weighted_bipartite_matching<CountVectorDimension>::calc_similarity(
		const count_matrix<CountVectorDimension>& first_cm, const count_matrix<CountVectorDimension>& second_cm) const
	{
		if (first_cm.empty())
		{
			return second_cm.empty() ? cm_similarity_result(1) : cm_similarity_result(0);
		}

		const dlib::matrix<size_t> assignment_matrix(create_assignment_matrix(first_cm, second_cm));
		return assign(first_cm, second_cm, assignment_matrix);
	}

	template <size_t CountVectorDimension>
	dlib::matrix<size_t> cm_max_weighted_bipartite_matching<CountVectorDimension>::create_assignment_matrix(
		const count_matrix<CountVectorDimension>& first_cm, const count_matrix<CountVectorDimension>& second_cm) const
	{
		const count_matrix<CountVectorDimension>* matrix_less_vectors_ptr, * matrix_more_vectors_ptr;
		if (first_cm.vectors_count() < second_cm.vectors_count())
		{
			matrix_less_vectors_ptr = &first_cm;
			matrix_more_vectors_ptr = &second_cm;
		}
		else
		{
			matrix_less_vectors_ptr = &second_cm;
			matrix_more_vectors_ptr = &first_cm;
		}

		dlib::matrix<size_t> assignment_matrix(matrix_more_vectors_ptr->vectors_count(), matrix_more_vectors_ptr->vectors_count());
		size_t row = 0, col = 0;
		for (const auto& vector_from_greater_matrix : *matrix_more_vectors_ptr)
		{
			for (const auto& vector_from_less_matrix : *matrix_less_vectors_ptr)
			{
				assignment_matrix(row, col) = calc_assignment_matrix_cell(vector_from_less_matrix, vector_from_greater_matrix);
				++col;
			}
			while (col != matrix_more_vectors_ptr->vectors_count())
			{
				assignment_matrix(row, col) = calc_assignment_matrix_cell(count_vector<CountVectorDimension>(), vector_from_greater_matrix);
				++col;
			}
			col = 0;
			++row;
		}

		return assignment_matrix;
	}

	template <size_t CountVectorDimension>
	size_t cm_max_weighted_bipartite_matching<CountVectorDimension>::calc_assignment_matrix_cell(
		const count_vector<CountVectorDimension>& first_vector,
		const count_vector<CountVectorDimension>& second_vector) const noexcept
	{
		const double vectors_distance = calc_vectors_distance(first_vector, second_vector);
		const double vectors_similarity = std::clamp(1.0 - normilize_distance(first_vector, second_vector, vectors_distance), 0.0, 1.0);
		return std::lround(vectors_similarity * weights_factor_);
	}

	template <size_t CountVectorDimension>
	cm_similarity_result cm_max_weighted_bipartite_matching<CountVectorDimension>::assign(
		const count_matrix<CountVectorDimension>& first_cm, const count_matrix<CountVectorDimension>& second_cm,
		const dlib::matrix<size_t>& assignment_matrix) const
	{
		const std::vector assignment(std::move(max_cost_assignment(assignment_matrix)));
		const double max_sum_of_vectors_similarities = assignment_cost(assignment_matrix, assignment) / weights_factor_;
		const double matrices_similarity = std::clamp(max_sum_of_vectors_similarities / assignment_matrix.nc(), 0.0, 1.0);

		std::vector<cm_similar_vectors_indices> similar_vectors{};
		if (first_cm.vectors_count() < second_cm.vectors_count())
		{
			similar_vectors(std::move(create_similar_vectors_pairs(
				assignment_matrix, assignment, first_cm.vectors_count(),
				index_of_column_in_pair, index_of_row_in_pair)));
		}
		else
		{
			similar_vectors(std::move(create_similar_vectors_pairs(
				assignment_matrix, assignment, second_cm.vectors_count(),
				index_of_row_in_pair, index_of_column_in_pair)));
		}

		return cm_similarity_result(matrices_similarity, similar_vectors);
	}

	template <size_t CountVectorDimension>
	std::vector<cm_similar_vectors_indices> cm_max_weighted_bipartite_matching<CountVectorDimension>::create_similar_vectors_pairs(
		const dlib::matrix<size_t>& assignment_matrix, const std::vector<long>& assignment, const size_t min_vectors_count,
		const size_t first_matrix_index_in_pair, const size_t second_matrix_index_in_pair) const
	{
		std::vector<cm_similar_vectors_indices> similar_vectors{};
		similar_vectors.reserve(min_vectors_count);
		long row = 0;
		for (const long assigned_column& : assignment)
		{
			if (assigned_column < min_vectors_count)
			{
				const std::array row_column_pair{ static_cast<size_t>(row), static_cast<size_t>(assigned_column) };
				similar_vectors.emplace_back(
					row_column_pair[first_matrix_index_in_pair],
					row_column_pair[second_matrix_index_in_pair],
					std::clamp(static_cast<double>(assignment_matrix(row, assigned_column)) / weights_decimal_precision_, 0.0, 1.0));
			}
			++row;
		}
		return similar_vectors;
	}

	template <size_t CountVectorDimension>
	double cm_max_weighted_bipartite_matching<CountVectorDimension>::calc_vector_l2_norm(
		const count_vector<CountVectorDimension>& vector) noexcept
	{
		size_t sum_of_squares = 0;
		for (const auto& vector_value : vector)
		{
			sum_of_squares += vector_value.value() * vector_value.value();
		}
		return std::sqrt(sum_of_squares);
	}

	template <size_t CountVectorDimension>
	double cm_max_weighted_bipartite_matching<CountVectorDimension>::calc_vectors_distance(
		const count_vector<CountVectorDimension>& first_vector,
		const count_vector<CountVectorDimension>& second_vector) noexcept
	{
		std::array<size_t, CountVectorDimension> squared_differences{};

		std::transform(first_vector.begin(), first_vector.end(), second_vector.begin(), squared_differences.begin(),
			[](const count_vector_value& value_from_first, const count_vector_value& value_from_second)
			{ return pow(static_cast<int>(value_from_first.value()) - value_from_second.value(), 2); });

		const size_t sum_of_squared_differences = std::accumulate(squared_differences.cbegin(), squared_differences.cend(), 0);
		return std::sqrt(sum_of_squared_differences);
	}

	template <size_t CountVectorDimension>
	double cm_max_weighted_bipartite_matching<CountVectorDimension>::normilize_distance(
		const count_vector<CountVectorDimension>& first_vector,
		const count_vector<CountVectorDimension>& second_vector,
		double distance) noexcept
	{
		if (distance < min_not_zero_distance)
		{
			return 0;
		}
		return distance / std::max(calc_vector_l2_norm(first_vector), calc_vector_l2_norm(second_vector));
	}

}