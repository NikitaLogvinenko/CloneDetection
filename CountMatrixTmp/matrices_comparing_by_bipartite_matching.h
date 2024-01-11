#pragma once
#include "matrices_comparing_abstract.h"
#include "bipartite_graph_on_count_matrices.h"
#include "bipartite_matching_abstract.h"
#include "similarity_estimator_abstract.h"
#include "nullptr_exception.h"
#include <memory>

namespace cm
{
	template <size_t CountVectorDimension, utility::non_const_arithmetic DistanceT,
	continuous_similarity_bounded_below SimilarityT> requires count_vector_length<CountVectorDimension>
	class matrices_comparing_by_bipartite_matching final : public matrices_comparing_abstract<CountVectorDimension, DistanceT, SimilarityT>
	{
	public:
		using metrics = count_vectors_metrics_abstract<CountVectorDimension, DistanceT>;
		using metrics_ptr = std::unique_ptr<metrics>;
		using bipartite_matching_ptr = std::unique_ptr<bipartite_matching_abstract<DistanceT>>;
		using similarity_estimator_ptr = std::unique_ptr<similarity_estimator_abstract<DistanceT, SimilarityT>>;

	private:
		metrics_ptr metrics_{};
		bipartite_matching_ptr bipartite_matching_{};
		similarity_estimator_ptr matching_result_to_similarity_{};

	public:
		constexpr explicit matrices_comparing_by_bipartite_matching(metrics_ptr metrics_for_count_vectors, 
			bipartite_matching_ptr bipartite_matching, 
			similarity_estimator_ptr matching_result_to_similarity)
		: metrics_(std::move(metrics_for_count_vectors)), bipartite_matching_(std::move(bipartite_matching)),
		matching_result_to_similarity_(std::move(matching_result_to_similarity))
		{
			if (metrics_for_count_vectors == nullptr || bipartite_matching == nullptr || matching_result_to_similarity == nullptr)
			{
				throw common_exceptions::nullptr_exception("matrices_comparing_by_bipartite_matching: nullptr was passed.");
			}
		}


		[[nodiscard]] matrices_comparing_result<CountVectorDimension, DistanceT, SimilarityT> operator()(
			const count_matrix<CountVectorDimension>& first_matrix,
			const count_matrix<CountVectorDimension>& second_matrix) const override
		{
			const bipartite_graph_on_count_matrices<DistanceT> cv_distances_matrix{ first_matrix, second_matrix, *metrics_ };
			const bipartite_graph_weights_matrix<DistanceT>& weights_matrix = cv_distances_matrix.to_weights_matrix();

			bipartite_matching_result<DistanceT> bipartite_matching_result = bipartite_matching_->match_parts(weights_matrix);

			const SimilarityT matrices_similarity = (*matching_result_to_similarity_)(weights_matrix, 
				bipartite_matching_result);

			return matrices_comparing_result<CountVectorDimension, DistanceT, SimilarityT>{ matrices_similarity,
				std::move(bipartite_matching_result).matching_edges() };
		}
	};
}
