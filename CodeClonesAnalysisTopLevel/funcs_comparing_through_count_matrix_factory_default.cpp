#include "funcs_comparing_through_count_matrix_factory_default.h"
#include "input_format_error.h"
#include "relative_similarity.h"
#include "matrices_comparing_by_bipartite_matching.h"
#include "cv_similarity_by_normilized_euclidean_dist.h"
#include "bipartite_matching_max_total_weight.h"
#include "mean_weight_in_matching.h"
#include <format>

namespace
{
	const std::string bipartite_matching_algorithm{ "BipartiteMatching" };

	const std::string count_vector_metrics_param{ "<CountVectorMetrics>" };
	const std::string count_vector_metrics_normilized_euclidean_distance{ "NormilizedEuclideanDistance" };

	const std::string bipartite_matching_param{ "<BipartiteMatchingAlgorithm>" };
	const std::string bipartite_matching_max_total_weight{ "MaxTotalWeight" };

	const std::string similarity_estimator_param{ "<SimilarityEstimator>" };
	const std::string similarity_estimator_mean_weight{ "MeanWeight" };
	
	constexpr size_t var_conditions_count = clang_code_analysis::var_usage_conditions_total;
	using traits = code_clones_analysis_top_level::funcs_pairwise_comparing_traits<var_conditions_count>;

	constexpr size_t count_vector_length = traits::matrices_comparing_traits::count_vector_length;
	using distance_t = traits::matrices_comparing_traits::distance_t;
	using similarity_t = traits::matrices_comparing_traits::similarity_t;

	using cv_metrics = cm::count_vectors_metrics_abstract<count_vector_length, distance_t>;
	using cv_metrics_ptr = std::unique_ptr<const cv_metrics>;

	cv_metrics_ptr create_cv_metrics(const code_clones_analysis_top_level::cmcd_config& config)
	{
		if (config.additional_params().contains(count_vector_metrics_param) 
			&& config.additional_params().at(count_vector_metrics_param) != count_vector_metrics_normilized_euclidean_distance)
		{
			throw common_exceptions::input_format_error{ std::format(
				"create_cv_metrics: only {} metrics supported now.", count_vector_metrics_normilized_euclidean_distance) };
		}

		return std::make_unique<cm::cv_similarity_by_normilized_euclidean_dist<count_vector_length>>(
			traits::matrices_comparing_traits::similarity_t::min_similarity_value(), 
			traits::matrices_comparing_traits::similarity_t::max_similarity_value());
	}

	using bipartite_matching_ptr = std::unique_ptr<const graphs::bipartite_matching_abstract<traits::matrices_comparing_traits::distance_t>>;

	bipartite_matching_ptr create_bipartite_matching(const code_clones_analysis_top_level::cmcd_config& config)
	{
		if (config.additional_params().contains(bipartite_matching_param)
			&& config.additional_params().at(bipartite_matching_param) != bipartite_matching_max_total_weight)
		{
			throw common_exceptions::input_format_error{ std::format(
				"create_bipartite_matching: only {} algorithm supported now.", bipartite_matching_max_total_weight) };
		}

		return std::make_unique<graphs::bipartite_matching_max_total_weight<distance_t>>();
	}

	using similarity_estimator_ptr = std::unique_ptr<const cm::similarity_estimator_abstract<distance_t, similarity_t>>;

	similarity_estimator_ptr create_similarity_estimator(const code_clones_analysis_top_level::cmcd_config& config)
	{
		if (config.additional_params().contains(similarity_estimator_param)
			&& config.additional_params().at(similarity_estimator_param) != similarity_estimator_mean_weight)
		{
			throw common_exceptions::input_format_error{ std::format(
				"create_similarity_estimator: only {} estimator supported now.", similarity_estimator_mean_weight) };
		}

		return std::make_unique<cm::mean_weight_in_matching<distance_t, similarity_t>>();
	}

	using cm_comparer_ptr = std::unique_ptr<const cm::matrices_comparer_abstract<traits::matrices_comparing_traits>>;

	cm_comparer_ptr create_comparer(const code_clones_analysis_top_level::cmcd_config& config)
	{
		if (config.matrices_comparing_algorithm() != bipartite_matching_algorithm && !config.matrices_comparing_algorithm().empty())
		{
			throw common_exceptions::input_format_error{ std::format(
				"funcs_comparing_through_count_matrix_factory_default::create_funcs_comparer: only {} algorithm supported now.",
				bipartite_matching_algorithm) };
		}

		return std::make_unique<cm::matrices_comparing_by_bipartite_matching<traits::matrices_comparing_traits>>(
			create_cv_metrics(config), create_bipartite_matching(config), create_similarity_estimator(config)
		);
	}
}

std::unique_ptr<code_clones_analysis_top_level::funcs_comparing_through_count_matrix_factory_abstract<var_conditions_count>::
funcs_pairwise_comparer_through_cm> code_clones_analysis_top_level::funcs_comparing_through_count_matrix_factory_default
::create_funcs_comparer(const cmcd_config& config) const
{
	return std::make_unique<funcs_pairwise_comparer_through_cm>(create_comparer(config), config.count_matrices_comparing_threads());
}
