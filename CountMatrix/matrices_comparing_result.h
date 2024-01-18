#pragma once
#include "bipartite_matching_result.h"
#include "count_matrix.h"
#include "continuous_similarity_bounded_below.h"
#include "matching_edge_index.h"
#include "count_vector_index.h"
#include "distance_between_count_vectors.h"

namespace cm
{
	template <size_t CountVectorLength, utility::non_const_arithmetic DistanceT, continuous_similarity_bounded_below SimilarityT>
	requires count_vector_length<CountVectorLength>
	class matrices_comparing_result final
	{
		SimilarityT matrices_similarity_{};
		std::vector<graphs::edge<DistanceT>> matched_vectors_{};

	public:
		constexpr matrices_comparing_result() noexcept = default;

		constexpr matrices_comparing_result(const SimilarityT matrices_similarity, 
			std::vector<graphs::edge<DistanceT>> matched_vectors) noexcept(std::is_nothrow_copy_constructible_v<SimilarityT>)
			: matrices_similarity_(matrices_similarity), matched_vectors_(std::move(matched_vectors)) {}

		[[nodiscard]] constexpr SimilarityT matrices_similarity() const noexcept(std::is_nothrow_copy_constructible_v<SimilarityT>)
		{
			return matrices_similarity_;
		}

		[[nodiscard]] constexpr size_t matched_vectors_count() const noexcept
		{
			return matched_vectors_.size();
		}

		[[nodiscard]] constexpr count_vector_index vector_from_first_matrix(const matching_edge_index edge_index)
		{
			return count_vector_index{ matched_vectors_.at(edge_index.to_size_t()).first_vertex_index().to_size_t() };
		}

		[[nodiscard]] constexpr count_vector_index vector_from_second_matrix(const matching_edge_index edge_index)
		{
			return count_vector_index{ matched_vectors_.at(edge_index.to_size_t()).second_vertex_index().to_size_t() };
		}

		[[nodiscard]] constexpr distance_between_count_vectors<DistanceT> distance_between_matched_vectors(
			const matching_edge_index index_of_pair)
		{
			const graphs::edge_weight<DistanceT> weight = matched_vectors_.at(index_of_pair.to_size_t()).weight();
			return distance_between_count_vectors<DistanceT>(weight);
		}
	};
}
