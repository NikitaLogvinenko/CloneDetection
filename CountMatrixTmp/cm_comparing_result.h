#pragma once
#include "bipartite_matching_result.h"
#include "count_matrix.h"
#include "similarity_concept.h"
#include "matched_variables_index.h"
#include "distance_between_cv.h"

namespace cm
{
	template <size_t CountVectorDimension, utility::non_const_arithmetic DistanceT,
		similarity_concept SimilarityT> requires cv_dimension_concept<CountVectorDimension>
		class cm_comparing_result final
	{
		SimilarityT matrices_similarity_{};
		std::vector<edge<DistanceT>> matched_variables_{};

	public:
		constexpr cm_comparing_result() noexcept = default;

		constexpr explicit cm_comparing_result(const SimilarityT matrices_similarity,
			std::vector<edge<DistanceT>> matched_variables)
			noexcept(std::is_nothrow_copy_constructible_v<SimilarityT>)
			: matrices_similarity_(matrices_similarity), matched_variables_(std::move(matched_variables)) {}

		[[nodiscard]] constexpr SimilarityT matrices_similarity() const noexcept
		{
			return matrices_similarity_;
		}

		[[nodiscard]] constexpr size_t matched_variables_count() const noexcept
		{
			return matched_variables_.size();
		}

		[[nodiscard]] constexpr vertex_index index_of_variable_from_first_cm(const matched_variables_index index_of_pair)
		{
			return matched_variables_.at(index_of_pair.to_size_t()).first_vertex_index();
		}

		[[nodiscard]] constexpr vertex_index index_of_variable_from_second_cm(const matched_variables_index index_of_pair)
		{
			return matched_variables_.at(index_of_pair.to_size_t()).second_vertex_index();
		}

		[[nodiscard]] constexpr distance_between_cv<DistanceT> distance_between_matched_variables(const matched_variables_index index_of_pair)
		{
			const edge_weight<DistanceT> weight = matched_variables_.at(index_of_pair.to_size_t()).weight();
			return distance_between_cv<DistanceT>(weight.value());
		}
	};
}
