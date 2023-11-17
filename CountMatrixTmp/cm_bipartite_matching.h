#pragma once
#include "cm_comparing_abstract.h"
#include "cv_bipartite_graph_distances_matrix.h"
#include "bipartite_matching_abstract.h"
#include "bipartite_matching_result_to_similarity.h"
#include "nullptr_exception.h"
#include <memory>

namespace cm
{
	template <size_t CountVectorDimension, utility::non_const_arithmetic DistanceT,
	similarity_concept SimilarityT> requires cv_dimension_concept<CountVectorDimension>
	class cm_bipartite_matching final : public cm_comparing_abstract<CountVectorDimension, DistanceT, SimilarityT>
	{
	public:
		using distance_function = cv_abstract_metrics<CountVectorDimension, DistanceT>;
		using distance_func_ptr = std::shared_ptr<distance_function>;
		using bipartite_matching_ptr = std::shared_ptr<bipartite_matching_abstract<DistanceT>>;
		using matching_result_to_similarity_ptr = std::shared_ptr<bipartite_matching_result_to_similarity<DistanceT, SimilarityT>>;

	private:
		distance_func_ptr distance_func_{};
		bipartite_matching_ptr bipartite_matching_{};
		matching_result_to_similarity_ptr matching_result_to_similarity_{};

	public:
		constexpr explicit cm_bipartite_matching(distance_func_ptr distance_func, 
			bipartite_matching_ptr bipartite_matching, 
			matching_result_to_similarity_ptr matching_result_to_similarity)
		: distance_func_(std::move(distance_func)), bipartite_matching_(std::move(bipartite_matching)),
		matching_result_to_similarity_(std::move(matching_result_to_similarity))
		{
			if (distance_func == nullptr || bipartite_matching == nullptr || matching_result_to_similarity == nullptr)
			{
				throw common_exceptions::nullptr_exception("cm_bipartite_matching: nullptr was passed.");
			}
		}


		[[nodiscard]] cm_comparing_result<CountVectorDimension, DistanceT, SimilarityT> operator()(
			const count_matrix<CountVectorDimension>& first_cm,
			const count_matrix<CountVectorDimension>& second_cm) const override
		{
			const cv_bipartite_graph_distances_matrix<DistanceT> cv_distances_matrix{ first_cm, second_cm, *distance_func_ };
			const bipartite_graph_weights_matrix<DistanceT>& weights_matrix = cv_distances_matrix.to_weights_matrix();

			bipartite_matching_result<DistanceT> bipartite_matching_result = bipartite_matching_->match_parts(weights_matrix);

			const SimilarityT matrices_similarity = (*matching_result_to_similarity_)(weights_matrix, 
				bipartite_matching_result);

			return cm_comparing_result<CountVectorDimension, DistanceT, SimilarityT>{ matrices_similarity,
				std::move(bipartite_matching_result).matching_edges() };
		}
	};
}