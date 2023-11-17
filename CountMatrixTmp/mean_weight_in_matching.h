#pragma once
#include "bipartite_matching_result_to_similarity.h"

namespace cm
{
	template <utility::non_const_arithmetic WeightT, similarity_concept SimilarityT>
	class mean_weight_in_matching final : public bipartite_matching_result_to_similarity<WeightT, SimilarityT>
	{
	public:
		[[nodiscard]] constexpr SimilarityT operator()(
			const bipartite_graph_weights_matrix<WeightT>& weights_matrix,
			const bipartite_matching_result<WeightT>& matching_result) const override
		{
			const size_t max_part_cardinality = std::max(weights_matrix.rows(), weights_matrix.columns());
			if (max_part_cardinality == 0)
			{
				return SimilarityT{ SimilarityT::min_similarity_value() };
			}

			const double mean_weight = static_cast<double>(matching_result.sum_of_matching_edges()) / max_part_cardinality;
			return SimilarityT{ mean_weight };
		}
	};

	template <similarity_concept SimilarityT>
	class mean_weight_in_matching final : public bipartite_matching_result_to_similarity<double, SimilarityT>
	{
	public:
		[[nodiscard]] constexpr SimilarityT operator()(
			const bipartite_graph_weights_matrix<double>& weights_matrix,
			const bipartite_matching_result<double>& matching_result) const override
		{
			const size_t max_part_cardinality = std::max(weights_matrix.rows(), weights_matrix.columns());
			if (max_part_cardinality == 0)
			{
				return SimilarityT{ SimilarityT::min_similarity_value() };
			}

			const double mean_weight = matching_result.sum_of_matching_edges() / max_part_cardinality;
			return SimilarityT{ mean_weight };
		}
	};
}