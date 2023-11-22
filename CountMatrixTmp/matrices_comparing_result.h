#pragma once
#include "bipartite_matching_result.h"
#include "count_matrix.h"
#include "continuous_similarity_bounded_below.h"
#include "matched_vectors_pair_index.h"
#include "distance_between_count_vectors.h"

namespace cm
{
	template <size_t CountVectorDimension, utility::non_const_arithmetic DistanceT,
	continuous_similarity_bounded_below SimilarityT>
	requires count_vector_dimension<CountVectorDimension>
	class matrices_comparing_result final
	{
		SimilarityT matrices_similarity_{};
		std::vector<edge<DistanceT>> matched_vectors_{};

	public:
		constexpr matrices_comparing_result() noexcept = default;

		constexpr explicit matrices_comparing_result(const SimilarityT matrices_similarity,
			std::vector<edge<DistanceT>> matched_vectors)
			noexcept(std::is_nothrow_copy_constructible_v<SimilarityT>)
			: matrices_similarity_(matrices_similarity), matched_vectors_(std::move(matched_vectors)) {}

		[[nodiscard]] constexpr SimilarityT matrices_similarity() const noexcept
		{
			return matrices_similarity_;
		}

		[[nodiscard]] constexpr size_t matched_vectors_count() const noexcept
		{
			return matched_vectors_.size();
		}

		[[nodiscard]] constexpr vertex_index vector_from_first_matrix(const matched_vectors_pair_index index_of_pair)
		{
			return matched_vectors_.at(index_of_pair.to_size_t()).first_vertex_index();
		}

		[[nodiscard]] constexpr vertex_index vector_from_second_matrix(const matched_vectors_pair_index index_of_pair)
		{
			return matched_vectors_.at(index_of_pair.to_size_t()).second_vertex_index();
		}

		[[nodiscard]] constexpr distance_between_count_vectors<DistanceT> distance_between_matched_vectors(
			const matched_vectors_pair_index index_of_pair)
		{
			const edge_weight<DistanceT> weight = matched_vectors_.at(index_of_pair.to_size_t()).weight();
			return distance_between_count_vectors<DistanceT>(weight.value());
		}
	};
}
