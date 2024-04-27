#pragma once
#include "funcs_comparing_through_count_matrix_factory_abstract.h"
#include "input_format_error.h"
#include "matrices_comparing_by_bipartite_matching.h"
#include "cv_similarity_by_normilized_euclidean_dist.h"
#include "bipartite_matching_max_total_weight.h"
#include "mean_weight_in_matching.h"
#include <format>

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class funcs_comparing_through_count_matrix_factory_default final
		: public funcs_comparing_through_count_matrix_factory_abstract<ConditionsCount>
	{
	public:
		using traits = funcs_pairwise_comparing_traits<ConditionsCount>;
		using funcs_pairwise_comparer_through_cm = code_clones_analysis_through_cm::code_entities_pairwise_comparer_through_cm<traits>;

	private:
		static const std::string bipartite_matching_algorithm{ "BipartiteMatching" };

		static const std::string count_vector_metrics_param{ "<CountVectorMetrics>" };
		static const std::string count_vector_metrics_normilized_euclidean_distance{ "NormilizedEuclideanDistance" };

		static const std::string bipartite_matching_param{ "<BipartiteMatchingAlgorithm>" };
		static const std::string bipartite_matching_max_total_weight{ "MaxTotalWeight" };

		static const std::string similarity_estimator_param{ "<SimilarityEstimator>" };
		static const std::string similarity_estimator_mean_weight{ "MeanWeight" };

		using distance_t = typename traits::matrices_comparing_traits::distance_t;
		using similarity_t = typename traits::matrices_comparing_traits::similarity_t;

		using cv_metrics = cm::count_vectors_metrics_abstract<ConditionsCount, distance_t>;
		using cv_metrics_ptr = std::unique_ptr<cv_metrics>;

		using bipartite_matching_ptr = std::unique_ptr<graphs::bipartite_matching_abstract<typename traits::matrices_comparing_traits::distance_t>>;

		using similarity_estimator_ptr = std::unique_ptr<cm::similarity_estimator_abstract<distance_t, similarity_t>>;

		using cm_comparer_ptr = std::unique_ptr<cm::matrices_comparer_abstract<typename traits::matrices_comparing_traits>>;

	public:
		[[nodiscard]] std::unique_ptr<funcs_pairwise_comparer_through_cm> create_funcs_comparer(
			const cmcd_config& config) const override
		{
			return std::make_unique<funcs_pairwise_comparer_through_cm>(create_comparer(config), config.count_matrices_comparing_threads());
		}

	private:
		[[nodiscard]] static cv_metrics_ptr create_cv_metrics(const cmcd_config& config)
		{
			if (config.additional_params().contains(count_vector_metrics_param)
				&& config.additional_params().at(count_vector_metrics_param) != count_vector_metrics_normilized_euclidean_distance)
			{
				throw common_exceptions::input_format_error{ std::format(
					"create_cv_metrics: only {} metrics supported now.", count_vector_metrics_normilized_euclidean_distance) };
			}

			return std::make_unique<cm::cv_similarity_by_normilized_euclidean_dist<ConditionsCount>>(
				traits::matrices_comparing_traits::similarity_t::min_similarity_value(),
				traits::matrices_comparing_traits::similarity_t::max_similarity_value());
		}


		[[nodiscard]] static bipartite_matching_ptr create_bipartite_matching(const cmcd_config& config)
		{
			if (config.additional_params().contains(bipartite_matching_param)
				&& config.additional_params().at(bipartite_matching_param) != bipartite_matching_max_total_weight)
			{
				throw common_exceptions::input_format_error{ std::format(
					"create_bipartite_matching: only {} algorithm supported now.", bipartite_matching_max_total_weight) };
			}

			return std::make_unique<graphs::bipartite_matching_max_total_weight<distance_t>>();
		}


		[[nodiscard]] static similarity_estimator_ptr create_similarity_estimator(const cmcd_config& config)
		{
			if (config.additional_params().contains(similarity_estimator_param)
				&& config.additional_params().at(similarity_estimator_param) != similarity_estimator_mean_weight)
			{
				throw common_exceptions::input_format_error{ std::format(
					"create_similarity_estimator: only {} estimator supported now.", similarity_estimator_mean_weight) };
			}

			return std::make_unique<cm::mean_weight_in_matching<distance_t, similarity_t>>();
		}


		[[nodiscard]] static cm_comparer_ptr create_comparer(const cmcd_config& config)
		{
			if (config.matrices_comparing_algorithm() != bipartite_matching_algorithm && !config.matrices_comparing_algorithm().empty())
			{
				throw common_exceptions::input_format_error{ std::format(
					"funcs_comparing_through_count_matrix_factory_default::create_funcs_comparer: only {} algorithm supported now.",
					bipartite_matching_algorithm) };
			}

			return std::make_unique<cm::matrices_comparing_by_bipartite_matching<typename traits::matrices_comparing_traits>>(
				create_cv_metrics(config), create_bipartite_matching(config), create_similarity_estimator(config)
			);
		}
	};
}
