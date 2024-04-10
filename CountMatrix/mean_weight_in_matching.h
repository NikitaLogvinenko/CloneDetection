#pragma once
#include "similarity_estimator_abstract.h"
#include "continuous_similarity_bounded.h"

namespace cm
{
	template <utility::non_const_arithmetic WeightT, utility::continuous_similarity_bounded_below SimilarityT>
	class mean_weight_in_matching final : public similarity_estimator_abstract<WeightT, SimilarityT>
	{
	public:
		[[nodiscard]] constexpr SimilarityT operator()(
			const graphs::bipartite_graph_weights_matrix<WeightT>& weights_matrix,
			const graphs::bipartite_matching_result<WeightT>& matching_result) const override
		{
			const size_t max_part_cardinality = std::max(weights_matrix.rows(), weights_matrix.columns());
			if (max_part_cardinality == 0)
			{
				return SimilarityT{ SimilarityT::min_similarity_value() };
			}

			const double mean_weight = static_cast<double>(matching_result.sum_of_matching_edges()) / max_part_cardinality;
			return SimilarityT{ std::max(mean_weight, SimilarityT::min_similarity_value()) };
		}
	};

	template <utility::non_const_arithmetic WeightT, continuous_similarity_bounded SimilarityT>
	class mean_weight_in_matching<WeightT, SimilarityT> final : public similarity_estimator_abstract<WeightT, SimilarityT>
	{
	public:
		[[nodiscard]] constexpr SimilarityT operator()(
			const graphs::bipartite_graph_weights_matrix<WeightT>& weights_matrix,
			const graphs::bipartite_matching_result<WeightT>& matching_result) const override
		{
			const size_t max_part_cardinality = std::max(weights_matrix.rows(), weights_matrix.columns());
			if (max_part_cardinality == 0)
			{
				return SimilarityT{ SimilarityT::min_similarity_value() };
			}

			const double mean_weight = static_cast<double>(matching_result.sum_of_matching_edges()) / max_part_cardinality;
			return SimilarityT{ std::clamp(mean_weight, SimilarityT::min_similarity_value(), SimilarityT::max_similarity_value()) };
		}
	};
}
