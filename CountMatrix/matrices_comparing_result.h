#pragma once
#include "bipartite_matching_result.h"
#include "count_matrix.h"
#include "matrices_similarity.h"
#include "matched_vectors_index.h"
#include "count_vector_index.h"
#include "distance_between_count_vectors.h"
#include "matrices_comparing_traits.h"

namespace cm
{
	template <matrices_comparing_traits CompTraits>
	class matrices_comparing_result final
	{
		using similarity_t = typename CompTraits::similarity_t;
		using distance_t = typename CompTraits::distance_t;

		matrices_similarity<similarity_t> similarity_{};
		std::vector<graphs::edge<distance_t>> matched_vectors_{};

	public:
		constexpr matrices_comparing_result() noexcept = default;

		constexpr matrices_comparing_result(const matrices_similarity<similarity_t> similarity,
			std::vector<graphs::edge<distance_t>> matched_vectors) noexcept
			: similarity_(similarity), matched_vectors_(std::move(matched_vectors)) {}

		[[nodiscard]] constexpr matrices_similarity<similarity_t> similarity() const noexcept
		{
			return similarity_;
		}

		[[nodiscard]] constexpr size_t matched_vectors_count() const noexcept
		{
			return matched_vectors_.size();
		}

		[[nodiscard]] constexpr count_vector_index vector_from_first_cm(const matched_vectors_index matching_index)
		{
			return count_vector_index{ matched_vectors_.at(matching_index.to_size_t()).first_vertex_index().to_size_t() };
		}

		[[nodiscard]] constexpr count_vector_index vector_from_second_cm(const matched_vectors_index matching_index)
		{
			return count_vector_index{ matched_vectors_.at(matching_index.to_size_t()).second_vertex_index().to_size_t() };
		}

		[[nodiscard]] constexpr distance_between_count_vectors<distance_t> distance_between_matched_vectors(
			const matched_vectors_index matching_index)
		{
			const graphs::edge_weight<distance_t> weight = matched_vectors_.at(matching_index.to_size_t()).weight();
			return distance_between_count_vectors<distance_t>(weight);
		}
	};
}
