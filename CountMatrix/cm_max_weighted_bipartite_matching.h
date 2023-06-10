#pragma once
#include "cm_similarity_abstract_algorithm.h"
#include "dlib/optimization/max_cost_assignment.h"

namespace count_matrix
{
	template <size_t CountVectorDimension>
	class cm_max_weighted_bipartite_matching final : public cm_similarity_abstract_algorithm<CountVectorDimension>
	{
		size_t weights_decimals_{};
		double weights_factor_{};

		static constexpr size_t default_decimals = 3;

		static constexpr double min_not_zero_distance = 1e-7;

		static constexpr size_t row_axis = 0;
		static constexpr size_t column_axis = 1;

		inline static constexpr double similarity_range_width = relative_similarity::max_similarity_value() - relative_similarity::min_similarity_value();

	public:
		explicit cm_max_weighted_bipartite_matching(const size_t weights_decimals = default_decimals)
			: weights_decimals_(weights_decimals), weights_factor_(pow(10, weights_decimals)) {}

		[[nodiscard]] count_matrices_similarity_data determine_similarity(
			const count_matrix<CountVectorDimension>& first_cm,
			const count_matrix<CountVectorDimension>& second_cm) const override;

		[[nodiscard]] static size_t default_weights_decimals() noexcept
		{
			return default_decimals;
		}

	private:
		[[nodiscard]] dlib::matrix<size_t> create_assignment_matrix(
			const count_matrix<CountVectorDimension>& first_cm,
			const count_matrix<CountVectorDimension>& second_cm) const;

		[[nodiscard]] size_t calc_assignment_matrix_cell(
			const count_vector<CountVectorDimension>& first_vector,
			const count_vector<CountVectorDimension>& second_vector) const noexcept;

		[[nodiscard]] count_matrices_similarity_data assign(
			const count_matrix<CountVectorDimension>& first_cm,
			const count_matrix<CountVectorDimension>& second_cm,
			const dlib::matrix<size_t>& assignment_matrix) const;

		[[nodiscard]] std::vector<cm_similar_vectors_data> create_similar_vectors_pairs(
			const dlib::matrix<size_t>& assignment_matrix, const std::vector<long>& assignment,
			size_t min_vectors_count, size_t first_matrix_axis_in_assignment_matrix, size_t second_matrix_axis_in_assignment_matrix) const;

		[[nodiscard]] static double calc_count_vectors_distance(
			const count_vector<CountVectorDimension>& first_vector,
			const count_vector<CountVectorDimension>& second_vector) noexcept;

		[[nodiscard]] static double convert_distance_to_similarity(
			const count_vector<CountVectorDimension>& first_vector,
			const count_vector<CountVectorDimension>& second_vector,
			double distance) noexcept;
	};


	template <size_t CountVectorDimension>
	count_matrices_similarity_data cm_max_weighted_bipartite_matching<CountVectorDimension>::determine_similarity(
		const count_matrix<CountVectorDimension>& first_cm, const count_matrix<CountVectorDimension>& second_cm) const
	{
		if (first_cm.empty())
		{
			return second_cm.empty() ?
				count_matrices_similarity_data(relative_similarity(relative_similarity::max_similarity_value()))
			:
				count_matrices_similarity_data(relative_similarity(relative_similarity::min_similarity_value()));
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

		dlib::matrix<size_t> assignment_matrix(
			static_cast<long>(matrix_more_vectors_ptr->vectors_count()), 
			static_cast<long>(matrix_more_vectors_ptr->vectors_count()));
		long row = 0, col = 0;
		for (const auto& vector_from_greater_matrix : *matrix_more_vectors_ptr)
		{
			for (const auto& vector_from_less_matrix : *matrix_less_vectors_ptr)
			{
				assignment_matrix(row, col) = calc_assignment_matrix_cell(vector_from_greater_matrix, vector_from_less_matrix);
				++col;
			}
			while (col != assignment_matrix.nc())
			{
				assignment_matrix(row, col) = calc_assignment_matrix_cell(vector_from_greater_matrix, count_vector<CountVectorDimension>{});
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
		const double vectors_distance = calc_count_vectors_distance(first_vector, second_vector);
		const double vectors_similarity = convert_distance_to_similarity(first_vector, second_vector, vectors_distance);
		return std::lround(vectors_similarity * weights_factor_);
	}

	template <size_t CountVectorDimension>
	count_matrices_similarity_data cm_max_weighted_bipartite_matching<CountVectorDimension>::assign(
		const count_matrix<CountVectorDimension>& first_cm, const count_matrix<CountVectorDimension>& second_cm,
		const dlib::matrix<size_t>& assignment_matrix) const
	{
		const std::vector assignment(max_cost_assignment(assignment_matrix));
		const double max_sum_of_vectors_similarities = static_cast<double>(assignment_cost(assignment_matrix, assignment)) / weights_factor_;
		const relative_similarity matrices_similarity{ std::clamp(max_sum_of_vectors_similarities / assignment_matrix.nc(),
			relative_similarity::min_similarity_value(), relative_similarity::max_similarity_value()) };


		if (first_cm.vectors_count() < second_cm.vectors_count())
		{
			return count_matrices_similarity_data(matrices_similarity, create_similar_vectors_pairs(assignment_matrix, assignment, first_cm.vectors_count(),
				column_axis, row_axis));
		}
		return count_matrices_similarity_data(matrices_similarity, create_similar_vectors_pairs(assignment_matrix, assignment, second_cm.vectors_count(),
			row_axis, column_axis));
	}

	template <size_t CountVectorDimension>
	std::vector<cm_similar_vectors_data> cm_max_weighted_bipartite_matching<CountVectorDimension>::create_similar_vectors_pairs(
		const dlib::matrix<size_t>& assignment_matrix, const std::vector<long>& assignment, const size_t min_vectors_count,
		const size_t first_matrix_axis_in_assignment_matrix, const size_t second_matrix_axis_in_assignment_matrix) const
	{
		std::vector<cm_similar_vectors_data> similar_vectors{};
		similar_vectors.reserve(min_vectors_count);
		long row = 0;
		for (const long& assigned_column : assignment)
		{
			if (static_cast<size_t>(assigned_column) < min_vectors_count)
			{
				const std::array row_column_pair{ index_of_count_vector(row), index_of_count_vector(assigned_column) };
				similar_vectors.emplace_back(
					row_column_pair[first_matrix_axis_in_assignment_matrix],
					row_column_pair[second_matrix_axis_in_assignment_matrix],
					relative_similarity{ std::clamp(static_cast<double>(assignment_matrix(row, assigned_column)) / static_cast<double>(weights_factor_),
						relative_similarity::min_similarity_value(), relative_similarity::max_similarity_value()) });
			}
			++row;
		}
		return similar_vectors;
	}

	template <size_t CountVectorDimension>
	double cm_max_weighted_bipartite_matching<CountVectorDimension>::calc_count_vectors_distance(
		const count_vector<CountVectorDimension>& first_vector,
		const count_vector<CountVectorDimension>& second_vector) noexcept
	{
		std::array<size_t, CountVectorDimension> squared_differences{};

		std::transform(first_vector.begin(), first_vector.end(), second_vector.begin(), squared_differences.begin(),
			[](const count_vector_value& value_from_first, const count_vector_value& value_from_second)
			{ return pow(static_cast<int>(value_from_first.value()) - static_cast<int>(value_from_second.value()), 2); });

		const size_t sum_of_squared_differences = std::accumulate(squared_differences.cbegin(), squared_differences.cend(), size_t{});
		return std::sqrt(sum_of_squared_differences);
	}

	template <size_t CountVectorDimension>
	double cm_max_weighted_bipartite_matching<CountVectorDimension>::convert_distance_to_similarity(
		const count_vector<CountVectorDimension>& first_vector,
		const count_vector<CountVectorDimension>& second_vector,
		const double distance) noexcept
	{
		if (distance < min_not_zero_distance)
		{
			return relative_similarity::max_similarity_value();
		}

		std::array<size_t, CountVectorDimension> max_squared_value_in_pair{};

		std::transform(first_vector.begin(), first_vector.end(), second_vector.begin(), max_squared_value_in_pair.begin(),
			[](const count_vector_value& value_from_first, const count_vector_value& value_from_second)
			{ return pow(std::max(value_from_first.value(), value_from_second.value()), 2); });

		const size_t sum_of_max_squared_values = std::accumulate(max_squared_value_in_pair.cbegin(), max_squared_value_in_pair.cend(), size_t{});
		const double normilized_distance = distance / std::sqrt(sum_of_max_squared_values);
		const double similarity = relative_similarity::min_similarity_value() + similarity_range_width * (1 - normilized_distance);
		return std::clamp(similarity, relative_similarity::min_similarity_value(), relative_similarity::max_similarity_value());
	}

}